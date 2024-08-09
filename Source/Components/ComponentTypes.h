#pragma once


#include <string>
#include <typeindex>
#include <unordered_map>

#include <pugixml.hpp>

#include <Component.h>
#include <Raylib.h>
#include <Types.h>

#include <Components/PhysicsComponent.h>


namespace pd { struct Condition; }
//namespace pd { struct Transition; }
namespace pd { struct ComponentArgData; }


namespace pd
{
	using component_arg_list = std::vector<std::pair<size_t, pugi::xml_attribute>>;

	enum class eComponentType : u8 {
		Graphics          = 0,
		Transform         = 1,
		Input		      = 2,
		Camera		      = 3,
		ActorCommand      = 4,
		AnimationFSM      = 5,
		AnimationState    = 6,
		PointerAim	      = 7,
		RotateToAim	      = 8,
		CameraFollow      = 9,
		ParticleSystem    = 10,
		ActorIsMoving     = 11,
		ActorIsShooting   = 12,
		ProjectileSpawner = 13,
		Projectile	      = 14,
		PhysicsComponent  = 15,
		//TileMap		   = 9,
		Null	          = 255,
		//PlayerCommand  = 4,
		//FilterBits	   = 5,
	};

	extern const std::unordered_map<size_t, eComponentType> g_ComponentTypes;
	//extern const std::unordered_map<eComponentType, std::type_index> g_ComponentTypeMap;

	eComponentType getComponentType(const std::string& type);
	

	// TODO: MOVE THIS
	const pugi::xml_attribute* getComponentArg(const std::string& argName, const std::unordered_map<size_t, pugi::xml_attribute>& argsData);
	const component_arg_list* getComponentArgList(const std::string& argName, const std::unordered_map<size_t, component_arg_list>& argListData);
	
	const ComponentArgData* getComponentArg(const std::string& argName, const std::unordered_map<size_t, ComponentArgData>& argData);

	Vector2 parseVector2Arg(const std::string& value);
	Vector3 parseVector3Arg(const std::string& value);
	//bool parseFSMTransitionData(const std::string& value, Transition& outData);
	bool parseFSMConditionData(const std::string& value, Condition& outData);
	eBodyType parseBodyTypeArg(const std::string& value);
	eColliderType parseColliderTypeArg(const std::string& value);
}
