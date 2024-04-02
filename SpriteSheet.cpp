#include "SpriteSheet.hpp"
#include <cassert>



int Engine::SpriteSheet::getSpriteWidth() const
{
	return m_cellWidth;
}

int Engine::SpriteSheet::getSpriteHeight() const
{
	return m_cellHeight;
}

int Engine::SpriteSheet::getColumns() const
{
	return m_columns;
}

int Engine::SpriteSheet::getRows() const
{
	return m_rows;
}

void Engine::SpriteSheet::drawSprite(Tmpl8::Surface* surface, int x, int y, int xDst, int yDst, bool useTransparency, bool flip) const
{
	assert(!(x < 0 || x > m_columns));
	assert(!(y < 0 || y > m_rows));

	int spriteX = x * m_cellWidth;
	int spriteY = y * m_cellHeight;

	m_sheet->PartialCopyToSurface(surface, xDst, yDst, spriteX, spriteY, spriteX + m_cellWidth, spriteY + m_cellHeight, useTransparency, flip);
}

void Engine::SpriteSheet::drawSprite(Tmpl8::Surface* surface, int spriteIndex, int xDst, int yDst, bool useTransparency, bool flip) const
{
	if (m_rows == 0 || m_columns == 0) return;
	int x = spriteIndex % m_rows;
	int y = spriteIndex / m_columns;

	drawSprite(surface, x, y, xDst, yDst, useTransparency, flip);
}


