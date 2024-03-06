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
		: SimpleMovement(pos, 30, 20.f)
		, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100) } {}

	void chooseBehavior(const Tilemap& map, const Player& player, const std::vector<Soul>& soulList);

	void actBehavior();

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
	float consumingSoulTime{ 1.f }; // this is the time that it takes to consume the soul. In this time the Devourer nor the soul will move.
	float consumingSoulProgress{ 0.f };

	// ChasingOldSoul
	bool newSoulPosition{ false }; // if the oldSoulPosition has just been set, this is thus true if soul broke line of sight then it will go to the new position, if it reaches this point newSoulPosition will be set to false
	Tmpl8::vec2 soulPosition{ 0.f };

	// ChasingOldPlayer
	bool newPlayerPosition{ false };
	Tmpl8::vec2 playerPosition{ 0.f }; // look at newSoulPosition comment

	float seekRadius{ 40.f }; // this is the distance to the position to considered it has been visited

	
	float collideRadius{ 10.f };


	
};

