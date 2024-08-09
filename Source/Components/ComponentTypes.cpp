#include <Components/ComponentTypes.h>


#include <string_view>
#include <sstream>

#include <AssetManager.h>
#include <ObjectPool.h>

#include <Components/ActorCommand.h>
#include <Components/AnimationFSM.h>
#include <Components/FilterBits.h>
#include <Components/Graphics.h>
#include <Components/Input.h>
#include <Components/PlayerCommand.h>
#include <Components/Transform.h>


namespace pd
{
	const std::unordered_map<size_t, eComponentType> g_ComponentTypes = {
		{std::hash<std::string_view>{}("Graphics"), eComponentType::Graphics},
		{std::hash<std::string_view>{}("Transform"), eComponentType::Transform},
		{std::hash<std::string_view>{}("Input"), eComponentType::Input},
		{std::hash<std::string_view>{}("Camera"), eComponentType::Camera},
		{std::hash<std::string_view>{}("ActorCommand"), eComponentType::ActorCommand},
		{std::hash<std::string_view>{}("AnimationFSM"), eComponentType::AnimationFSM},
		{std::hash<std::string_view>{}("AnimationState"), eComponentType::AnimationState},
		{std::hash<std::string_view>{}("PointerAim"), eComponentType::PointerAim},
		{std::hash<std::string_view>{}("RotateToAim"), eComponentType::RotateToAim},
		{std::hash<std::string_view>{}("CameraFollow"), eComponentType::CameraFollow},
		{std::hash<std::string_view>{}("ParticleSystem"), eComponentType::ParticleSystem},
		{std::hash<std::string_view>{}("ActorIsMoving"), eComponentType::ActorIsMoving},
		{std::hash<std::string_view>{}("ActorIsShooting"), eComponentType::ActorIsShooting},
		{std::hash<std::string_view>{}("ProjectileSpawner"), eComponentType::ProjectileSpawner},
		{std::hash<std::string_view>{}("Projectile"), eComponentType::Projectile},
		{std::hash<std::string_view>{}("PhysicsComponent"), eComponentType::PhysicsComponent},

		//{std::hash<std::string_view>{}("TileMap"), eComponentType::TileMap},

		//{std::hash<std::string_view>{}("PlayerCommand"), eComponentType::PlayerCommand},
		//{std::hash<std::string_view>{}("FilterBits"), eComponentType::FilterBits},
	};

	//const std::unordered_map<eComponentType, std::type_index> g_ComponentTypeMap = {
	//	{eComponentType::Graphics, typeid(ObjectPool<Graphics>)},
	//	{eComponentType::Transform, typeid(ObjectPool<Transform>)},
	//	{eComponentType::Input, typeid(ObjectPool<Input>)},
	//	{eComponentType::ActorCommand, typeid(ObjectPool<ActorCommand>)},
	//	{eComponentType::PlayerCommand, typeid(ObjectPool<PlayerCommand>)},
	//	{eComponentType::FilterBits, typeid(ObjectPool<FilterBits>)},
	//};

	eComponentType getComponentType(const std::string& type)
	{
		auto key = std::hash<std::string_view>{}(type);

		auto it = g_ComponentTypes.find(key);
		if (it == g_ComponentTypes.end())
			return eComponentType::Null;

		return it->second;
	}

	const pugi::xml_attribute* getComponentArg(const std::string& argName, const std::unordered_map<size_t, pugi::xml_attribute>& argsData)
	{
		auto it = argsData.find(std::hash<std::string_view>{}(argName));
		if (it == argsData.end())
			return nullptr;
		return &it->second;
	}

	const ComponentArgData* getComponentArg(const std::string& argName, const std::unordered_map<size_t, ComponentArgData>& argData)
	{
		auto it = argData.find(std::hash<std::string_view>{}(argName));
		if (it == argData.end())
			return nullptr;
		return &it->second;
	}

	const component_arg_list* getComponentArgList(const std::string& argName, const std::unordered_map<size_t, component_arg_list>& argListData)
	{
		auto it = argListData.find(std::hash<std::string_view>{}(argName));
		if (it == argListData.end())
			return nullptr;
		return &it->second;
	}

	Vector2 parseVector2Arg(const std::string& value)
	{
		std::stringstream ss(value);
		std::string token;
		std::vector<f32> values;

		while (std::getline(ss, token, ',')) {
			try {
				f32 value = std::stof(token);
				values.push_back(value);
			}
			catch (const std::exception& e) {
				auto error = e.what();
				ASSERT_MSG(false, "Error parsing input: %s", error);
			}
		}

		ASSERT_MSG(values.size() == 2, "Input does not contain two values.");

		return Vector2{ values[0], values[1] };
	}

	Vector3 parseVector3Arg(const std::string& value)
	{
		std::stringstream ss(value);
		std::string token;
		std::vector<f32> values;

		while (std::getline(ss, token, ',')) {
			try {
				f32 value = std::stof(token);
				values.push_back(value);
			}
			catch (const std::exception& e) {
				auto error = e.what();
				ASSERT_MSG(false, "Error parsing input: %s", error);
			}
		}

		ASSERT_MSG(values.size() == 3, "Input does not contain three values.");

		return Vector3{ values[0], values[1], values[2]};
	}

	//bool parseFSMTransitionData(const std::string& value, Transition& outData)
	//{
	//	std::istringstream ss(value);
	//	std::string token;

	//	try {
	//		// Parse the first three fields
	//		std::getline(ss, token, ',');
	//		outData.state = token;
	//		std::getline(ss, token, ',');
	//		outData.parameter = token;
	//		std::getline(ss, token, ',');
	//		outData.condition = g_TransitionConditions.at(std::hash<std::string_view>{}(token));

	//		// Parse the fourth field as a float
	//		std::getline(ss, token, ',');
	//		outData.threshold = std::stof(token);
	//	}
	//	catch (const std::exception& e) {
	//		auto error = e.what();
	//		ASSERT_MSG(false, "Error parsing input: %s", error);
	//	}

	//	return true;
	//}

	bool parseFSMConditionData(const std::string& value, Condition& outData)
	{
		std::istringstream ss(value);
		std::string token;

		try {
			std::getline(ss, token, ',');
			outData.parameter = token;
			std::getline(ss, token, ',');
			outData.type = g_ConditionTypes.at(std::hash<std::string_view>{}(token));
			std::getline(ss, token, ',');
			outData.threshold = std::stof(token);
		}
		catch (const std::exception& e) {
			auto error = e.what();
			ASSERT_MSG(false, "Error parsing input: %s", error);
		}

		return true;
	}

	eBodyType parseBodyTypeArg(const std::string& value)
	{
		return g_BodyTypes.at(std::hash<std::string_view>{}(value));
	}

	eColliderType parseColliderTypeArg(const std::string& value)
	{
		return g_ColliderTypes.at(std::hash<std::string_view>{}(value));
	}
}
