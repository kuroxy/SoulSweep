#pragma once
#include <memory>
#include "SpriteSheet.hpp"
#include "template.h"
#include "Camera.hpp"




class Tilemap
{
public:
	Tilemap(std::shared_ptr<Engine::SpriteSheet> spriteSheet, int mapWidth, int mapHeight, int tileWidth, int tileHeight);

	const Tmpl8::vec2& getPos() const;
	void setPosition(const Tmpl8::vec2 newPos);
	
	void setTile(int x, int y, int tilemapIndex, bool collider = false);

	void draw(Engine::Camera& c, bool debug = false) const;

	bool doesTileCollide(int x, int y) const;

	bool lineSegmentCollide(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2) const;

	bool lineSegmentCollideDebug(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2, Engine::Camera& c) const; // visual to view what LineSegmentCollision does. Should normally not be used.

private:
	std::shared_ptr<Engine::SpriteSheet> m_spriteSheet{ nullptr };

	Tmpl8::vec2 m_offset{ 0 };

	int* m_mapSprite;
	bool* m_mapCollision;

	int m_mapWidth;
	int m_mapHeight;

	int m_tileWidth;
	int m_tileHeight;

};
