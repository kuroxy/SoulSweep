#pragma once
#include <vector>
#include "Camera.hpp"
#include "Player.hpp"
#include "Tilemap.hpp"
#include "Soul.hpp"
#include "SpriteSheet.hpp"
#include <format>
#include "Config.hpp"

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


	

public:
	SoulSweep() = default;
	~SoulSweep() { delete mainPlayer; delete terrainTileMap; }

	void Initialize()
	{
		loadSpriteSheet(Config::TERRAIN_TEXTURE, Config::TERRAIN_SPRITE_SIZE, Config::TERRAIN_CHROMA);
		loadMap(Config::MAP_NAME);
		mainPlayer = new Player(Tmpl8::vec2(0), Config::PLAYER_SPEED, Config::PLAYER_SIZE, Config::PLAYER_SIZE);; // player is square for now
	}

	void update(float deltaTime, Engine::InputManager im);

	void render(Engine::Camera& camera);

	
private:
	std::shared_ptr<Engine::SpriteSheet> spriteSheet{ nullptr };
	Tilemap* terrainTileMap{ nullptr };
	
	Player* mainPlayer{ nullptr };
	

	std::vector<Soul> souls;


	//debug stuff
	bool terrainDebug = true;
	bool playerDebug = true;
	bool soulsDebug = false;

};