#pragma once
#include "template.h"
#include "SimpleMovement.hpp"
#include "ParticleSystem.hpp"
#include <vector>
#include <memory>


class Player;
class Soul;
class Tilemap;


class Devourer : public SimpleMovement
{
public:
	Devourer() : SimpleMovement() {}

	Devourer(const Tmpl8::vec2& pos, const Engine::ParticleSystemParams& particleParams) 
		: SimpleMovement(pos, 50.f, 100.f)
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}

	Devourer(const Tmpl8::vec2& pos, 
		float maxSpeed, 
		float maxForce, 
		float consumingTime, 
		float visitRadius,
		float maxPlayerDist,
		float collideSize,
		const Engine::ParticleSystemParams& particleParams)
		: SimpleMovement(pos, maxSpeed, maxForce)
		, consumingSoulTime{ consumingTime }
		, seekRadius { visitRadius }
		, maxPlayerDistance{ maxPlayerDist }
		, collideRadius{ collideSize }
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}


	void chooseBehavior(const Tilemap& map, const Player& player, std::vector<Soul>& soulList);

	void actBehavior(float deltaTime);

	bool collideWithSoul(const Soul& soul) const;

	void update(float deltaTime);

	void draw(Engine::Camera& camera, bool debug);

	

private:
	// visuals
	std::unique_ptr<Engine::BaseParticleSystem> particleSystem;


	enum class BehaviorState {
		ConsumingSoul,
		ChasingSoul,
		ChasingPlayer,
		ChasingOldSoul,
		ChasingOldPlayer,
		Wandering
	};

	BehaviorState currentState = BehaviorState::Wandering;


	// ConsumingSoul info
	Soul* consumingSoulPtr{ nullptr };
	const float consumingSoulTime{ 2.f }; // this is the time that it takes to consume the soul. In this time the Devourer nor the soul will move.
	float consumingSoulProgress{ 0.f };

	// ChasingOldSoul
	bool newSoulPosition{ false }; // if the oldSoulPosition has just been set, this is thus true if soul broke line of sight then it will go to the new position, if it reaches this point newSoulPosition will be set to false
	Tmpl8::vec2 soulPosition{ 0.f };

	// ChasingOldPlayer
	bool newPlayerPosition{ false };
	Tmpl8::vec2 playerPosition{ 0.f }; // look at newSoulPosition comment

	const float seekRadius{ 40.f }; // this is the distance to the position to considered it has been visited

	const float maxPlayerDistance{ 400.f };

	
	const float collideRadius{ 10.f };


	std::vector<std::string_view> statesString = {
		"ConsumingSoul",
		"ChasingSoul",
		"ChasingPlayer",
		"ChasingOldSoul",
		"ChasingOldPlayer",
		"Wandering"
	};
};

