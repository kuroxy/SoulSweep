#pragma once
#include <memory>
#include "SpriteSheet.hpp"
#include "template.h"
#include <string>

// forward declaration
namespace Engine
{
	class Camera;
	struct AABB;
}



class Tilemap
{
public:
	enum class Visibility
	{
		Unknown,
		Dark,
		Dim,
		Light
	};

	Tilemap() = default;
	Tilemap(std::shared_ptr<Engine::SpriteSheet> spriteSheet, int mapWidth, int mapHeight, float viewDistanceMin, float viewDistanceMax);
	Tilemap(std::shared_ptr<Engine::SpriteSheet> spriteSheet, std::string_view fileMap, std::string_view fileCollision, float viewDistanceMin, float viewDistanceMax);

	~Tilemap()
	{
		delete[] m_mapSprite;
		delete[] m_mapCollision;
	}

	const Tmpl8::vec2& getPos() const;
	void setPosition(const Tmpl8::vec2 newPos);
	
	const Tmpl8::vec2& getOffset() const { return m_offset; }

	void setTile(int x, int y, int tilemapIndex, bool collider = false);

	Tmpl8::vec2 worldToGrid(Tmpl8::vec2 worldSpace) const;

	void updateVisibility(Tmpl8::vec2 worldspace);
	void draw(Engine::Camera& c, bool debug = false) const;
	void drawFOW(Engine::Camera& c) const;

	bool doesTileCollide(int x, int y) const;

	bool lineSegmentCollide(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2) const;

	bool boxCollide(const Engine::AABB& box) const;

	Tmpl8::vec2 resolveBoxCollision(const Engine::AABB& box, const Tmpl8::vec2 dir) const; //returns a vector to add to fix position

	bool lineSegmentCollideDebug(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2, Engine::Camera& c) const; // visual to view what LineSegmentCollision does. Should normally not be used.

	bool boxCollideDebug(const Engine::AABB& box, Engine::Camera& c) const; // same as lineSegmentCollideDebug

	void toggleFogOfWar() {
		fogOfWarEnabled = !fogOfWarEnabled;
	}

private:
	std::shared_ptr<Engine::SpriteSheet> m_spriteSheet{ nullptr };

	Tmpl8::vec2 m_offset{ 0 };

	int* m_mapSprite{ nullptr };
	bool* m_mapCollision{ nullptr };
	Visibility* m_tileVisibiliy{ nullptr };

	int m_mapWidth{ 0 };
	int m_mapHeight{ 0 };

	int m_tileSize{ 0 };


	float viewDistanceMin{0.f};
	float viewDistanceMax{0.f};

	bool fogOfWarEnabled = false;
};
