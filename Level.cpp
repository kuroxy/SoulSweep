#include "Level.hpp"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Camera.hpp"

// for convenience
using json = nlohmann::json;

Level::Level(
	std::shared_ptr<Engine::SpriteSheet> spriteSheetLevel,
	std::shared_ptr<Engine::SpriteSheet> spriteSheetConduit,
	std::shared_ptr<Engine::SpriteSheet> spriteSheetGraveStone,
	std::string_view filename)
{
	// setting spriteSheets
	terrainSpriteSheet = spriteSheetLevel;
	graveStoneSpriteSheet = spriteSheetGraveStone;

	// opening and parsing files
	std::ifstream file(std::string{ filename });
	if (!file.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
	}

	json data = json::parse(file);
	file.close();


	// setting level data


	levelWidth = data["levelWidth"];
	levelHeight = data["levelHeight"];

	terrainTilemap = std::make_unique<Engine::RawTilemap>(terrainSpriteSheet, levelWidth, levelHeight);
	

	terrainColliders.resize(levelWidth * levelHeight);
	terrainColliders.assign(levelWidth * levelHeight, false);

	fogOfWar.resize(levelWidth * levelHeight);
	fogOfWar.assign(levelWidth * levelHeight, Visibility::Unknown);

	for (int i = 0; i < levelWidth*levelHeight; i++)
	{
		int x = i % levelWidth;
		int y = i / levelWidth;
		terrainTilemap->setTile(x, y, data["visualData"][i]);
		terrainColliders[i] = ((int)data["colliderData"][i] == 255);
	}


	float conduitPositionX = (float)data["conduitPosition"][0] * terrainSpriteSheet->getSpriteWidth();
	float conduitPositionY = (float)data["conduitPosition"][1] * terrainSpriteSheet->getSpriteHeight();

	for (auto& spawnLocation : data["spawnLocations"])
	{
		spawnLocations.push_back({ (float)spawnLocation[0] * terrainSpriteSheet->getSpriteWidth(),(float)spawnLocation[1] * terrainSpriteSheet->getSpriteHeight() });
	}


	soulConduit = std::make_unique<SoulConduit>(Tmpl8::vec2(conduitPositionX, conduitPositionY), spriteSheetConduit);

	


}

void Level::draw(Engine::Camera& c) const
{
	terrainTilemap->draw(c);

	// objects
	soulConduit->draw(c);

	for (auto& location : spawnLocations)
	{
		c.renderSpriteWorldSpace(*graveStoneSpriteSheet.get(), 0, location, false);
	}

}

void Level::drawCollision(Engine::Camera& c, Tmpl8::Pixel terrainColor, Tmpl8::Pixel conduitColor) const
{
	for (int y = 0; y < levelHeight; y++)
	{
		for (int x = 0; x < levelWidth; x++)
		{
			if (!terrainColliders[y * levelWidth + x])
				continue;

			Tmpl8::vec2 drawpos = Tmpl8::vec2((float)x * (float)terrainSpriteSheet->getSpriteWidth(), (float)y * (float)terrainSpriteSheet->getSpriteWidth());
			c.drawRectangle(drawpos, drawpos + Tmpl8::vec2((float)(terrainSpriteSheet->getSpriteWidth() - 1.f), (float)(terrainSpriteSheet->getSpriteHeight() - 1.f)), terrainColor, 1);
			
		}
	}

	// also a collider
	soulConduit->draw(c, true);

	for (auto& location : spawnLocations)
	{
		c.drawRectangle(location, location + Tmpl8::vec2(10.f), 0x00ffff);
	}
}

