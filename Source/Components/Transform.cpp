#include <Components/Transform.h>

#include <Entity.h>


void pd::Transform::init(ObjectArgs* pArgs)
{
	ASSERT_MSG(pArgs, "Expected TransformArgs was null.");

	auto args = *reinterpret_cast<TransformArgs*>(pArgs);

	m_position = args.position;
	m_rotation = args.rotation * (PI / 180.0f);
	m_scale = args.scale;

	updateWorldMatrix();
}

Vector2 pd::Transform::position() const
{
	return { m_worldMatrix.m6, m_worldMatrix.m7 };
}

void pd::Transform::position(const Vector2& position, bool updateWorldMat)
{
	Vector2 localPosition = position;

	if (m_parent != nullptr)
		localPosition = Vector2Subtract(localPosition, m_parent->position());

	this->localPosition(localPosition, updateWorldMat);
}

f32 pd::Transform::rotation() const
{
	return atan2(m_worldMatrix.m1, m_worldMatrix.m4) * (180.0f / PI);
}

void pd::Transform::rotation(const f32& angle, bool updateWorldMat)
{
	Mat3 localRotation = pd::rotation(angle * (PI / 180.0f));
	if (m_parent != nullptr)
	{
		Mat3 parentRotation = pd::inverse(pd::rotation(m_parent->rotation() * (PI / 180.0f)));
		localRotation = matrixMultiply(localRotation, parentRotation);
	}

	m_rotation = atan2(localRotation.m1, localRotation.m4);

	if (updateWorldMat)
		updateWorldMatrix();
}

// TODO: Test this!!!
Vector2 pd::Transform::scale() const
{
	return Vector2{
		sqrtf(m_worldMatrix.m0 * m_worldMatrix.m0 + m_worldMatrix.m3 * m_worldMatrix.m3),
		sqrtf(m_worldMatrix.m1 * m_worldMatrix.m1 + m_worldMatrix.m4 * m_worldMatrix.m4),
	};
}

// TODO: Test this!!!
void pd::Transform::scale(const Vector2& scale, bool updateWorldMat)
{
	Vector2 localScale = scale;

	if (m_parent != nullptr)
	{
		Vector2 parentScale = m_parent->scale();
		localScale.x /= parentScale.x;
		localScale.y /= parentScale.y;
	}

	this->localScale(localScale, updateWorldMat);
}

void pd::Transform::localPosition(const Vector2& position, bool updateWorldMat)
{ 
	m_position = position;
	if (updateWorldMat)
		updateWorldMatrix();
}

void pd::Transform::localRotation(const f32& angle, bool updateWorldMat)
{
	m_rotation = angle * (PI / 180.0f);  // Transform degrees to radians
	if (updateWorldMat)
		updateWorldMatrix();
}

void pd::Transform::localScale(const Vector2& scale, bool updateWorldMat)
{
	m_scale = scale;
	if (updateWorldMat)
		updateWorldMatrix();
}

Vector2 pd::Transform::front() const
{
	return { cosf(m_rotation), sinf(m_rotation) };
}

pd::Mat3 pd::Transform::localMatrix() const
{
	Mat3 t = pd::translation(m_position);
	Mat3 r = pd::rotation(m_rotation);
	Mat3 s = pd::scaling(m_scale);

	Mat3 result = matrixMultiply(t, r);
	result = matrixMultiply(result, s);

	return result;
}

void pd::Transform::onEntityRelationUpdate()
{
	auto previousParent = m_parent;

	if (previousParent != nullptr) {
		previousParent->updateChildReferences();
	}

	m_parent = m_owner->parent()->getComponentInParent<Transform>();

	if (m_parent)
		m_parent->addChildReference(this);

	updateWorldMatrix();
}

void pd::Transform::updateChildReferences()
{
	m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
		[&](Transform* child) { return child->m_parent != this; }), m_children.end());
}

void pd::Transform::addChildReference(Transform* child)
{
	m_children.push_back(child);
}

void pd::Transform::updateWorldMatrix()
{
	if (m_parent == nullptr) {
		m_worldMatrix = localMatrix();
	}
	else {
		m_worldMatrix = matrixMultiply(m_parent->worldMatrix(), localMatrix());
	}

	for (const auto& child : m_children) {
		child->updateWorldMatrix();
	}
}

void pd::Transform::translate(const Vector2& translation, const eSpace relativeTo)
{
	if (relativeTo == eSpace::Self)
	{
		localPosition(Vector2Add(localPosition(), translation));
	}
	else if (relativeTo == eSpace::World)
	{
		position(Vector2Add(position(), translation));
	}
}

void pd::Transform::translate(f32 x, f32 y, const eSpace relativeTo)
{
	translate({ x, y }, relativeTo);
}

// TODO: Test this!!!
void pd::Transform::translate(const Vector2& translation, const Transform& relativeTo)
{
	// TODO: Implement
}

void pd::Transform::translate(f32 x, f32 y, const Transform& relativeTo)
{
	translate({ x, y }, relativeTo);
}

void pd::Transform::rotate(f32 angle, eSpace relativeTo)
{
	if (relativeTo == eSpace::Self)
	{
		localRotation(localRotation() + angle);
	}
	else if (relativeTo == eSpace::World)
	{
		rotation(rotation() + angle);
	}
}

void pd::Transform::rotateAround(const Vector2& point, f32 angle)
{
	// Step 1: Calculate the translation to the origin of rotation
	Vector2 translate = Vector2Subtract(point, position());

	// Step 2: Perform the rotation around the origin
	f32 radians = angle * PI / 180.0f;
	Mat3 rotation = pd::rotation(radians);
	Vector3 rotatedTranslation = pd::vectorTransform({ translate.x, translate.y, 0.0f }, rotation);

	// Step 3: Calculate the translation back to the original position
	// TODO: This calculation will put accumulated floating point error into effect. How to counteract?
	position(Vector2Subtract(point, { rotatedTranslation.x, rotatedTranslation.y }), false);

	// Update the rotation angle
	this->rotate(angle);

	// Update the world matrix (assuming you have a function to update the matrix)
	updateWorldMatrix();
}
