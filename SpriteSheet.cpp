#include "SpriteSheet.hpp"
#include <cassert>


Engine::SpriteSheet::SpriteSheet(std::shared_ptr<Texture> fullTexture, int spriteWidth, int spriteHeight)
	: m_sheet{ fullTexture }
	, m_cellWidth{ spriteWidth }
	, m_cellHeight{ spriteHeight}
	, m_columns{ m_sheet->getWidth() / spriteWidth }
	, m_rows{ m_sheet->getHeight() / spriteHeight }
{
}

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

void Engine::SpriteSheet::drawSprite(Tmpl8::Surface* surface, int x, int y, int xDst, int yDst, bool useTransparency) const
{
	assert(x < 0 || x > m_columns);
	assert(y < 0 || y > m_rows);

	int spriteX = x * m_cellWidth;
	int spriteY = y * m_cellHeight;

	m_sheet->PartialCopyToSurface(surface, xDst, yDst, spriteX, spriteY, spriteX + m_cellWidth, spriteX + m_cellHeight, useTransparency);
}




