#include "Tilemap.hpp"
#include <iostream>
#include "Camera.hpp"

void Engine::Tilemap::setTile(int x, int y, int tilemapIndex)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		std::cerr << "Error: Attempting to set tile at out-of-bounds coordinates (" << x << ", " << y << ")" << std::endl;
		return;
	}

	int index = y * width + x;
	mapData[index] = tilemapIndex;
}

void Engine::Tilemap::draw(Engine::Camera& camera) const
{
	if (spriteSheet == nullptr)
	{
		std::cerr << "Error: Spritesheet is empty" << std::endl;
		return;
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Tmpl8::vec2 drawpos = Tmpl8::vec2(x * spriteSheet->getSpriteWidth() + offset.x, y * spriteSheet->getSpriteHeight() + offset.y);

			spriteSheet->draw(camera, mapData[y * width + x], drawpos);
		}
	}
}
