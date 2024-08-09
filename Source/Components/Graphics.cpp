#include <Components/Graphics.h>


#include <AssetManager.h>
#include <Entity.h>
#include <Service.h>


using namespace pd;


void Graphics::init(ObjectArgs* pArgs)
{
	ASSERT_MSG(pArgs, "Expected GraphicsArgs was null.");

	auto args = *reinterpret_cast<GraphicsArgs*>(pArgs);

	auto& assets = ServiceGetter::get<AssetManager>();
	auto* texturePtr = assets.getTexture(args.textureName);

	ASSERT_MSG(texturePtr, "Unable to get texture `%s`", args.textureName.c_str());

	m_pTexture = texturePtr;

	m_size = args.size;
	m_offset = args.offset;
	m_pivot = args.pivot;
	m_rotation = args.rotation;
	m_drawLayer = args.drawLayer;
	m_animated = args.animated;
	m_frames = args.animationFrames;

	m_sourceRect = {
		args.offset.x,
		args.offset.y,
		args.size.x,
		args.size.y,
	};
}

const Rectangle Graphics::dstRect(const Vector2& position, const Vector2& scale) const
{
	const auto& size = Vector2Multiply(m_size, scale);
	return {
		position.x,
		position.y,
		size.x,
		size.y,
	};
}

// Defines the point of reference of texture for rotation and scaling

const Vector2 pd::Graphics::origin(const Vector2& scale)
{
	const auto& size = Vector2Multiply(m_size, scale);
	return Vector2Multiply(size, m_pivot);
}
