//#include <Components/TileMap.h>
//
//
//#include <Service.h>
//
//
//void pd::TileMap::init(ObjectArgs* pArgs)
//{
//	ASSERT_MSG(pArgs, "Expected TileMapArgs was null.");
//
//	auto args = *reinterpret_cast<TileMapArgs*>(pArgs);
//
//	auto& assets = ServiceGetter::get<AssetManager>();
//
//	auto* levelDataPtr = assets.getLevelData(args.levelName);
//	auto* texturePtr = assets.getTexture(args.textureName);
//
//	ASSERT_MSG(levelDataPtr, "Unable to get level data `%s`", args.textureName.c_str());
//	ASSERT_MSG(texturePtr, "Unable to get texture `%s`", args.textureName.c_str());
//
//	m_pLevelData = levelDataPtr;
//	m_pTexture = texturePtr;
//}