void Level::updateFogOfWar(const Tmpl8::vec2& playerPosition, float minDistance, float maxDistance)
{

	// Again this function assumes that the sprites are square, so if ever wanted a tilemap with different width/height sprites need to rewrite this
	int tileSize = terrainSpriteSheet->getSpriteHeight();

	Tmpl8::vec2 center = Tmpl8::vec2(playerPosition.x / tileSize, playerPosition.y / tileSize);
	float minDist = minDistance / tileSize;
	float maxDist = maxDistance / tileSize;


	for (int y = 0; y < levelHeight; y++)
	{
		for (int x = 0; x < levelWidth; x++)
		{

			float dist = (center - Tmpl8::vec2(x + .5f, y + .5f)).sqrLentgh();
			Visibility newVis = Visibility::Unknown;

			if (dist > maxDist * maxDist)
				newVis = Visibility::Unknown;

			else if (dist < minDist * minDist)
				newVis = Visibility::Light;	// inside the small circle means it is always visible 

			else // somewhere inbetween
			{
				//remove the collide point
				bool wasCollider = false;
				if (terrainColliders[y * levelWidth + x])
				{
					terrainColliders[y * levelWidth + x] = false;
					wasCollider = true;
				}

				// backwards logic a bit 
				// we check the 4 midpoints of each side of the square it one has side is visible this means this tile is visiable.
				bool hidden = true;

				hidden = hidden && lineSegmentCollision(playerPosition, Tmpl8::vec2(x * tileSize + tileSize * .5f, y * tileSize));
				hidden = hidden && lineSegmentCollision(playerPosition, Tmpl8::vec2(x * tileSize + tileSize * .5f, y * tileSize + tileSize));
				hidden = hidden && lineSegmentCollision(playerPosition, Tmpl8::vec2(x * tileSize                 , y * tileSize + tileSize * .5f));
				hidden = hidden && lineSegmentCollision(playerPosition, Tmpl8::vec2(x * tileSize + tileSize      , y * tileSize + tileSize * .5f));

				

				if (hidden)
					newVis = Visibility::Unknown;

				else if ((sqrt(dist) - minDist) / (maxDist - minDist) > .5f)
					newVis = Visibility::Dark;

				else
					newVis = Visibility::Dim;


				terrainColliders[y * levelWidth + x] = wasCollider; // reset if the original position was a collider
			}

			fogOfWar[x + y * levelWidth] = newVis;
		}
	}
}

void Level::drawFogOfWar(Engine::Camera& c) const
{
	int tileSize = terrainSpriteSheet->getSpriteHeight();

	for (int y = 0; y < levelHeight; y++)
	{
		for (int x = 0; x < levelWidth; x++)
		{
			Tmpl8::vec2 drawpos = Tmpl8::vec2((float)(x * tileSize),(float)(y * tileSize));
			
			// can be done better maybe a switch case to get the amount of darkener
			// magic numbers?
			// we do tilesize -1 because it is draw bar is inclusive while a tilewidth is not inclusive
			switch (fogOfWar[y * levelWidth + x])
			{

			case Visibility::Unknown:
				c.drawBarDarkenWorldSpace(drawpos, drawpos + Tmpl8::vec2((float)tileSize - 1.f), 255);
				break;
			case Visibility::Dark:
				c.drawBarDarkenWorldSpace(drawpos, drawpos + Tmpl8::vec2((float)tileSize - 1.f), 100);
				break;
			case Visibility::Dim:
				c.drawBarDarkenWorldSpace(drawpos, drawpos + Tmpl8::vec2((float)tileSize - 1.f), 40);
				break;
			case Visibility::Light:
				break;
			default:
				break;
			}
		}
	}

	
	// outside the level we also draw the fog for 10 more tiles, this seems enough
	// we do this because the camera can view outside the level, and fog of war is only calculated within the level


	//left side
	c.drawRectangle(Tmpl8::vec2(-10.f*tileSize, -10.f * tileSize), Tmpl8::vec2(0.f, (levelHeight + 10.f) * tileSize), 0, 0);
	//right side
	c.drawRectangle(Tmpl8::vec2(levelWidth * tileSize, -10.f * tileSize), Tmpl8::vec2((levelWidth+10.f) * tileSize, (levelHeight + 10.f) * tileSize), 0, 0);
	//top side
	c.drawRectangle(Tmpl8::vec2(0, -10.f * tileSize), Tmpl8::vec2(levelWidth * tileSize, 0.f), 0, 0);
	//bottom side
	c.drawRectangle(Tmpl8::vec2(0, levelHeight * tileSize), Tmpl8::vec2(levelWidth * tileSize, (levelHeight+10.f)*tileSize), 0, 0);

}


