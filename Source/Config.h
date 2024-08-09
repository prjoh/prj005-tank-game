#pragma once


#define DEBUG_BUILD 1

#if (DEBUG_BUILD)
#define ASSERT_ENABLED 1
#define LOG_ENABLED	   1
#else
#define ASSERT_ENABLED 0
#define LOG_ENABLED	   0
#endif

#define PHYSICS_TIMESTEP 1.0f / 60.0f

#define ENTITY_POOL_SIZE				  (1 << 16)
#define GRAPHICS_POOL_SIZE				  (1 << 13)
#define TRANSFORM_POOL_SIZE				  (1 << 13)
#define ACTOR_COMMAND_POOL_SIZE			  (1 << 6)
#define ACTOR_MOVING_POOL_SIZE			  (1 << 6)
#define ACTOR_SHOOTING_POOL_SIZE		  (1 << 6)
#define ANIMATION_FSM_POOL_SIZE			  (1 << 7)
#define ANIMATION_STATE_POOL_SIZE		  (1 << 7)
#define ROTATE_TO_AIM_POOL_SIZE			  (1 << 7)
#define TILE_MAP_POOL_SIZE				  (1 << 5)
#define PARTICLE_SYSTEM_POOL_SIZE		  (1 << 13)
#define PROJECTILE_POOL_SIZE			  (1 << 8)
#define PROJECTILE_SPAWNER_POOL_SIZE	  (1 << 6)
#define STA_PHYSICS_COMPONENT_POOL_SIZE   (1 << 12)
#define KIN_PHYSICS_COMPONENT_POOL_SIZE   (1 << 10)
#define DYN_PHYSICS_COMPONENT_POOL_SIZE   (1 << 10)

#define MAX_PARTICLE_POOL_SIZE	      (1 << 14)

#define MAX_PROJECTILE_LIFETIME       30.0f
//#define MAX_PARTICLE_POOL_SIZE	   5000000

//#define PHYSICS_GRAVITY_X  0.0f
//#define PHYSICS_GRAVITY_Y  0.0f
//#define PHYSICS_TIMESTEP   1.0f / 60.0f

//#define PLAYER_COMMAND_POOL_SIZE   1
//#define FILTER_BITS_POOL_SIZE      100