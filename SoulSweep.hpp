#pragma once
#include <vector>
#include "Camera.hpp"
#include "Player.hpp"
#include "Soul.hpp"
#include "SpriteSheet.hpp"
#include <format>
#include "Config.hpp"
#include "SoulConduit.hpp"
#include "ParticleSystem.hpp"
#include "Devourer.hpp"
#include "Level.hpp"
#include "ResourceBar.hpp"

namespace Engine
{
	class InputManager;
};

class SoulSweep {
	void setSoulParticleSystem()
	{
		soulParticles.spawnRate = .05f;
		soulParticles.initialVelocityDeviation = Tmpl8::vec2(5.f, 5.f);

		soulParticles.sizeRangeStart = 10.5f;

		soulParticles.sizeRangeEnd = 2.5f;
		
		soulParticles.colorRangeEnd = Tmpl8::vec3(200.f);
		soulParticles.particleLifetime = 1.f;
	}

	void setDevourerParticleSystem()
	{
		
		devourerParticles.spawnRate = .05f;
		devourerParticles.initialVelocityDeviation = Tmpl8::vec2(5.f, 5.f);

		devourerParticles.sizeRangeStart = 10.5f;

		devourerParticles.sizeRangeEnd = 2.5f;

		devourerParticles.colorRangeStart = Tmpl8::vec3(40.f, 15.f, 15.f);
		devourerParticles.colorRangeEnd = Tmpl8::vec3(20.f, 5.f , 5.f);
		devourerParticles.particleLifetime = 1.f;
	}


public:
	SoulSweep() = default;

	void Initialize()
	{
		setSoulParticleSystem();
		setDevourerParticleSystem();


		// currently loading textures here, maybe later we can move it back to config but this seems to work for now..
		terrainSpriteSheet = std::make_shared<Engine::SpriteSheet>("assets/Textures/terrainSheet.png", 32, 32, 0xff00ff); 
		playerSpriteSheet = std::make_shared<Engine::SpriteSheet>("assets/Textures/playerSheet.png", 32, 32, 0xff00ff);

		soulConduitSpriteSheet = std::make_shared<Engine::SpriteSheet>("assets/Textures/SoulConduit.png", 94, 72, 0xff00ff);

		
		mainPlayer = std::make_unique<Player>(*playerSpriteSheet.get(), Tmpl8::vec2(200.f), Config::PLAYER_SPEED, Config::PLAYER_SIZE, Config::PLAYER_SIZE);

		level = std::make_unique<Level>(terrainSpriteSheet, soulConduitSpriteSheet, "assets/Maps/level1.json");


		spawnRandomDevourer(500.f);
	}

	Soul& spawnSoul(const Tmpl8::vec2& spawnPosition, const Tmpl8::vec2& initialVelocity);
	Devourer& spawnDevourer(const Tmpl8::vec2& spawnPosition);

	// spawns them based on the position of the player but a certain distance from it
	Soul& spawnRandomSoul(float distance);
	Devourer& spawnRandomDevourer(float distance);


	void update(float deltaTime, Engine::InputManager im, Engine::Camera& camera);

	void render(Engine::Camera& camera);

	
private:

	int collectSoulsGoal{ 10 };
	ResourceBar collectedSoulsBar{ std::make_shared<Engine::Texture>("assets/Textures/Bar.png", 0xff00ff), 15, 4, 635, 27, (float)collectSoulsGoal };
	int collectedSouls{ 0 }; 

	
	std::shared_ptr<Engine::SpriteSheet> terrainSpriteSheet{ nullptr };
	std::shared_ptr<Engine::SpriteSheet> playerSpriteSheet{ nullptr };

	std::shared_ptr<Engine::SpriteSheet> soulConduitSpriteSheet{ nullptr };

	

	std::unique_ptr<Level> level{ nullptr };

	std::unique_ptr<Player> mainPlayer{ nullptr };

	std::vector<Soul> souls;
	std::vector<Devourer> devourers;
	Engine::ParticleSystemParams soulParticles;
	Engine::ParticleSystemParams devourerParticles;



	//debug stuff
	bool fogOfWarDisabled = false;


	bool terrainDebug = false;
	bool playerDebug = false;
	bool soulsDebug = false;
	bool devourerDebug = false;
	bool soulsConduitDebug = false;

	
};