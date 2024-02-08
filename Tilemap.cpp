#include "Tilemap.hpp"
#include <cmath>

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
				c.drawBoxWorldSpace(drawpos, drawpos + Tmpl8::vec2((float)(m_tileWidth-1), (int)(m_tileHeight-1)), 0xff0000);
			}
		}
	}
	
}

bool Tilemap::doesTileCollide(int x, int y) const
{
	if (x < 0 || x >= m_mapWidth || y < 0 || y >= m_mapHeight)
		return false;

	return m_mapCollision[y*m_mapWidth+x];
}


bool Tilemap::lineSegmentCollide(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2) const
{
	// https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_RayCastDDA.cpp
		// not written by me

	Tmpl8::vec2 lineStart = (p1 - m_offset);
	lineStart.x /= m_tileWidth;
	lineStart.y /= m_tileWidth;

	Tmpl8::vec2 dir = (p2 - p1).normalized();

	Tmpl8::vec2 vRayUnitStepSize = { std::sqrt(1.f + (dir.y / dir.x) * (dir.y / dir.x)), std::sqrt(1.f + (dir.x / dir.y) * (dir.x / dir.y)) };
	Tmpl8::vec2 vMapCheck = Tmpl8::vec2((int)lineStart.x, (int)lineStart.y);
	Tmpl8::vec2 vRayLength1D{ 0 };
	Tmpl8::vec2 vStep{ 0 };

	if (dir.x < 0)
	{
		vStep.x = -1;
		vRayLength1D.x = (lineStart.x - float(vMapCheck.x)) * vRayUnitStepSize.x;
	}
	else
	{
		vStep.x = 1;
		vRayLength1D.x = (float(vMapCheck.x + 1) - lineStart.x) * vRayUnitStepSize.x;
	}

	if (dir.y < 0)
	{
		vStep.y = -1;
		vRayLength1D.y = (lineStart.y - float(vMapCheck.y)) * vRayUnitStepSize.y;
	}
	else
	{
		vStep.y = 1;
		vRayLength1D.y = (float(vMapCheck.y + 1) - lineStart.y) * vRayUnitStepSize.y;
	}

	int i = 0;
	float fMaxDistance = (p2 - p1).length() / m_tileWidth;
	float fDistance = 0.0f;
	while (fDistance < fMaxDistance) {

		if (doesTileCollide((int)vMapCheck.x, (int)vMapCheck.y))
			return true;



		if (vRayLength1D.x < vRayLength1D.y)
		{
			vMapCheck.x += vStep.x;
			fDistance = vRayLength1D.x;
			vRayLength1D.x += vRayUnitStepSize.x;
		}
		else
		{
			vMapCheck.y += vStep.y;
			fDistance = vRayLength1D.y;
			vRayLength1D.y += vRayUnitStepSize.y;
		}


	}

	return false;
}

bool Tilemap::lineSegmentCollideDebug(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2, Engine::Camera& c) const
{
	// https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_RayCastDDA.cpp
	// not written by me

	Tmpl8::vec2 lineStart = (p1 - m_offset);
	lineStart.x /= m_tileWidth;
	lineStart.y /= m_tileWidth;

	Tmpl8::vec2 dir = (p2 - p1).normalized();

	Tmpl8::vec2 vRayUnitStepSize = { std::sqrt(1.f + (dir.y / dir.x) * (dir.y / dir.x)), std::sqrt(1.f + (dir.x / dir.y) * (dir.x / dir.y)) };
	Tmpl8::vec2 vMapCheck = Tmpl8::vec2((int)lineStart.x, (int)lineStart.y);
	Tmpl8::vec2 vRayLength1D{ 0 };
	Tmpl8::vec2 vStep{ 0 };

	if (dir.x < 0)
	{
		vStep.x = -1;
		vRayLength1D.x = (lineStart.x - float(vMapCheck.x)) * vRayUnitStepSize.x;
	}
	else
	{
		vStep.x = 1;
		vRayLength1D.x = (float(vMapCheck.x + 1) - lineStart.x) * vRayUnitStepSize.x;
	}

	if (dir.y < 0)
	{
		vStep.y = -1;
		vRayLength1D.y = (lineStart.y - float(vMapCheck.y)) * vRayUnitStepSize.y;
	}
	else
	{
		vStep.y = 1;
		vRayLength1D.y = (float(vMapCheck.y + 1) - lineStart.y) * vRayUnitStepSize.y;
	}

	int i = 0;
	float fMaxDistance = (p2 - p1).length() / m_tileWidth;
	float fDistance = 0.0f;
	while (fDistance < fMaxDistance) {
		Tmpl8::vec2 drawpos = Tmpl8::vec2((int)vMapCheck.x * m_tileWidth, (int)vMapCheck.y * m_tileWidth) + m_offset;
		c.drawBoxWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileWidth - 1, m_tileHeight - 1), 0xff0000);

		if (doesTileCollide((int)vMapCheck.x, (int)vMapCheck.y))
		{
			c.drawLineWorldSpace(p1, p2, 0xff0000);
			return true;
		}


		if (vRayLength1D.x < vRayLength1D.y)
		{
			vMapCheck.x += vStep.x;
			fDistance = vRayLength1D.x;
			vRayLength1D.x += vRayUnitStepSize.x;
		}
		else
		{
			vMapCheck.y += vStep.y;
			fDistance = vRayLength1D.y;
			vRayLength1D.y += vRayUnitStepSize.y;
		}


	}
	c.drawLineWorldSpace(p1, p2, 0x00ff00);
	return false;
}