bool Level::isCollider(int x, int y) const
{
	if (x < 0 || x >= levelWidth || y < 0 || y >= levelHeight)
		return false;

	return terrainColliders[y * levelWidth + x];
}

bool Level::aabbCollision(const Engine::AABB& aabb) const
{
	int minX = (int)(aabb.min.x / terrainSpriteSheet->getSpriteWidth());
	int minY = (int)(aabb.min.y / terrainSpriteSheet->getSpriteHeight());

	int maxX = (int)ceil(aabb.max.x / terrainSpriteSheet->getSpriteWidth());
	int maxY = (int)ceil(aabb.max.y / terrainSpriteSheet->getSpriteHeight());

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			if (isCollider(x, y))
				return true;
		}
	}


	return false;
}

bool Level::lineSegmentCollision(const Tmpl8::vec2& p1, const Tmpl8::vec2& p2) const
{
	// Written by Javidx9
	// https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_RayCastDDA.cpp
	// almost worked without changing anything only the variable names that are outside this function
	// but some more resources I used to understand it a bit more
	// https://gis.stackexchange.com/questions/70862/how-to-discretise-a-line-into-grid-lines
	// https://stackoverflow.com/questions/3270840/find-the-intersection-between-line-and-grid-in-a-fast-manner
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm (Does skip some squares and is not good for this situation it did help me understand how line-grid intersection algorithms are written)


	Tmpl8::vec2 lineStart = p1;
	lineStart.x /= terrainSpriteSheet->getSpriteWidth();
	lineStart.y /= terrainSpriteSheet->getSpriteHeight();

	Tmpl8::vec2 dir = (p2 - p1).normalized();

	Tmpl8::vec2 vRayUnitStepSize = { std::sqrt(1.f + (dir.y / dir.x) * (dir.y / dir.x)), std::sqrt(1.f + (dir.x / dir.y) * (dir.x / dir.y)) };
	Tmpl8::vec2 vMapCheck = Tmpl8::vec2(floor(lineStart.x), floor(lineStart.y));
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
	float fMaxDistance = (p2 - p1).length() / terrainSpriteSheet->getSpriteWidth();
	float fDistance = 0.0f;
	while (fDistance < fMaxDistance) {

		if (isCollider((int)vMapCheck.x, (int)vMapCheck.y))
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

std::vector<Engine::AABB> Level::getAABBs(const Engine::AABB& aabb) const
{
	std::vector<Engine::AABB> colliders{};

	int minX = (int)(aabb.min.x / terrainSpriteSheet->getSpriteWidth());
	int minY = (int)(aabb.min.y / terrainSpriteSheet->getSpriteHeight());

	int maxX = (int)ceil(aabb.max.x / terrainSpriteSheet->getSpriteWidth());
	int maxY = (int)ceil(aabb.max.y / terrainSpriteSheet->getSpriteHeight());

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			if (isCollider(x, y))
			{
				Tmpl8::vec2 min = Tmpl8::vec2((float)(x * terrainSpriteSheet->getSpriteWidth()), (float)(y * terrainSpriteSheet->getSpriteWidth()));
				colliders.push_back({ min, min + Tmpl8::vec2(terrainSpriteSheet->getSpriteWidth() - 1.f) });
			}
		}
	}

	return colliders;
}

Tmpl8::vec2 Level::resolveBoundryLevelCollision(const Tmpl8::vec2 position, float radius) const
{
	Tmpl8::vec2 newPos = position;
	if (position.x- radius < 0.f)
		newPos.x = radius;
	else if (position.x + radius > levelWidth * terrainSpriteSheet->getSpriteWidth())
		newPos.x = levelWidth * terrainSpriteSheet->getSpriteWidth() - radius;

	if (position.y - radius < 0.f)
		newPos.y = radius;
	else if (position.y + radius > levelHeight * terrainSpriteSheet->getSpriteHeight())
		newPos.y = levelHeight * terrainSpriteSheet->getSpriteHeight() - radius;

	return newPos;
}