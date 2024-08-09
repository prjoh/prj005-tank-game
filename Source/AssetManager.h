#pragma once


#include <string>
#include <unordered_map>

#include <pugixml.hpp>

#include <Component.h>
#include <Components/ComponentTypes.h>
#include <Raylib.h>
#include <Types.h>

//RLAPI Image LoadImage(const char* fileName);                                                             // Load image from file into CPU memory (RAM)
//RLAPI Image LoadImageRaw(const char* fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
//RLAPI Image LoadImageAnim(const char* fileName, int* frames);                                            // Load image sequence from file (frames appended to image.data)
//RLAPI Image LoadImageFromMemory(const char* fileType, const unsigned char* fileData, int dataSize);      // Load image from memory buffer, fileType refers to extension: i.e. '.png'
//RLAPI Image LoadImageFromTexture(Texture2D texture);                                                     // Load image from GPU texture data
//RLAPI Image LoadImageFromScreen(void);                                                                   // Load image from screen buffer and (screenshot)
//RLAPI bool IsImageReady(Image image);                                                                    // Check if an image is ready
//RLAPI void UnloadImage(Image image);                                                                     // Unload image from CPU memory (RAM)
//RLAPI bool ExportImage(Image image, const char* fileName);                                               // Export image data to file, returns true on success
//RLAPI bool ExportImageAsCode(Image image, const char* fileName);

//RLAPI Texture2D LoadTexture(const char* fileName);                                                       // Load texture from file into GPU memory (VRAM)
//RLAPI Texture2D LoadTextureFromImage(Image image);                                                       // Load texture from image data
//RLAPI TextureCubemap LoadTextureCubemap(Image image, int layout);                                        // Load cubemap from image, multiple image cubemap layouts supported
//RLAPI RenderTexture2D LoadRenderTexture(int width, int height);                                          // Load texture for rendering (framebuffer)
//RLAPI bool IsTextureReady(Texture2D texture);                                                            // Check if a texture is ready
//RLAPI void UnloadTexture(Texture2D texture);                                                             // Unload texture from GPU memory (VRAM)
//RLAPI bool IsRenderTextureReady(RenderTexture2D target);                                                 // Check if a render texture is ready
//RLAPI void UnloadRenderTexture(RenderTexture2D target);                                                  // Unload render texture from GPU memory (VRAM)
//RLAPI void UpdateTexture(Texture2D texture, const void* pixels);                                         // Update GPU texture with new data
//RLAPI void UpdateTextureRec(Texture2D texture, Rectangle rec, const void* pixels);


namespace pd
{
	enum class eComponentArgType
	{
		Root,
		Arg,
		ArgList,
		ArgTuple,
	};

	//struct ComponentArgLeaf
	//{
	//	ComponentArgNode* parent;
	//	pugi::xml_attribute* pAttribute{ nullptr };
	//};

	struct ComponentArgData
	{
		eComponentArgType type;
		
		ComponentArgData* parent;
		std::string name{};
		pugi::xml_attribute valueAttribute{};
		std::unordered_map<size_t, ComponentArgData> children{};
	};

	// TODO: This needs to be updated to a tree structure!!!
	//  OR: how about we just pass the freaking node...
	struct ComponentData
	{
	private:
		using arg_list_type = std::vector<std::pair<size_t, pugi::xml_attribute>>;
	
	public:
		eComponentType type;
		std::unordered_map<size_t, pugi::xml_attribute> args;
		std::unordered_map<size_t, arg_list_type> argLists;
		ComponentArgData argsRoot{ eComponentArgType::Root, nullptr };
	};

	struct EntityData
	{
		std::string name;
		std::vector<ComponentData> components;
		std::string parent;
	};

	struct LevelLayer
	{
		s8 drawLayer;
		std::vector<std::vector<s32>> data;
	};

	struct LevelData
	{
		std::string name;
		std::vector<LevelLayer> levelLayers;
		std::vector<std::vector<s32>> collisionLayer;
	};

	class AssetManager : NonMovable
	{
	public:
		AssetManager();
		~AssetManager() = default;

	public:
		bool loadTexture(const std::string& path);
		bool loadGameData(const std::string& path);
		bool loadLevelData(const std::string& pathPrefix);

		observer_ptr<Texture2D> getTexture(const std::string& textureName);
		const std::vector<EntityData>& getGameData() { return m_gameData; }
		observer_ptr<LevelData> getLevelData(const std::string& levelName);

	private:
		void processComponentArgDataNode(const pugi::xml_node& node, ComponentArgData& data);
		std::vector<std::vector<s32>> parseLevelLayerData(const std::string& path);

	private:
		std::unordered_map<size_t, u32> m_textureHandles;
		std::vector<Texture2D> m_loadedTextures;

		pugi::xml_document m_gameDataSrc;
		std::vector<EntityData> m_gameData;

		std::unordered_map<size_t, u32> m_levelDataHandles;
		std::vector<LevelData> m_levelData;
	};
}
