#pragma once
#include <memory>
#include "SpriteSheet.hpp"
#include "Template.h"
#include "RawTilemap.hpp"
#include <string_view>
#include <vector>
#include "template.h"
#include "aabb.hpp"
#include "Texture.hpp"
#include <memory>
#include "ParticleSystem.hpp"
#include "SoulConduit.hpp"
#include "aabb.hpp"

class Level
{
	enum class Visibility
	{
		Unknown,
		Dark,
		Dim,
		Light
	};


public:
	Level() = default;
	Level(	std::shared_ptr<Engine::SpriteSheet> spriteSheetLevel, 
			std::shared_ptr<Engine::SpriteSheet> spriteSheetConduit, 
			std::shared_ptr<Engine::SpriteSheet> spriteSheetGraveStone,
			std::string_view filename);

	const std::vector<Tmpl8::vec2>& getSpawnLocations() const {return spawnLocations;}
	Tmpl8::vec2 getPlayerSpawnPosition() const;

	bool isSoulCollectable(const Tmpl8::vec2& soulPosition) const { return soulConduit->isConduitActive() && soulConduit->contains(soulPosition); }

	void collectSoul(const Tmpl8::vec2& soulPosition)
	{
		soulConduit->spawnCollectedSoul(soulPosition);
	}

	int getSoulAmount() const { return soulConduit->getCollectedSouls(); };

	Engine::AABB getLevelArea() const { 
		return Engine::AABB({ 0.f }, { (float)(levelWidth * terrainSpriteSheet->getSpriteWidth()),(float)(levelHeight * terrainSpriteSheet->getSpriteHeight()) }); }

	void draw(Engine::Camera& c) const;
	void drawCollision(Engine::Camera& c, Tmpl8::Pixel terrainColor=0xff0000, Tmpl8::Pixel conduitColor=0x00ff00) const;
	
	// fog of war
	void updateFogOfWar(const Tmpl8::vec2& playerPosition, float minDistance, float maxDistance);
	void drawFogOfWar(Engine::Camera& c) const;


	// collisions
	void updateSoulConduit(float deltaTime, const Tmpl8::vec2 playerPosition) { soulConduit->update(deltaTime, playerPosition); }


	bool isCollider(int x, int y) const; // local x&y should be based on the terrain grid
	bool aabbCollision(const Engine::AABB& aabb) const;
	bool lineSegmentCollision(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2) const;

	std::vector<Engine::AABB> getAABBs(const Engine::AABB& size) const;

	Tmpl8::vec2 resolveBoundryLevelCollision(const Tmpl8::vec2 position, float radius) const; //returns the corrected position. Used to clamp the position within the level.


private:
	std::shared_ptr<Engine::SpriteSheet> terrainSpriteSheet{ nullptr };
	std::shared_ptr<Engine::SpriteSheet> graveStoneSpriteSheet{ nullptr };

	int levelWidth{ 0 };
	int levelHeight{ 0 };

	std::unique_ptr<Engine::RawTilemap> terrainTilemap{nullptr};
	std::vector<bool> terrainColliders{0};
	std::vector<Visibility> fogOfWar{ 0 };

	
	std::unique_ptr<SoulConduit> soulConduit{ nullptr };
	std::vector<Tmpl8::vec2> spawnLocations;

	// Textures



};

