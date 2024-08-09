#pragma once


#include <Component.h>
#include <MathExtension.h>


namespace pd
{
	enum class eSpace : char {
		Self = 1 << 0,
		World = 1 << 1,
	};

	struct TransformArgs : public ComponentArgs
	{
		Vector2 position = { 0.0f, 0.0f };
		f32 rotation = { 0.0f };
		Vector2 scale = { 1.0f, 1.0f };
	};

	class Transform : public PooledComponent<Transform>
	{
		using kinematics_pool_type = ObjectPool<Transform>;
		using pooled_base_type = PooledComponent<Transform>;

		friend class kinematics_pool_type;

		friend class PlayerController;
		friend class RenderSystem;

	public:
		Transform() = default;
		~Transform() = default;

	private:
		void onConstruction() override {}		// Called when object is allocated
		void init(ObjectArgs* pArgs) override;	// Called when object is created by pool

	public:
		//void destroy() override {};

		Vector2 position() const;
		void position(const Vector2& position, bool updateWorldMat = true);
		f32 rotation() const;
		void rotation(const f32& angle, bool updateWorldMat = true);
		Vector2 scale() const;
		void scale(const Vector2& scale, bool updateWorldMat = true);

		Vector2 localPosition() const { return m_position; }
		void localPosition(const Vector2& position, bool updateWorldMat = true);
		f32 localRotation() const { return m_rotation * (180.0f / PI); }
		void localRotation(const f32& angle, bool updateWorldMat = true);
		Vector2 localScale() const { return m_scale; }
		void localScale(const Vector2& scale, bool updateWorldMat = true);

		Vector2 front() const;

		void translate(const Vector2& translation, const eSpace relativeTo = eSpace::Self);
		void translate(f32 x, f32 y, const eSpace relativeTo = eSpace::Self);
		void translate(const Vector2& translation, const Transform& relativeTo);
		void translate(f32 x, f32 y, const Transform& relativeTo);

		void rotate(f32 angle, eSpace relativeTo = eSpace::Self);
		void rotateAround(const Vector2& point, f32 angle);

		Mat3 localMatrix() const;
		Mat3 worldMatrix() const { return m_worldMatrix; }

		// TODO: Implement 
		/*
		 * InverseTransformDirection(Vector3 direction);          Transforms a direction from world space to local space. The opposite of Transform.TransformDirection.
		 * InverseTransformDirection(float x, float y, float z);
		 * InverseTransformPoint(Vector3 position);               Transforms position from world space to local space.
		 * InverseTransformPoint(float x, float y, float z);
		 * InverseTransformVector(Vector3 vector);                Transforms a vector from world space to local space. The opposite of Transform.TransformVector.
		 * InverseTransformVector(float x, float y, float z);
		 *
		 * TransformDirection  Transforms direction from local space to world space.
		 * TransformPoint      Transforms position from local space to world space.
		 * TransformVector     Transforms vector from local space to world space.
		 *
		 */

	private:
		void onEntityRelationUpdate() override;

		void updateChildReferences();
		void addChildReference(Transform* child);

		void updateWorldMatrix();

	private:
		Vector2 m_position{};  // Local space position
		f32 m_rotation{};	   // Local space rotation (radians)
		Vector2 m_scale{};	   // Local space scale

		Mat3 m_worldMatrix{};  // contains translation, orienatation and scaling transforms in world space

		Transform* m_parent{ nullptr };
		std::vector<Transform*> m_children{};  // TODO: Set a fixed size in constructor?
	};
}
