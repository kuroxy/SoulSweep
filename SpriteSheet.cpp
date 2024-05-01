#include "SpriteSheet.hpp"
#include "Camera.hpp"
#include <cassert>


void Engine::SpriteSheet::draw(Tmpl8::Surface* surface, int x, int y, int xDst, int yDst, bool forceOpaque, bool flip) const
{
	assert(!(x < 0 || x > columns));
	assert(!(y < 0 || y > rows));

	int spriteX = x * cellWidth;
	int spriteY = y * cellHeight;

	texture->PartialCopyToSurface(surface, xDst, yDst, spriteX, spriteY, spriteX + cellWidth, spriteY + cellHeight, forceOpaque, flip);
}

void Engine::SpriteSheet::draw(Tmpl8::Surface* surface, int spriteIndex, int xDst, int yDst, bool forceOpaque, bool flip) const
{
	if (rows == 0 || columns == 0) return;
	int x = spriteIndex % columns;
	int y = spriteIndex / columns;

	draw(surface, x, y, xDst, yDst, forceOpaque, flip);
}

void Engine::SpriteSheet::draw(Engine::Camera& camera, int x, int y, const Tmpl8::vec2& worldSpace, bool flip)
{
	Tmpl8::vec2 local = camera.worldToScreen(worldSpace);

	draw(camera.getSurface(), x, y, static_cast<int>(local.x), static_cast<int>(local.y), false, flip);
}

void Engine::SpriteSheet::draw(Engine::Camera& camera, int spriteIndex, const Tmpl8::vec2& worldSpace, bool flip)
{
	Tmpl8::vec2 local = camera.worldToScreen(worldSpace);

	draw(camera.getSurface(), spriteIndex, static_cast<int>(local.x), static_cast<int>(local.y), false, flip);
}


