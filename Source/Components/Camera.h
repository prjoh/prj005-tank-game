#pragma once


#include <Component.h>
#include <MathExtension.h>
#include <Singleton.h>


namespace pd
{
	struct CameraArgs : public ComponentArgs
	{
        u32 screenWidth;
        u32 screenHeight;
        Vector2 position = { 0.0f, 0.0f };
        f32 rotation = { 0.0f };
        Vector2 scale = { 1.0f, 1.0f };
	};

	class Camera : public SingletonComponent, public Singleton<Camera>
	{
		friend class Singleton<Camera>;
		friend class EntityManager;

		friend class CommandSystem;
        friend class CameraSystem;
        friend class RenderSystem;

	private:
		Camera() = default;
		~Camera() = default;

	private:
		void init(ObjectArgs* pArgs) override
        {
            ASSERT_MSG(pArgs, "Expected CameraArgs was null.");

            auto args = *reinterpret_cast<CameraArgs*>(pArgs);

            m_screenSize = {
                (f32)args.screenWidth,
                (f32)args.screenHeight
            };

            setPosition(args.position);
            m_rotation = args.rotation * (PI / 180.0f);
            m_scale = args.scale;
        }

    public:
        void setPosition(Vector2 worldPosition)
        {
            m_position = {
                -(f32)m_screenSize.x * 0.5f + worldPosition.x,
                -(f32)m_screenSize.y * 0.5f + worldPosition.y,
            };
        }

        Vector2 position()
        {
            return Vector2Add(m_position, Vector2Scale(m_screenSize, 0.5f));
        }

        Mat3 viewMatrix() const {
            Mat3 translationMat = pd::translation(Vector2Scale(m_position, -1.0f));
            Mat3 rotationMat = pd::rotation(m_rotation);
            Mat3 scalingMat = pd::scaling(m_scale);

            return pd::matrixMultiply(pd::matrixMultiply(translationMat, rotationMat), scalingMat);
        }

        Vector2 screenToWorld(f32 screenX, f32 screenY) const {
            Mat3 viewMatrixInverse = pd::inverse(viewMatrix());
            Vector3 screenCoords{ screenX, screenY, 1.0f };
            Vector3 worldCoords = pd::vectorTransform(screenCoords, viewMatrixInverse);
            return Vector2{ worldCoords.x, worldCoords.y };
        }

	private:
        Vector2 m_position{};
        f32 m_rotation{};
        Vector2 m_scale{};

        Vector2 m_screenSize{};
	};
}
