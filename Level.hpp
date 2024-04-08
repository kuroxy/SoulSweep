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
	Level(std::shared_ptr<Engine::SpriteSheet> terrainSpriteSheet, std::string_view filename);

	//getters
	const Engine::AABB& getConduit() const { return conduitCollider; }
	bool isSoulConduitActivated() const { return conduitActivated; }


	
	void draw(Engine::Camera& c) const;
	void drawCollision(Engine::Camera& c, Tmpl8::Pixel terrainColor=0xff0000, Tmpl8::Pixel conduitColor=0x00ff00) const;
	
	// fog of war
	void updateFogOfWar(const Tmpl8::vec2& playerPosition, float minDistance, float maxDistance);
	void drawFogOfWar(Engine::Camera& c) const;


	// collisions
	void updateSoulConduit(const Tmpl8::vec2 playerPosition) { conduitActivated = conduitCollider.contains(playerPosition); }


	bool isCollider(int x, int y) const; // local x&y should be based on the terrain grid
	bool aabbCollision(const Engine::AABB& aabb) const;
	bool lineSegmentCollision(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2) const;

	Tmpl8::vec2 resolveBoxCollision(const Engine::AABB& box, const Tmpl8::vec2 dir) const; //returns a vector to add to fix position

private:
	std::shared_ptr<Engine::SpriteSheet> terrainSpriteSheet{ nullptr };

	int levelWidth{ 0 };
	int levelHeight{ 0 };

	std::unique_ptr<Engine::RawTilemap> terrainTilemap{nullptr};
	std::vector<bool> terrainColliders{0};
	std::vector<Visibility> fogOfWar{ 0 };

	// conduitCollider + conduitTexture can be togther like a class that contains this data but for now here

	Engine::AABB conduitCollider;
	
	bool conduitActivated = false;

	// Textures

	Engine::SpriteSheet conduitSheet{ "assets/Textures/SoulConduit.png", 94, 72, 0xff00ff };


};

