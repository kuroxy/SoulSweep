#include "Tilemap.hpp"

Tilemap::Tilemap(std::shared_ptr<Engine::SpriteSheet> spriteSheet, int mapWidth, int mapHeight, int tileWidth, int tileHeight)
	: m_spriteSheet{spriteSheet}
	, m_offset{0}
	, m_mapSprite{ new int[mapWidth* mapHeight] }
	, m_mapCollision{ new bool[mapWidth*mapHeight]}
	, m_mapWidth{ mapWidth }
	, m_mapHeight{ mapHeight }
	, m_tileWidth{ tileWidth }
	, m_tileHeight{ tileHeight }
{
	for (int i = 0; i < m_mapWidth * m_mapHeight; i++)
	{

		m_mapSprite[i] = 0;
		m_mapCollision[i] = false;
		
	}
}

const Tmpl8::vec2& Tilemap::getPos() const
{
	return m_offset;
}

void Tilemap::setPosition(const Tmpl8::vec2 newPos)
{
	m_offset = newPos;
}

void Tilemap::setTile(int x, int y, int tilemapIndex, bool collider)
{
	int index = y * m_mapWidth + x;

	m_mapSprite[index] = tilemapIndex;
	m_mapCollision[index] = collider;
}

void Tilemap::draw(Engine::Camera& c, bool debug) const
{
	for (int y = 0; y < m_mapHeight; y++)
	{
		for (int x = 0; x < m_mapWidth; x++)
		{
			Tmpl8::vec2 drawpos = Tmpl8::vec2(x * m_tileWidth + m_offset.x, y * m_tileHeight + m_offset.y);
			if (m_mapSprite[y * m_mapWidth + x] == 1)
				int i = 3;

			c.renderSpriteWorldSpace(m_spriteSheet.get(), m_mapSprite[y * m_mapWidth + x], drawpos);



			if (debug && m_mapCollision[y * m_mapWidth + x])
			{
				c.drawBoxWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileWidth-1, m_tileHeight-1), 0xff0000);
			}
		}
	}
	
}
