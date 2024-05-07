#pragma once

#include "template.h"
#include "aabb.hpp"
#include "SpriteSheet.hpp"
#include "Animator.hpp"
#include "ParticleSystem.hpp"
#include "Config.hpp"

// Forward declaration
namespace Engine
{
	class Camera;
	class InputManager;
}

class Level;



class Player
{
public:
	Player(std::shared_ptr<Engine::SpriteSheet> playerSheet, std::shared_ptr<Engine::SpriteSheet> soulCarrySprites, const Tmpl8::vec2& pos, float width, float height, float playerSpeed, float rechargeSpeed, float dashCost, float dashDuration, float dashSpeed)
		: sprites { playerSheet }
		, anim { sprites }
		, soulCarrySprites { soulCarrySprites }
		, position{ pos }
		, playerCollider{ {0.f,0.f},{width,height} }
		, playerSpeed{ playerSpeed }
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
	const Tmpl8::vec2& getPosition() const { return position; }
	void setPosition(const Tmpl8::vec2& pos) { position = pos; updateAABB(); }

	bool canCollectSoul() const { return currentSouls < maxSouls; }
	void collectSoul() { currentSouls = Tmpl8::Min<int>(currentSouls + 1, maxSouls); }
	void removeSoul() { currentSouls = Tmpl8::Max<int>(currentSouls - 1, 0); }
	float getCollectRadius() const { return collectRadius; }

	const Engine::AABB& getAABB() const { return playerCollider; }
	const Tmpl8::vec2& getVacuumDirection() const { return vacuumDirection; }
	bool isVacuumEnabled() const { return vacuumEnabled; }

	bool shouldDropSoul() const { return currentSouls > 0 && dropSoul; }

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
	std::shared_ptr<Engine::SpriteSheet> sprites;
	Engine::Animator anim;
	bool flipCharacter = false;

	std::shared_ptr<Engine::SpriteSheet> soulCarrySprites;


	// position and collider data

	float playerSpeed{  100.f };		// px/s
	Tmpl8::vec2 moveDirection{ 0.f };	

	Tmpl8::vec2 velocity{ 0.f };
	Tmpl8::vec2 position{ 0.f };


	Engine::AABB playerCollider;
	void updateAABB(); // Updates the aabb based on position




	// dash Data

	float dashResource = 1.f;
	float dashRechargeSpeed = .2f;

	float dashCost = .5f;

	float dashSpeed = 200.f;
	float dashDuration = .3f;

	float currentDashDuration = 0.f;
	Tmpl8::vec2 dashDirection{ 0.f };
	
	

	// Vacuum

	bool vacuumEnabled = false;
	bool dropSoul = false;
	Tmpl8::vec2 vacuumDirection{ 1,0 };
	float vacuumCone{ .86f }; // How large the cone angle is, you can calculate this angle by cos(angle) 

	float maxVacuumDistance = 150.f;
	float maxVacuumForce = 1500.f;

	// Vacuum particles
	Engine::BaseParticleSystem vacuumParticles{ Config::vacuumParticles, 50 };
	float spawnTime = .2f;
	float timeTillNextSpawn = 0.f;


	// Collecting souls
	float collectRadius{ 12.f };
	int maxSouls{ 5 };
	int currentSouls{ 0 };
	

	// for game state. 
	// dead = play the death animation. totalDead = animation has finished.

	bool dead = false;
	bool totalDead = false;
};

