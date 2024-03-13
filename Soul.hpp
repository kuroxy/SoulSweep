#pragma once
#include "template.h"
#include "surface.h"
#include "Player.hpp"
#include "Tilemap.hpp"
#include "ParticleSystem.hpp"
#include "SimpleMovement.hpp"
#include "Devourer.hpp"

namespace Engine
{
	class Camera;
}


class Soul : public SimpleMovement
{
public:
	Soul() : SimpleMovement() {}

	Soul(const Tmpl8::vec2& pos, const Engine::ParticleSystemParams& particleParams)
		: SimpleMovement{pos, 50.f, 100.f }
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}

	Soul(const Tmpl8::vec2& pos,
		float maxForce,
		float vacuumSpeed,
		float fleeSpeed,
		float defaultSpeed,
		float maxNeighbourRadius,
		float minPlayerDist,
		float maxPlayerDistance,
		float wanderSpeed,
		float collisionRadius,
		const Engine::ParticleSystemParams& particleParams)
		: SimpleMovement{ pos, defaultSpeed, maxForce }
		, vacuumMaxSpeed{ vacuumSpeed }
		, fleeMaxSpeed{ fleeSpeed }
		, defaultMaxSpeed{ defaultSpeed }
		, seekRadius{ maxNeighbourRadius }
		, minPlayerDistance { minPlayerDist }
		, maxPlayerDistance { maxPlayerDistance }
		, wanderStrength { wanderSpeed }
		, collideRadius{ collisionRadius }
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}


	float getCollisionRadius() const { return collideRadius; }

	void chooseBehavior(const Tilemap& map, const Player& player, std::vector<Soul>& soulList, std::vector<Devourer>& devourerList);

	void actBehavior(float deltaTime);

	void applyVacuumForce(const Player& player);

	void update(float deltaTime, const Player& player);

	void draw(Engine::Camera& camera, bool debug);

	bool isEaten{ false }; // souls with isEaten to true will be removed from soulList
private:
	bool beingVacuumed{ false };

	enum class BehaviorState {
		Flee,
		SeekPlayer,
		AvoidNeigbours,
		Wandering
	};

	BehaviorState currentState = BehaviorState::Wandering;

	float vacuumMaxSpeed{ 100.f };
	float fleeMaxSpeed{ 40.f };
	float defaultMaxSpeed{ 30.f };

	
	float seekRadius{ 40.f }; // the distance between neighbours to go in AvoidNeigbours state

	float minPlayerDistance{ 100.f }; // will end seekplayer behaviour if inside this range
	float maxPlayerDistance{ 400.f }; // will start seekplayer behaviour if outside this range

	float wanderStrength{ .5f };

	float collideRadius{ 10.f };


	Tmpl8::vec2 fleePosition{ 0.f };
	Tmpl8::vec2 neigboursPosition{ 0.f };
	Tmpl8::vec2 playerPosition{ 0.f };

	// visuals
	std::unique_ptr<Engine::BaseParticleSystem> particleSystem;

};

