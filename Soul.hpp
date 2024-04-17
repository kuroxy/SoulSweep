#pragma once
#include "template.h"
#include "surface.h"
#include "Player.hpp"
#include "ParticleSystem.hpp"
#include "SimpleMovement.hpp"
#include "Devourer.hpp"
#include "aabb.hpp"

namespace Engine
{
	class Camera;
}


class Soul : public SimpleMovement
{
public:
	Soul() : SimpleMovement() {}

	Soul(const Tmpl8::vec2& pos, const Engine::BaseParticleSystemParams& particleParams)
		: SimpleMovement{pos, 50.f, 100.f }
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}

	Soul(const Tmpl8::vec2& pos,
		float maxForce,
		float vacuumSpeed,
		float fleeSpeed,
		float defaultSpeed,
		float maxNeighbourRadius,
		const Engine::AABB& levelArea,
		float wanderPointDist,
		float collisionRadius,
		const Engine::BaseParticleSystemParams& particleParams)
		: SimpleMovement{ pos, defaultSpeed, maxForce }
		, vacuumMaxSpeed{ vacuumSpeed }
		, fleeMaxSpeed{ fleeSpeed }
		, defaultMaxSpeed{ defaultSpeed }
		, seekRadius{ maxNeighbourRadius }
		, wanderArea { levelArea }
		, wanderPointDistance { wanderPointDist }
		, collideRadius{ collisionRadius }
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}


	float getCollisionRadius() const { return collideRadius; }

	void chooseBehavior(const Level& level, const Player& player, std::vector<Soul>& soulList, std::vector<Devourer>& devourerList);

	void actBehavior(float deltaTime);

	void applyVacuumForce(const Player& player);

	void update(float deltaTime, const Player& player);

	void draw(Engine::Camera& camera);
	void drawDebug(Engine::Camera& camera);

	bool isEaten{ false }; // souls with isEaten to true will be removed from soulList
private:

	void setFlee();
	void setAvoidNeighbours();
	void setWandering();

	void generateWanderingPoint();

	bool beingVacuumed{ false };
	float collideRadius{ 10.f };


	enum class BehaviorState {
		Flee,					// Highest priority, if able to see devourer flee from it.
		AvoidNeigbours,			// To close to neightbours flee from them so they don't overlap
		Wandering				// Nothing else to do, go to a random point on the map.
	};

	BehaviorState currentState = BehaviorState::Wandering;

	float vacuumMaxSpeed{ 100.f };
	float fleeMaxSpeed{ 40.f };
	float defaultMaxSpeed{ 30.f };

	
	float seekRadius{ 40.f }; // the distance between neighbours to go in AvoidNeigbours state

	float wanderPointDistance{40.f }; // distance to point to say it has been reached

	float wanderTime{ 5.f };
	float currentWanderTime{ 0.f };

	Engine::AABB wanderArea{ 0.f, 0.f };

	Tmpl8::vec2 fleePosition{ 0.f };
	Tmpl8::vec2 neigboursPosition{ 0.f };
	Tmpl8::vec2 playerPosition{ 0.f };

	Tmpl8::vec2 wanderPosition{ 0.f };

	// visuals
	std::unique_ptr<Engine::BaseParticleSystem> particleSystem;

};

