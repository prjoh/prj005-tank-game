#pragma once


#include <string>

#include <AssetManager.h>
#include <Component.h>
#include <Service.h>


namespace pd
{
	struct ProjectileArgs : public ComponentArgs
	{
		f32 lifetimeSeconds;
		Vector2 velocity;
		f32 damage;
		f32 hitRadius;
		// TODO: Owner entity
	};

	class Projectile : public PooledComponent<Projectile>
	{
		using base_type = PooledComponent<Projectile>;
		using pool_type = ObjectPool<Projectile>;

		friend class pool_type;
		friend class ProjectileSystem;

	public:
		Projectile() = default;
		~Projectile() = default;

	private:
		void onConstruction() override {}		  // Called when object is allocated
		void init(ObjectArgs* pArgs) override     // Called when object is created by pool
		{
			ASSERT_MSG(pArgs, "Expected ProjectileSpawnerArgs was null.");

			auto args = *reinterpret_cast<ProjectileArgs*>(pArgs);

			m_lifetimeSeconds = args.lifetimeSeconds;
			m_velocity = args.velocity;
			m_damage = args.damage;
			m_hitRadius = args.hitRadius;
		}

	private:
		f32 m_lifetimeSeconds{};
		Vector2 m_velocity{};
		f32 m_damage{};
		f32 m_hitRadius{};
	};

	struct ProjectileSpawnerArgs : public ComponentArgs
	{
		f32 projectileSpeed;
		f32 projectileDamage;
		f32 projectileHitRadius;
		// TODO: bounding box?
		std::string textureName = "";
	};

	class ProjectileSpawner : public PooledComponent<ProjectileSpawner>
	{
		using base_type = PooledComponent<ProjectileSpawner>;
		using pool_type = ObjectPool<ProjectileSpawner>;

		friend class pool_type;
		friend class ProjectileSystem;

	public:
		ProjectileSpawner() = default;
		~ProjectileSpawner() = default;

	private:
		void onConstruction() override {}		  // Called when object is allocated
		void init(ObjectArgs* pArgs) override     // Called when object is created by pool
		{
			ASSERT_MSG(pArgs, "Expected ProjectileSpawnerArgs was null.");

			auto args = *reinterpret_cast<ProjectileSpawnerArgs*>(pArgs);

			m_projectileSpeed = args.projectileSpeed;
			m_projectileDamage = args.projectileDamage;
			m_projectileHitRadius = args.projectileHitRadius;

			if (args.textureName.size() > 0)
			{
				auto& assets = ServiceGetter::get<AssetManager>();
				auto* texturePtr = assets.getTexture(args.textureName);

				ASSERT_MSG(texturePtr, "Unable to get texture `%s`", args.textureName.c_str());

				m_pTexture = texturePtr;
			}
		}

	private:
		f32 m_projectileSpeed{};
		f32 m_projectileDamage{};
		f32 m_projectileHitRadius{};
		Texture2D* m_pTexture{ nullptr };
	};
}
