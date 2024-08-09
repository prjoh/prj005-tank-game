#include <AssetManager.h>


// TODO: Why???
#define PUGIXML_HEADER_ONLY
#include <pugixml.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <Error.h>


using namespace pd;


AssetManager::AssetManager()
{
    m_loadedTextures.reserve(100);
}

bool AssetManager::loadTexture(const std::string& path)
{
    Texture texture = LoadTexture(path.c_str());
    
    // Get the fileName from the path
    auto fileName = path.substr(path.find_last_of("/\\") + 1);
    
    // Remove the file extension from the fileName
    size_t dotPos = fileName.find_last_of(".");
    if (dotPos != std::string::npos) {
        fileName = fileName.substr(0, dotPos);
    }
    
    auto key = std::hash<std::string_view>{}(fileName);

    auto it = m_textureHandles.find(key);
    if (it != m_textureHandles.end())
    {
        // LOG_ERROR(); // TODO
        ASSERT(false);
        return false;
    }

    m_textureHandles.emplace(key, (u32)m_loadedTextures.size());
    m_loadedTextures.emplace_back(texture);

    return true;
}

#include <iostream>

bool pd::AssetManager::loadGameData(const std::string& path)
{
    using namespace pugi;

    if (!m_gameDataSrc.load_file(path.c_str()))
        return false;

    xml_node entities = m_gameDataSrc.child("entities");
    for (xml_node entity = entities.child("entity"); entity; entity = entity.next_sibling("entity"))
    {
        std::cout << "Id: " << entity.attribute("id").value() << "\n";

        auto id = entity.attribute("id").value();

        EntityData data;
        data.name = id;

        xml_node components = entity.child("components");
        for (xml_node component = components.child("component"); component; component = component.next_sibling("component"))
        {
            std::cout << "    Type: " << component.attribute("type").value() << "\n";

            auto componentType = component.attribute("type").value();

            ComponentData compData;
            auto type = getComponentType(componentType);
            ASSERT_MSG(type != eComponentType::Null, "Component type `%s` not found!", componentType.c_str());
            compData.type = type;

            processComponentArgDataNode(component, compData.argsRoot);

            for (xml_node arg = component.child("arg"); arg; arg = arg.next_sibling("arg"))
            {
                std::cout << "        Arg: " << arg.attribute("name").value() << " | "  << arg.attribute("value").value() << "\n";

                compData.args.emplace(std::make_pair(
                    std::hash<std::string_view>{}(arg.attribute("name").value()),
                    arg.attribute("value")
                ));
            }

            for (xml_node argList = component.child("argList"); argList; argList = argList.next_sibling("argList"))
            {
                using arg_list_type = std::vector<std::pair<size_t, pugi::xml_attribute>>;
                
                arg_list_type args;
                for (xml_node arg = argList.child("arg"); arg; arg = arg.next_sibling("arg"))
                {
                    args.emplace_back(std::make_pair(
                        std::hash<std::string_view>{}(arg.attribute("name").value()),
                        arg.attribute("value")
                    ));
                }
                compData.argLists.emplace(std::make_pair(
                    std::hash<std::string_view>{}(argList.attribute("name").value()),
                    args
                ));
            }

            data.components.emplace_back(compData);
        }

        xml_node parent = entity.child("parent");
        if (parent)
        {
            std::cout << "Parent: " << parent.attribute("id").value() << "\n";

            data.parent = parent.attribute("id").value();
        }

        m_gameData.emplace_back(data);
    }

    return true;
}

bool pd::AssetManager::loadLevelData(const std::string& pathPrefix)
{
    std::string bottomLayerPath = pathPrefix + "._Bottom.csv";
    std::string objectsLayerPath = pathPrefix + "._Objects.csv";
    std::string collisionLayerPath = pathPrefix + "._Collision.csv";

    // Get the fileName from the path
    auto fileName = pathPrefix.substr(pathPrefix.find_last_of("/\\") + 1);

    LevelData level{ fileName };
    level.levelLayers.push_back({
        -3,  // drawLayer
        parseLevelLayerData(bottomLayerPath),
    });
    level.levelLayers.push_back({
        -1,  // drawLayer
        parseLevelLayerData(objectsLayerPath),
    });
    level.collisionLayer = parseLevelLayerData(collisionLayerPath);

    std::cout << "Loaded level data: " << fileName << "\n";

    m_levelDataHandles.emplace(std::make_pair(
        std::hash<std::string_view>{}(fileName),
        (u32)m_levelData.size() 
    ));
    m_levelData.emplace_back(level);
    return true;
}

observer_ptr<Texture2D> AssetManager::getTexture(const std::string& textureName)
{
    auto key = std::hash<std::string_view>{}(textureName);
    auto it = m_textureHandles.find(key);

    if (it == m_textureHandles.end())
        return nullptr;

    return &m_loadedTextures[it->second];
}

observer_ptr<LevelData> AssetManager::getLevelData(const std::string& levelName)
{
    auto key = std::hash<std::string_view>{}(levelName);
    auto it = m_levelDataHandles.find(key);

    if (it == m_levelDataHandles.end())
        return nullptr;

    return &m_levelData[it->second];
}

void pd::AssetManager::processComponentArgDataNode(const pugi::xml_node& node, pd::ComponentArgData& data)
{
    using namespace pugi;

    for (xml_node arg = node.child("arg"); arg; arg = arg.next_sibling("arg"))
    {
        data.children.emplace(std::make_pair(
            std::hash<std::string_view>{}(arg.attribute("name").value()),
            ComponentArgData{
                eComponentArgType::Arg,
                &data,
                arg.attribute("name").value(),
                arg.attribute("value"),
            }
        ));
    }

    for (xml_node argList = node.child("argList"); argList; argList = argList.next_sibling("argList"))
    {
        auto key = std::hash<std::string_view>{}(argList.attribute("name").value());
        data.children.emplace(std::make_pair(
            key,
            ComponentArgData{
                eComponentArgType::ArgList,
                &data,
                argList.attribute("name").value(),
            }
        ));

        processComponentArgDataNode(argList, data.children.at(key));
    }


    for (xml_node argTuple = node.child("argTuple"); argTuple; argTuple = argTuple.next_sibling("argTuple"))
    {
        auto key = std::hash<std::string_view>{}(argTuple.attribute("name").value());
        data.children.emplace(std::make_pair(
            key,
            ComponentArgData{
                eComponentArgType::ArgTuple,
                &data,
                argTuple.attribute("name").value(),
            }
        ));

        processComponentArgDataNode(argTuple, data.children.at(key));
    }
}

std::vector<std::vector<s32>> pd::AssetManager::parseLevelLayerData(const std::string& path)
{
    std::vector<std::vector<s32>> data;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return data; // Return an empty vector
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<s32> row;
        std::istringstream iss(line);
        std::string value;

        while (std::getline(iss, value, ',')) {
            try {
                s32 num = std::stoi(value);
                row.push_back(num);
            }
            catch (const std::exception& e) {
                // TODO: Propert error logging
                // Handle conversion error if needed
                std::cerr << "Error converting value: " << value << " - " << e.what() << std::endl;
                ASSERT(false);
            }
        }

        data.push_back(row);
    }

    file.close();
    return data;
}
