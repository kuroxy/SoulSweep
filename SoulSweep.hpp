#pragma once
#include <vector>
#include "Camera.hpp"
#include "Player.hpp"
#include "Tilemap.hpp"
#include "Soul.hpp"
#include "SpriteSheet.hpp"
#include <format>
#include "Config.hpp"
#include "SoulConduit.hpp"
#include "ParticleSystem.hpp"

namespace Engine
{
	class InputManager;
};

class SoulSweep {
	void loadSpriteSheet(std::string_view textureFile, int spriteSize, Tmpl8::Pixel chroma)
	{
		spriteSheet = std::make_shared<Engine::SpriteSheet>(textureFile, spriteSize, spriteSize, chroma);
	}

	void loadMap(std::string_view mapName) 
	{
		std::string sprites = std::format("assets/map/{}L1.csv", mapName);
		std::string collision = std::format("assets/map/{}L2.csv", mapName);

		terrainTileMap = new Tilemap(spriteSheet, sprites, collision);
	}

	void setSoulParticleSystem()
	{
		soulParticles.spawnRate = .05f;
		soulParticles.initialVelocityDeviation = Tmpl8::vec2(5.f, 5.f);

		soulParticles.sizeRangeStart = 10.f;

		soulParticles.sizeRangeEnd = 3.f;

		soulParticles.particleLifetime = 1.f;
	}
	

public:
	SoulSweep() = default;
	~SoulSweep() { delete mainPlayer; delete terrainTileMap; }

	void Initialize()
	{
		setSoulParticleSystem();
		loadSpriteSheet(Config::TERRAIN_TEXTURE, Config::TERRAIN_SPRITE_SIZE, Config::TERRAIN_CHROMA);
		loadMap(Config::MAP_NAME);
		mainPlayer = new Player(Tmpl8::vec2(200), Config::PLAYER_SPEED, Config::PLAYER_SIZE, Config::PLAYER_SIZE);; // player is square for now

		for(int i = 0; i < 100; i++)
		{

			spawnSoul(Tmpl8::vec2(Rand(300.f) + 200.f, Rand(300) + 200.f));
		}
		//spawnSoul(mainPlayer->getPosition());
		
	}

	Soul& spawnSoul(Tmpl8::vec2 spawnPosition);


	void update(float deltaTime, Engine::InputManager im);

	void render(Engine::Camera& camera);

	
private:

	unsigned int collectedSouls{ 0 }; // unsigned doesnt really matter maybe can even go down to a char since we will never collect that many souls

	std::shared_ptr<Engine::SpriteSheet> spriteSheet{ nullptr };
	Tilemap* terrainTileMap{ nullptr };
	
	Player* mainPlayer{ nullptr };
	
	SoulConduit soulConduit{ {50,50}, {100,100} };

	std::vector<Soul> souls;
	Engine::ParticleSystemParams soulParticles;



	//debug stuff
	bool terrainDebug = false;
	bool playerDebug = true;
	bool soulsDebug = false;
	bool soulsConduitDebug = true;

	
};