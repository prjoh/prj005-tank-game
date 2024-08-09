#pragma once


#include <string>

#include <raylib.h>

#include <Component.h>


namespace pd
{
	struct GraphicsArgs : public ComponentArgs
	{
		std::string textureName;
		Vector2 size = { 0.0f, 0.0f };
		Vector2 offset = { 0.0f, 0.0f };
		Vector2 pivot = { 0.0f, 0.0f };
		f32 rotation = 0.0f;
		s8 drawLayer = 0;
		bool animated = false;
		u8 animationFrames = 0;
	};

	class Graphics : public PooledComponent<Graphics>
	{
		using graphics_pool_type = ObjectPool<Graphics>;
		using pooled_base_type = PooledComponent<Graphics>;

	friend class graphics_pool_type;

	friend class EntityManager;
	friend class GraphicsAnimator;
	friend class RenderSystem;
	friend class PlayerController;

	public:
		Graphics() = default;
		~Graphics() = default;

	private:
		void onConstruction() override {}		 // Called when object is allocated
		void init(ObjectArgs* pArgs) override;	 // Called when object is created by pool

	public:
		//void destroy() override {};
		//void update(f32 elapsedTimeS) override {};

		// Defines the part of the texture we use for drawing (Texture coordinates)
		Rectangle srcRect() { return m_sourceRect; }
		// Defines rectangle to project sourceRect into, scaling it to fit (Screen coordinates)
		const Rectangle dstRect(const Vector2& position, const Vector2& scale = { 1.0f, 1.0f }) const;
		// Defines the point of reference of texture for rotation and scaling
		const Vector2 origin(const Vector2& scale = { 1.0f, 1.0f });

	private:
		observer_ptr<Texture2D> m_pTexture{ nullptr };
		
		Vector2 m_size{};
		Vector2 m_offset{};  // TODO: Make this ID (such that top-left is ID 0 and increasing by row-major order)
		Vector2 m_pivot{};
		f32 m_rotation{};
		s8 m_drawLayer{};
		bool m_animated{};
		u8 m_frames{};
		u8 m_currentFrame{};
		Rectangle m_sourceRect{};
	};
}
