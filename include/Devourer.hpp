#pragma once

#include <vector>
#include <memory>
#include <string_view>

#include "template.h"
#include "SimpleMovement.hpp"
#include "ParticleSystem.hpp"


// Forward declaration
class Player;
class Soul;
class Level;


class Devourer : public SimpleMovement
{
public:
	Devourer() : SimpleMovement() {}

	Devourer(const Tmpl8::vec2& pos, const Engine::BaseParticleSystemParams& particleParams) 
		: SimpleMovement(pos, 50.f, 100.f)
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}

	Devourer(const Tmpl8::vec2& pos, 
		float maxSpeed, 
		float maxForce, 
		float consumingTime, 
		float visitRadius,
		float maxPlayerDist,
		float collideSize,
		const Engine::BaseParticleSystemParams& particleParams)
		: SimpleMovement(pos, maxSpeed, maxForce)
		, consumingSoulTime{ consumingTime }
		, seekRadius { visitRadius }
		, maxPlayerDistance{ maxPlayerDist }
		, collideRadius{ collideSize }
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}


	float getCollideRadius() const { return collideRadius; }

	bool collidesWithSoul(const Soul& soul) const;

	// Sets behavoir state based on environment
	void chooseBehavior(const Level& level, const Player& player, std::vector<Soul>& soulList);
	
	void actBehavior(float deltaTime); // Acts based on the behaviour state

	void update(float deltaTime);

	void draw(Engine::Camera& camera);
	void drawDebug(Engine::Camera& camera);


private:
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
	float consumingSoulTime{ 2.f }; // this is the time that it takes to consume the soul. In this time the Devourer nor the soul will move.
	float consumingSoulProgress{ 0.f };

	// ChasingOldSoul
	bool newSoulPosition{ false }; // if the oldSoulPosition has just been set, this is thus true if soul broke line of sight then it will go to the new position, if it reaches this point newSoulPosition will be set to false
	Tmpl8::vec2 soulPosition{ 0.f };

	// ChasingOldPlayer
	bool newPlayerPosition{ false };
	Tmpl8::vec2 playerPosition{ 0.f }; // look at newSoulPosition comment


	float seekRadius{ 40.f }; // this is the distance to the position to considered it has been visited
	float maxPlayerDistance{ 400.f };


	float wanderStrength{ .5f };

	float collideRadius{ 10.f };


	// visuals
	std::unique_ptr<Engine::BaseParticleSystem> particleSystem;
};

