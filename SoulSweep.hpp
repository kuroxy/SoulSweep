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

namespace Engine
{
	class InputManager;
};

class SoulSweep {
	void loadSpriteSheet(std::string_view textureFile, int spriteSize, Tmpl8::Pixel chroma)
	{
		spriteSheet = std::make_shared<Engine::SpriteSheet>(textureFile, spriteSize, spriteSize, chroma);
	}

	void loadMap(std::string_view mapName, float viewDistMin, float viewDistMax) 
	{
		std::string sprites = std::format("assets/map/{}L1.csv", mapName);
		std::string collision = std::format("assets/map/{}L2.csv", mapName);

		terrainTileMap = new Tilemap(spriteSheet, sprites, collision, viewDistMin, viewDistMax);
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
	~SoulSweep() { delete mainPlayer; delete terrainTileMap; }

	void Initialize()
	{
		setSoulParticleSystem();
		setDevourerParticleSystem();
		loadSpriteSheet(Config::TERRAIN_TEXTURE, Config::TERRAIN_SPRITE_SIZE, Config::TERRAIN_CHROMA);
		loadMap(Config::MAP_NAME, Config::viewDistanceMin, Config::viewDistanceMax);
		mainPlayer = new Player(Tmpl8::vec2(200.f), Config::PLAYER_SPEED, Config::PLAYER_SIZE, Config::PLAYER_SIZE);; // player is square for now

		for(int i = 0; i < 1; i++)
		{
			spawnSoul(Tmpl8::vec2(Rand(300.f) + 200.f, Rand(300.f) + 200.f), Tmpl8::vec2(0.f));
		}
		
		//spawnDevourer(Tmpl8::vec2(200.f, 200.f));
	}

	Soul& spawnSoul(const Tmpl8::vec2& spawnPosition, const Tmpl8::vec2& initialVelocity);
	Devourer& spawnDevourer(const Tmpl8::vec2& spawnPosition);


	void update(float deltaTime, Engine::InputManager im);

	void render(Engine::Camera& camera);

	
private:

	unsigned int collectedSouls{ 0 }; 


	std::shared_ptr<Engine::SpriteSheet> spriteSheet{ nullptr };
	Tilemap* terrainTileMap{ nullptr };
	
	Player* mainPlayer{ nullptr };
	
	SoulConduit soulConduit{ {50.f,50.f}, {100.f,100.f} };

	std::vector<Soul> souls;
	std::vector<Devourer> devourers;
	Engine::ParticleSystemParams soulParticles;
	Engine::ParticleSystemParams devourerParticles;



	//debug stuff
	bool terrainDebug = false;
	bool playerDebug = true;
	bool soulsDebug = true;
	bool devourerDebug = true;
	bool soulsConduitDebug = true;

	
};