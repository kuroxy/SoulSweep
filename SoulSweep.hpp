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
#include "Radar.hpp"

namespace Engine
{
	class InputManager;
};

class SoulSweep {
public:
	SoulSweep() = delete;
	SoulSweep(std::string_view levelPath)
	{
		// Loading Assets
		terrainSpriteSheet = std::make_shared<Engine::SpriteSheet>("assets/Textures/terrainSheet.png", 32, 32, 0xff00ff);
		playerSpriteSheet = std::make_shared<Engine::SpriteSheet>("assets/Textures/playerSheet.png", 32, 32, 0xff00ff);
		soulConduitSpriteSheet = std::make_shared<Engine::SpriteSheet>("assets/Textures/SoulConduit.png", 94, 72, 0xff00ff);
		graveStoneSpriteSheet = std::make_shared<Engine::SpriteSheet>("assets/Textures/GraveStones.png", 30, 41, 0xff00ff);

		level = std::make_unique<Level>(terrainSpriteSheet, soulConduitSpriteSheet, graveStoneSpriteSheet, "assets/Maps/level1.json");

		mainPlayer = std::make_unique<Player>(*playerSpriteSheet.get(), Tmpl8::vec2(200.f), Config::PLAYER_SPEED, Config::PLAYER_SIZE, Config::PLAYER_SIZE);

		spawnRandomDevourer();
	}

	Soul& spawnSoul(const Tmpl8::vec2& spawnPosition, const Tmpl8::vec2& initialVelocity);
	Devourer& spawnDevourer(const Tmpl8::vec2& spawnPosition);

	// spawns them based on the position of the player but a certain distance from it
	Soul& spawnRandomSoul();
	Devourer& spawnRandomDevourer();



	void update(float deltaTime, Engine::InputManager im, Engine::Camera& camera);

	void render(Engine::Camera& camera);
	
	bool victoryState() const { return victoryTimer <= 0.f; }
	bool deathState() const { return deathTimer <= 0.f; }

private:

	int collectSoulsGoal{ 10 };
	ResourceBar collectedSoulsBar{ std::make_shared<Engine::Texture>("assets/Textures/Bar.png", 0xff00ff), 15, 4, 635, 27, (float)collectSoulsGoal };
	int collectedSouls{ 0 }; 

	
	std::shared_ptr<Engine::SpriteSheet> terrainSpriteSheet{ nullptr };
	std::shared_ptr<Engine::SpriteSheet> playerSpriteSheet{ nullptr };

	std::shared_ptr<Engine::SpriteSheet> soulConduitSpriteSheet{ nullptr };
	std::shared_ptr<Engine::SpriteSheet> graveStoneSpriteSheet{ nullptr };
	

	std::unique_ptr<Level> level{ nullptr };

	std::unique_ptr<Player> mainPlayer{ nullptr };
	Radar playerRadar = Radar(20.f, Tmpl8::PI*.7f);


	std::vector<Soul> souls;
	std::vector<Devourer> devourers;

	// -- game state -- 
	// when you have died or won there is a delay between changes in game state
	float victoryTimer = 2.f;
	float deathTimer = 2.f;



	//debug stuff
	bool fogOfWarDisabled = false;


	bool terrainDebug = false;
	bool playerDebug = false;
	bool playerRadarDebug = false;
	bool soulsDebug = false;
	bool devourerDebug = false;
	bool soulsConduitDebug = false;

	
};