#include <GameData.h>

// TODO: Why???
#define PUGIXML_HEADER_ONLY
#include <pugixml.hpp>


namespace
{
    using namespace pd;

    bool loadPhysicsComponentArgs(const ComponentArgData& data, PhysicsComponentArgs& args)
    {
        if (auto arg = getComponentArg("bodyType", data.children))
            args.bodyType = parseBodyTypeArg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("colliderType", data.children))
            args.colliderType = parseColliderTypeArg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("density", data.children))
            args.density = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("friction", data.children))
            args.friction = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("restitution", data.children))
            args.restitution = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("linearDamping", data.children))
            args.linearDamping = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("angularDamping", data.children))
            args.angularDamping = arg->valueAttribute.as_float();
        return true;
    }
}

namespace pd
{
	bool loadGraphicsArgs(const ComponentArgData& data, GraphicsArgs& args)
	{
        if (auto arg = getComponentArg("textureName", data.children))
            args.textureName = arg->valueAttribute.as_string();
        if (auto arg = getComponentArg("size", data.children))
            args.size = parseVector2Arg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("offset", data.children))
            args.offset = parseVector2Arg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("pivot", data.children))
            args.pivot = parseVector2Arg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("rotation", data.children))
            args.rotation = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("drawLayer", data.children))
            args.drawLayer = arg->valueAttribute.as_int();
        if (auto arg = getComponentArg("animated", data.children))
            args.animated = arg->valueAttribute.as_bool();
        if (auto arg = getComponentArg("animationFrames", data.children))
            args.animationFrames = arg->valueAttribute.as_uint();
		return true;
	}

    bool loadActorCommandArgs(const ComponentArgData& data, ActorCommandArgs& args)
    {
        if (auto arg = getComponentArg("isPlayer", data.children))
            args.isPlayer = arg->valueAttribute.as_bool();
        if (auto arg = getComponentArg("observer", data.children))
            args.observer = arg->valueAttribute.as_bool();
        return true;
    }

    bool loadAnimationFSMArgs(const ComponentArgData& data, AnimationFSMArgs& args)
    {
        if (auto arg = getComponentArg("initialState", data.children))
            args.initialState = arg->valueAttribute.as_string();
        return true;
    }

    bool loadAnimationStateArgs(const ComponentArgData& data, AnimationStateArgs& args)
    {
        if (auto arg = getComponentArg("name", data.children))
            args.name = arg->valueAttribute.as_string();
        if (auto arg = getComponentArg("animationFrames", data.children))
            args.animationFrames = arg->valueAttribute.as_uint();
        if (auto arg = getComponentArg("framesPerSecond", data.children))
            args.framesPerSecond = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("isLooped", data.children))
            args.isLooped = arg->valueAttribute.as_bool();
        if (auto transitionList = getComponentArg("transitions", data.children))
        {
            for (const auto& [_, tuple] : transitionList->children)
            {
                Transition transition;
                if (auto stateArg = getComponentArg("state", tuple.children))
                {
                    transition.state = stateArg->valueAttribute.as_string();

                    if (auto conditionList = getComponentArg("conditions", tuple.children))
                    {
                        for (const auto& [_, conditionArg] : conditionList->children)
                        {
                            Condition condition;
                            if (parseFSMConditionData(conditionArg.valueAttribute.as_string(), condition))
                                transition.conditions.push_back(condition);
                        }
                    }
                }
                args.transitions.push_back(transition);
            }
        }
        return true;
    }

    bool loadTransformArgs(const ComponentArgData& data, TransformArgs& args)
    {
        if (auto arg = getComponentArg("position", data.children))
            args.position = parseVector2Arg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("rotation", data.children))
            args.rotation = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("scale", data.children))
            args.scale = parseVector2Arg(arg->valueAttribute.as_string());
        return true;
    }

    bool loadParticleSystemArgs(const ComponentArgData& data, ParticleSystemArgs& args)
    {
        if (auto arg = getComponentArg("drawLayer", data.children))
            args.drawLayer = arg->valueAttribute.as_int();
        if (auto arg = getComponentArg("particleLifetime", data.children))
            args.particleLifetime = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("emissionRate", data.children))
            args.emissionRate = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("initialSpeed", data.children))
            args.initialSpeed = parseVector2Arg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("initialRotation", data.children))
            args.initialRotation = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("angularSpeed", data.children))
            args.angularSpeed = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("initialScale", data.children))
            args.initialScale = parseVector2Arg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("scalarSpeed", data.children))
            args.scalarSpeed = parseVector2Arg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("inheritRotation", data.children))
            args.inheritRotation = arg->valueAttribute.as_bool();
        if (auto arg = getComponentArg("autoPlay", data.children))
            args.autoPlay = arg->valueAttribute.as_bool();
        if (auto arg = getComponentArg("worldSpaceEmission", data.children))
            args.worldSpaceEmission = arg->valueAttribute.as_bool();
        if (auto arg = getComponentArg("color", data.children))
            args.color = parseVector3Arg(arg->valueAttribute.as_string());
        if (auto arg = getComponentArg("alpha", data.children))
            args.alpha = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("alphaSpeed", data.children))
            args.alphaSpeed = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("maxParticleAmount", data.children))
            args.maxParticleAmount = arg->valueAttribute.as_uint();
        if (auto arg = getComponentArg("textureName", data.children))
            args.textureName = arg->valueAttribute.as_string();
        return true;
    }

    bool loadProjectileSpawnerArgs(const ComponentArgData& data, ProjectileSpawnerArgs& args)
    {
        if (auto arg = getComponentArg("projectileSpeed", data.children))
            args.projectileSpeed = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("projectileDamage", data.children))
            args.projectileDamage = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("projectileHitRadius", data.children))
            args.projectileHitRadius = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("textureName", data.children))
            args.textureName = arg->valueAttribute.as_string();
        return true;
    }

    bool loadBoxPhysicsComponentArgs(const ComponentArgData& data, BoxPhysicsComponentArgs& args)
    {
        loadPhysicsComponentArgs(data, args);
        if (auto arg = getComponentArg("width", data.children))
            args.width = arg->valueAttribute.as_float();
        if (auto arg = getComponentArg("height", data.children))
            args.height = arg->valueAttribute.as_float();
        return true;
    }
}
