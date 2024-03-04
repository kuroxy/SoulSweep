#include "Tilemap.hpp"
#include "Camera.hpp"
#include "aabb.hpp"
#include "Surface.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

Tilemap::Tilemap(std::shared_ptr<Engine::SpriteSheet> spriteSheet, int mapWidth, int mapHeight)
	: m_spriteSheet{spriteSheet}
	, m_offset{0}
	, m_mapSprite{ new int[mapWidth* mapHeight] }
	, m_mapCollision{ new bool[mapWidth*mapHeight] }
	, m_tileVisibiliy{ new Visibility[mapWidth * mapHeight] }
	, m_mapWidth{ mapWidth }
	, m_mapHeight{ mapHeight }
	, m_tileSize{ spriteSheet->getSpriteWidth()}
{
	for (int i = 0; i < m_mapWidth * m_mapHeight; i++)
	{

		m_mapSprite[i] = 0;
		m_mapCollision[i] = false;
		m_tileVisibiliy[i] = Visibility::Unknown;
	}
	

}

Tilemap::Tilemap(std::shared_ptr<Engine::SpriteSheet> spriteSheet, std::string_view fileMap, std::string_view fileCollision)
	: m_spriteSheet{ spriteSheet }
{
	// big function
	std::ifstream file(std::string{ fileMap });
	std::ifstream fileCol(std::string{ fileCollision });
	if (!file.is_open() || !fileCol.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
	}

	int lines = 0;
	std::vector<int> mapData;
	std::vector<int> colData;

	std::string line;
	while (std::getline(file, line)) {
		lines++;
		std::vector<int> row;
		std::stringstream ss(line);

		std::string cell;
		while (std::getline(ss, cell, ',')) {
			mapData.push_back(std::stoi(cell));
		}

	}
	file.close();


	while (std::getline(fileCol, line)) {
		std::stringstream ss(line);

		std::string cell;
		while (std::getline(ss, cell, ',')) {
			colData.push_back(std::stoi(cell));
		}
	}
	fileCol.close();


	
	m_mapSprite = new int[mapData.size()];
	m_mapCollision = new bool[mapData.size()];
	m_tileVisibiliy = new Visibility[mapData.size()];
	m_mapWidth = (int)mapData.size() / lines;
	m_mapHeight = lines;
	m_tileSize = spriteSheet->getSpriteWidth();

	for (int i = 0; i < m_mapWidth * m_mapHeight; i++)
	{

		m_mapSprite[i] = mapData[i];
		m_mapCollision[i] = colData[i]!=-1;
		m_tileVisibiliy[i] = Visibility::Unknown;

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

Tmpl8::vec2 Tilemap::worldToGrid(Tmpl8::vec2 worldSpace) const
{
	return Tmpl8::vec2((worldSpace.x-m_offset.x)/m_tileSize, (worldSpace.y - m_offset.y) / m_tileSize);
}


void Tilemap::draw(Engine::Camera& c, bool debug) const
{
	for (int y = 0; y < m_mapHeight; y++)
	{
		for (int x = 0; x < m_mapWidth; x++)
		{
			Tmpl8::vec2 drawpos = Tmpl8::vec2(x * m_tileSize + m_offset.x, y * m_tileSize + m_offset.y);
			c.renderSpriteWorldSpace(m_spriteSheet.get(), m_mapSprite[y * m_mapWidth + x], drawpos);
			

			// can be done better maybe a switch case to get the amount of darkener
			// magic numbers?
			// we do tilesize -1 because it is draw bar is inclusive while a tilewidth is not inclusive
			
			if (fogOfWarEnabled)
			{
				switch (m_tileVisibiliy[y * m_mapWidth + x])
				{

				case Visibility::Unknown:
					c.drawBarDarkenWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileSize - 1), 200);
					break;
				case Visibility::Dark:
					c.drawBarDarkenWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileSize - 1), 100);
					break;
				case Visibility::Dim:
					c.drawBarDarkenWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileSize - 1), 40);
					break;
				case Visibility::Light:
					break;
				default:
					break;
				}
			}
			


			if (debug && m_mapCollision[y * m_mapWidth + x])
			{
				c.drawBoxWorldSpace(drawpos, drawpos + Tmpl8::vec2((float)(m_tileSize-1), (int)(m_tileSize -1)), 0xff0000);
			}
		}
	}
	
}

