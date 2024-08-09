#pragma once


#include <AssetManager.h>
#include <Entity.h>


namespace pd
{
	bool loadGraphicsArgs(const ComponentArgData& data, GraphicsArgs& args);
	bool loadActorCommandArgs(const ComponentArgData& data, ActorCommandArgs& args);
	bool loadAnimationFSMArgs(const ComponentArgData& data, AnimationFSMArgs& args);
	bool loadAnimationStateArgs(const ComponentArgData& data, AnimationStateArgs& args);
	bool loadTransformArgs(const ComponentArgData& data, TransformArgs& args);
	bool loadParticleSystemArgs(const ComponentArgData& data, ParticleSystemArgs& args);
	bool loadProjectileSpawnerArgs(const ComponentArgData& data, ProjectileSpawnerArgs& args);
	bool loadBoxPhysicsComponentArgs(const ComponentArgData& data, BoxPhysicsComponentArgs& args);
}
