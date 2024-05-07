#pragma once

#include <memory>

#include "template.h"
#include "SpriteSheet.hpp"


namespace Engine
{
class Camera;


class Tilemap
{
public:
	Tilemap() = delete;
	Tilemap(std::shared_ptr<Engine::SpriteSheet> terrainSpriteSheet, int mapWidth, int mapHeight)
		: spriteSheet{ terrainSpriteSheet }
		, width{ mapWidth }
		, height{ mapHeight }
		, mapData{ std::make_unique<int[]>(width * height) } {}

	Tilemap(std::shared_ptr<Engine::SpriteSheet> terrainSpriteSheet, int mapWidth, int mapHeight, const Tmpl8::vec2& offset)
		: spriteSheet{ terrainSpriteSheet }
		, width{ mapWidth }
		, height{ mapHeight }
		, offset{ offset } 
		, mapData{ std::make_unique<int[]>(width * height) } {}


	int getWidth() { return width; }
	int getHeight() { return height; }\

	int* getData() { return mapData.get(); }

	const Tmpl8::vec2& getOffset() { return offset; }
	void setOffset(const Tmpl8::vec2& newOffset) { offset = newOffset; }

	void setTile(int x, int y, int tilemapIndex);

	void draw(Engine::Camera& c) const;


private:
	std::shared_ptr<Engine::SpriteSheet> spriteSheet{ nullptr };

	int width{ 0 };
	int height{ 0 };

	std::unique_ptr<int[]> mapData;
	
	Tmpl8::vec2 offset{ 0 };
};
}
