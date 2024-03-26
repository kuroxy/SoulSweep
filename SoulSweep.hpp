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
#include "Devourer.hpp"
#include "Level.hpp"

namespace Engine
{
	class InputManager;
};

class SoulSweep {
	void loadSpriteSheet(std::string_view textureFile, int spriteSize, Tmpl8::Pixel chroma)
	{
		spriteSheet = std::make_shared<Engine::SpriteSheet>(textureFile, spriteSize, spriteSize, chroma);
	}

	void setSoulParticleSystem()
	{
		soulParticles.spawnRate = .05f;
		soulParticles.initialVelocityDeviation = Tmpl8::vec2(5.f, 5.f);

		soulParticles.sizeRangeStart = 10.f;

		soulParticles.sizeRangeEnd = 2.f;
		
		soulParticles.colorRangeEnd = Tmpl8::vec3(200.f);
		soulParticles.particleLifetime = 1.f;
	}

	void setDevourerParticleSystem()
	{
		
		devourerParticles.spawnRate = .05f;
		devourerParticles.initialVelocityDeviation = Tmpl8::vec2(5.f, 5.f);

		devourerParticles.sizeRangeStart = 10.f;

		devourerParticles.sizeRangeEnd = 2.f;

		devourerParticles.colorRangeStart = Tmpl8::vec3(40.f, 15.f, 15.f);
		devourerParticles.colorRangeEnd = Tmpl8::vec3(20.f, 5.f , 5.f);
		devourerParticles.particleLifetime = 1.f;
	}

public:
	SoulSweep() = default;
	~SoulSweep() { delete mainPlayer; }

	void Initialize()
	{
		setSoulParticleSystem();
		setDevourerParticleSystem();
		loadSpriteSheet(Config::TERRAIN_TEXTURE, Config::TERRAIN_SPRITE_SIZE, Config::TERRAIN_CHROMA);
		//loadMap(Config::MAP_NAME, Config::viewDistanceMin, Config::viewDistanceMax);
		mainPlayer = new Player(Tmpl8::vec2(200.f), Config::PLAYER_SPEED, Config::PLAYER_SIZE, Config::PLAYER_SIZE);; // player is square for now

		level = std::make_unique<Level>(spriteSheet, "assets/map/level1.json");


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

	unsigned int collectedSouls{ 0 }; 


	std::shared_ptr<Engine::SpriteSheet> spriteSheet{ nullptr };
	std::unique_ptr<Level> level{ nullptr };

	
	Player* mainPlayer{ nullptr };

	std::vector<Soul> souls;
	std::vector<Devourer> devourers;
	Engine::ParticleSystemParams soulParticles;
	Engine::ParticleSystemParams devourerParticles;



	//debug stuff
	bool fogOfWarDisabled = false;


	bool terrainDebug = false;
	bool playerDebug = true;
	bool soulsDebug = false;
	bool devourerDebug = false;
	bool soulsConduitDebug = true;

	
};