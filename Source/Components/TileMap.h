//#pragma once
//
//
//#include <string>
//
//#include <AssetManager.h>
//#include <Component.h>
//
//
//// TODO: DELETE THIS? Why don't we just use Graphics?
//namespace pd
//{
//	struct TileMapArgs : public ComponentArgs
//	{
//		std::string levelName;
//		std::string textureName;
//	};
//
//	class TileMap : public PooledComponent<TileMap>
//	{
//		using pool_type = ObjectPool<TileMap>;
//		using base_type = PooledComponent<TileMap>;
//
//		friend class pool_type;
//
//	public:
//		TileMap() = default;
//		~TileMap() = default;
//
//	private:
//		void onConstruction() override {}		 // Called when object is allocated
//		void init(ObjectArgs* pArgs) override;	 // Called when object is created by pool
//
//	private:
//		observer_ptr<LevelData> m_pLevelData{ nullptr };
//		observer_ptr<Texture2D> m_pTexture{ nullptr };
//	};
//}