void Tilemap::updateVisibility(Tmpl8::vec2 worldSpace)
{
	if (!fogOfWarEnabled)
		return;

	Tmpl8::vec2 center = worldToGrid(worldSpace);

	for (int y = 0; y < m_mapHeight; y++)
	{
		for (int x = 0; x < m_mapWidth; x++)
		{
			float dist = (center - Tmpl8::vec2(x + .5f , y+.5f)).sqrLentgh();
			Visibility newVis = Visibility::Unknown;

			if (dist < 5*5)
				newVis = Visibility::Light;
			else if (dist < 7*7)
				newVis = Visibility::Dim;
			else if (dist < 10*10)
				newVis = Visibility::Dark;
			else
			{
				if (m_tileVisibiliy[x + y * m_mapWidth] != Visibility::Unknown)
					newVis = Visibility::Dark;
				else
					newVis = Visibility::Unknown;
			}


			m_tileVisibiliy[x + y * m_mapWidth] = newVis;
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
	lineStart.x /= m_tileSize;
	lineStart.y /= m_tileSize;

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
	float fMaxDistance = (p2 - p1).length() / m_tileSize;
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

bool Tilemap::boxCollide(const Engine::AABB& box) const
{
	int minX = (box.min.x - m_offset.x) / m_tileSize;
	int minY = (box.min.y - m_offset.y) / m_tileSize;

	int maxX = ceil((box.max.x - m_offset.x) / m_tileSize);
	int maxY = ceil((box.max.y - m_offset.y) / m_tileSize);

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			if (doesTileCollide(x, y))
				return true;
		}
	}


	return false;
}

bool Tilemap::lineSegmentCollideDebug(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2, Engine::Camera& c) const
{
	// https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_RayCastDDA.cpp
	// not written by me

	Tmpl8::vec2 lineStart = (p1 - m_offset);
	lineStart.x /= m_tileSize;
	lineStart.y /= m_tileSize;

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
	float fMaxDistance = (p2 - p1).length() / m_tileSize;
	float fDistance = 0.0f;
	while (fDistance < fMaxDistance) {
		Tmpl8::vec2 drawpos = Tmpl8::vec2((int)vMapCheck.x * m_tileSize, (int)vMapCheck.y * m_tileSize) + m_offset;
		c.drawBoxWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileSize - 1, m_tileSize - 1), 0x00ff00);

		if (doesTileCollide((int)vMapCheck.x, (int)vMapCheck.y))
		{
			Tmpl8::vec2 drawpos = Tmpl8::vec2((int)vMapCheck.x * m_tileSize, (int)vMapCheck.y * m_tileSize) + m_offset;
			c.drawBoxWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileSize - 1, m_tileSize - 1), 0xff0000);
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

bool Tilemap::boxCollideDebug(const Engine::AABB& box, Engine::Camera& c) const
{
	int minX = (box.min.x - m_offset.x) / m_tileSize;
	int minY = (box.min.y - m_offset.y) / m_tileSize;

	int maxX = ceil((box.max.x - m_offset.x) / m_tileSize);
	int maxY = ceil((box.max.y - m_offset.y) / m_tileSize);

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			Tmpl8::vec2 drawpos = Tmpl8::vec2(x * m_tileSize, y * m_tileSize) + m_offset;
			if (doesTileCollide(x, y))
			{

				c.drawBoxWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileSize - 1, m_tileSize - 1), 0xff0000);
				return true;
			}

			c.drawBoxWorldSpace(drawpos, drawpos + Tmpl8::vec2(m_tileSize - 1.f, m_tileSize - 1.f), 0x00ff00);

		}
	}


	return false;
}

// this function can definitely be made better
Tmpl8::vec2 Tilemap::resolveBoxCollision(const Engine::AABB& box, const Tmpl8::vec2 dir) const 
{
	{
		Tmpl8::vec2 change{ 0 };

		int minX = (int)(box.min.x - m_offset.x) / m_tileSize;
		int minY = (int)(box.min.y - m_offset.y) / m_tileSize;

		int maxX = (int)ceil((box.max.x - m_offset.x) / m_tileSize);
		int maxY = (int)ceil((box.max.y - m_offset.y) / m_tileSize);

		for (int y = minY; y < maxY; y++)
		{
			for (int x = minX; x < maxX; x++)
			{
				if (doesTileCollide(x, y))
				{
					Tmpl8::vec2 min = Tmpl8::vec2(x * m_tileSize + m_offset.x, y * m_tileSize + m_offset.y);
					Engine::AABB tileBox = Engine::AABB(min, min + Tmpl8::vec2(m_tileSize - 1.f));


					if (std::abs(dir.x) > std::abs(dir.y))
					{

						if (dir.x > 0)// we are going to the right so this means max.x = tile.min.
						{
							change.x = tileBox.min.x - box.width() - box.min.x-1;

						}
						else// we are going to the left so this means min.x = tile.max.x
						{
							change.x = tileBox.max.x - box.min.x+1;
						}
					}
					else if (std::abs(dir.x) < std::abs(dir.y))
					{

						if (dir.y > 0)// we are going down so this means max.y = tile.min.y
						{
							change.y = tileBox.min.y - box.height() - box.min.y-1;

						}
						else// we are going up so this means min.y = tile.max.y
						{
							change.y = tileBox.max.y - box.min.y+1;
						}
					}
					else
					{
						Tmpl8::vec2 centerdist = tileBox.center() - box.center();

						if (centerdist.y * centerdist.y > centerdist.x * centerdist.x)
						{
							if (centerdist.y > 0) //top collision
							{ 
								change.y = tileBox.min.y - box.height() - box.min.y - 1;
							}
							else
							{
								change.y = tileBox.max.y - box.min.y + 1;
							}

						}
						else
						{
							if (centerdist.x > 0) //top collision
							{
								change.x = tileBox.min.x - box.width() - box.min.x - 1;

							}
							else
							{
								change.x = tileBox.max.x - box.min.x + 1;
							}
						}

						// now they are the same so probably checking based on center
					}

					return change;
				}

			}
		}


		return change;
	}
}
