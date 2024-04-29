#pragma once
#include "template.h"
#include "aabb.hpp"
#include "SpriteSheet.hpp"
#include "Animator.hpp"
#include "ParticleSystem.hpp"
#include "Config.hpp"

//forward declaration
namespace Engine
{
	class Camera;
	class InputManager;
}




class Level;

class Player
{
public:
	Player(const Engine::SpriteSheet& playerSheet, const Engine::SpriteSheet& soulCarrySprites, const Tmpl8::vec2& pos, float width, float height, float playerSpeed, float rechargeSpeed, float dashCost, float dashDuration, float dashSpeed)
		: sprites { playerSheet }
		, anim { sprites }
		, soulCarrySprites { soulCarrySprites }
		, m_position{ pos }
		, m_width{ width }
		, m_height{ height }
		, m_playerSpeed{ playerSpeed }
		, dashRechargeSpeed{ rechargeSpeed }
		, dashCost{ dashCost}
		, dashDuration{ dashDuration}
		, dashSpeed{ dashSpeed }
	{
		updateAABB();
		
		anim.addAnimation("idle", { 0,0,0,1,1, 1 }, 1.f / 10.f, true);
		anim.addAnimation("walk", { 24, 25, 26, 27, 28, 29 ,30 ,31 }, 1.f / 10.f, true);
		anim.addAnimation("death", {56, 57, 58, 59, 60, 61, 62, 63, 7}, 1.f / 10.f, false);
	}



	const Tmpl8::vec2& getVelocity() const { return velocity; }
	const Tmpl8::vec2& getPosition() const { return m_position; }
	void setPosition(const Tmpl8::vec2& pos) { m_position = pos; updateAABB(); }

	bool canCollectSoul() const { return m_currentSouls < m_maxSouls; }
	void collectSoul() { m_currentSouls = Tmpl8::Min<int>(m_currentSouls + 1, m_maxSouls); }
	void removeSoul() { m_currentSouls = Tmpl8::Max<int>(m_currentSouls - 1, 0); }
	float getCollectRadius() const { return m_collectRadius; }

	const Engine::AABB& getAABB() const { return m_collisionBox; }
	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; }
	const Tmpl8::vec2& getVacuumDirection() const { return m_vacuumDirection; }
	bool isVacuumEnabled() const { return m_vacuumEnabled; }

	bool shouldDropSoul() const { return m_currentSouls > 0 && m_dropSoul; }

	void handleInput(const Engine::InputManager& im);

	void update(float deltaTime);
	void checkCollisions(std::vector<Engine::AABB> colliders);


	bool vacuumRange(const Tmpl8::vec2 pos) const;
	const Tmpl8::vec2 calculateVacuumForce(const Tmpl8::vec2 pos) const;

	void draw(Engine::Camera& camera);
	void drawDebug(Engine::Camera& camera);

	float getDashResource() const { return dashResource; }

	bool isDead() const { return dead; }
	void setDeath(bool val) { dead = val; }

private:
	bool checkVerticalCollisions(const Engine::AABB& collider, Engine::AABB& aabb, Tmpl8::vec2& vel) const;
	bool checkHorizontalCollisions(const Engine::AABB& collider, Engine::AABB& aabb, Tmpl8::vec2& vel) const;

	// Player Sprite
	Engine::SpriteSheet sprites;
	Engine::Animator anim;
	bool flipCharacter = false;

	Engine::SpriteSheet soulCarrySprites;

	// Vacuum particles
	Engine::BaseParticleSystem vacuumParticles{Config::vacuumParticles, 50};
	float spawnTime = .2f;
	float timeTillNextSpawn = 0.f;


	// dash Data
	float dashResource = 1.f;
	float dashRechargeSpeed = .2f;

	float dashCost = .5f;

	float dashDuration = .3f;
	float currentDashDuration = 0.f;

	Tmpl8::vec2 dashDirection{ 0.f };
	float dashSpeed = 200.f;


	// position and collider data
	Tmpl8::vec2 moveDirection{ 0.f };

	Tmpl8::vec2 velocity{ 0.f };
	Tmpl8::vec2 m_position{ 0.f };

	
	
	float m_playerSpeed;
	float m_width;
	float m_height;

	Engine::AABB m_collisionBox;
	void updateAABB();

	
	bool m_vacuumEnabled = false;
	bool m_dropSoul = false;
	Tmpl8::vec2 m_vacuumDirection{ 1,0 };
	float m_vacuumCone = .86f; // cone angle you can calculate by cos(angle) 

	// Collecting souls
	float m_collectRadius{ 12.f };
	int m_maxSouls{ 5 };
	int m_currentSouls{ 0 };

	float m_maxVacuumForce = 1500.f;
	float m_maxVacuumDistance = 150.f;


	// for game state. 
	// vdead = play the death animation. totalDead = animation has finished.

	bool dead = false;
	bool totalDead = false;
};

