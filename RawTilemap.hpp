#pragma once
#include <memory>
#include "template.h"
#include "SpriteSheet.hpp"


namespace Engine
{
class Camera;


class RawTilemap
{
public:
	RawTilemap() = delete;
	RawTilemap(std::shared_ptr<Engine::SpriteSheet> spriteSheet, int mapWidth, int mapHeight)
		: spriteSheet{ spriteSheet }
		, width{ mapWidth }
		, height{ mapHeight }
		, mapData{ new int[mapWidth* mapHeight] } {};
	RawTilemap(std::shared_ptr<Engine::SpriteSheet> spriteSheet, int mapWidth, int mapHeight, const Tmpl8::vec2& offset)
		: spriteSheet{ spriteSheet }
		, width{ mapWidth }
		, height{ mapHeight }
		, offset{ offset }
		, mapData{ new int[mapWidth * mapHeight] } {};

	~RawTilemap()
	{
		delete[] mapData;
	}

	int getWidth() { return width; }
	int getHeight() { return height; }
	int* getData() { return mapData; }

	const Tmpl8::vec2& getOffset() { return offset; }
	void setOffset(const Tmpl8::vec2& newOffset) { offset = newOffset; }

	void setTile(int x, int y, int tilemapIndex);

	void draw(Engine::Camera& c) const;


private:
	std::shared_ptr<Engine::SpriteSheet> spriteSheet{ nullptr };

	int width{ 0 };
	int height{ 0 };
	int* mapData{ nullptr };

	Tmpl8::vec2 offset{ 0 };
};
}
