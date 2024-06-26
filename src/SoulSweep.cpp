#include "SoulSweep.hpp"
#include "Texture.hpp"
#include "InputManager.hpp"
#include <format>
#include "Config.hpp"


Tmpl8::vec2 lerp(Tmpl8::vec2 p1, Tmpl8::vec2 p2, float progress)
{
	return (p2 - p1) * progress + p1;
}


Soul& SoulSweep::spawnSoul(const Tmpl8::vec2& spawnPosition, const Tmpl8::vec2& initialVelocity)
{
	souls.push_back(Soul(spawnPosition,
		Config::soulMaxForce,
		Config::soulVacuumSpeed,
		Config::soulFleeSpeed,
		Config::soulDefaultSpeed,
		Config::soulMaxNeighbourRadius,
		level->getLevelArea(), 
		Config::soulWanderDist,
		Config::soulCollisionRadius,
		Config::soulParticleParams));
	
	souls.back().setVelocity(initialVelocity);
	return souls.back();
}

Devourer& SoulSweep::spawnDevourer(const Tmpl8::vec2& spawnPosition)
{
	devourers.push_back(Devourer(spawnPosition, 
		Config::devourerMaxSpeed, 
		Config::devourerMaxForce, 
		Config::devourerConsumingTime, 
		Config::devourerVisitRadius, 
		Config::devourerMaxPlayerDistance, 
		Config::devourerCollideRadius, 
		Config::devourerParticleParams));

	return devourers.back();
}

Soul& SoulSweep::spawnRandomSoul()
{
	std::vector<Tmpl8::vec2> spawnLocations = level->getSpawnLocations();
	
	int closestIndex = 0;
	float sqDist = INFINITY;
	
	for (int i = 0; i < spawnLocations.size(); i++)
	{
		float dist = (mainPlayer->getPosition() - spawnLocations.at(i)).sqrLentgh();
		if (dist < sqDist)
		{
			sqDist = dist;
			closestIndex = i;
		}
	}

	
	int random = IRand(static_cast<int>(spawnLocations.size())-1);
	random = random == closestIndex ? random + 1 : random; // if the random index is the same as the closest we add 1 to get the next one instead, this will always work because the the if the closestIndex is the last, IRand will never generate that value because it is excluse the parameter


	return spawnSoul(spawnLocations.at(random), Tmpl8::vec2(0));
}

Devourer& SoulSweep::spawnRandomDevourer()
{
	std::vector<Tmpl8::vec2> spawnLocations = level->getSpawnLocations();

	int furthestIndex = 0;
	float sqDist = 0;

	for (int i = 0; i < spawnLocations.size(); i++)
	{
		float dist = (mainPlayer->getPosition() - spawnLocations.at(i)).sqrLentgh();
		if (dist > sqDist)
		{
			sqDist = dist;
			furthestIndex = i;
		}
	}

	return spawnDevourer(spawnLocations.at(furthestIndex));
}

void SoulSweep::update(float deltaTime, Engine::InputManager im, Engine::Camera& camera)
{
	// update camera
	
	// newPlayerPosition is the position such that the player is in the center of the screen
	// Then we lerp towards the new position, this way we can have a smooth moving camera that "lags" behind the player, instead of being locked onto the player
	Tmpl8::vec2 newPlayerPosition = mainPlayer->getPosition() - Tmpl8::vec2(static_cast<float>(camera.getWidth()), static_cast<float>(camera.getHeight())) * .5f;

	camera.setPosition(lerp(camera.getPosition(), newPlayerPosition, deltaTime*2.f) );

	// Update level

	if (im.isActionPressed("debugfogofwar"))
		fogOfWarDisabled = !fogOfWarDisabled;

	if (!fogOfWarDisabled)
	{
		level->updateFogOfWar(mainPlayer->getPosition(), Config::viewDistanceMin, Config::viewDistanceMax);
	}

	level->updateSoulConduit(deltaTime, mainPlayer->getPosition());


	// Update player

	mainPlayer->handleInput(im);

	mainPlayer->update(deltaTime); // handles collision

	mainPlayer->checkCollisions(level->getAABBs(mainPlayer->getAABB()));

	if (mainPlayer->shouldDropSoul())
	{
		mainPlayer->removeSoul();
		spawnSoul(mainPlayer->getPosition(), mainPlayer->getVacuumDirection()*100.f);
	}


	// Update souls
	
	for (auto soulIter = souls.begin(); soulIter != souls.end();/*no increase we do it manually, bc we can also remove souls*/)
	{
		bool removeSoul = false;

		// Soul Behavior
		soulIter->chooseBehavior(*level.get(), *mainPlayer, souls, devourers);
		soulIter->update(deltaTime, *mainPlayer); //updates position based on velocity and acceleration
		soulIter->setPosition(level->resolveBoundryLevelCollision(soulIter->getPosition(), soulIter->getCollisionRadius()));

		// if eaten remove it from the game.
		if (soulIter->isEaten)
			removeSoul = true;
		
		

		if (!mainPlayer->isDead() &&level->isSoulCollectable(soulIter->getPosition()))
		{
			level->collectSoul(soulIter->getPosition());
			removeSoul = true;
		}

		if (!mainPlayer->isDead() && mainPlayer->isVacuumEnabled() && mainPlayer->canCollectSoul())
		{
			// calculate it the collect radius of the players hits the soul
			float rad = mainPlayer->getCollectRadius() + soulIter->getCollisionRadius();
			if ((mainPlayer->getPosition() - soulIter->getPosition()).sqrLentgh() < rad * rad)
			{
				mainPlayer->collectSoul();
				// now need to remove 

				removeSoul = true;

			}
		}

		if (removeSoul)
		{
			soulIter = souls.erase(soulIter);
			continue;
		}

		++soulIter;

	}

	// Update devourers

	for (auto& devourer : devourers)
	{
		devourer.chooseBehavior(*level.get(), *mainPlayer, souls);
		devourer.update(deltaTime);

		float distsq = (devourer.getPosition() - mainPlayer->getPosition()).sqrLentgh();
		if (distsq < (devourer.getCollideRadius() + mainPlayer->getCollectRadius()) * (devourer.getCollideRadius() + mainPlayer->getCollectRadius()))
		{
			mainPlayer->setDeath(true);
		}
	}


	playerRadar.update(deltaTime, mainPlayer->getPosition(), souls);


	// spawn regulator

	if (souls.size() < Config::minSouls)
	{
		spawnRandomSoul();
	}


	// game state
	// We use this timer based because this I wanted a delay before transitioning to the next state/screen.

	if (mainPlayer->isDead() && deathTimer > 0.f)
		deathTimer -= deltaTime;


	if (level->getSoulAmount() >= collectSoulsGoal && victoryTimer > 0.f)
		victoryTimer -= deltaTime;
	else
		gameTime += deltaTime;


	// update UI
	collectedSoulsBar.setValue(static_cast<float>(level->getSoulAmount()));

	dashResource.setValue(mainPlayer->getDashResource());

}

void SoulSweep::render(Engine::Camera& camera)
{
	// draw order
	// terrain -> SoulConduit -> souls -> ?(monster) -> player -> ?(fog of war)
	level->draw(camera);

	for (auto& soul : souls)
	{
		soul.draw(camera);
	}

	for (auto& devourer : devourers)
	{
		devourer.draw(camera);
	}

	mainPlayer->draw(camera);

	if (!mainPlayer->isDead())
		playerRadar.draw(camera);

	if (!fogOfWarDisabled)
	{
		level->drawFogOfWar(camera);
	}


	// DEBUG STUFF


	if (terrainDebug)
	{
		level->drawCollision(camera);
		for (auto aabb : level->getAABBs(mainPlayer->getAABB()))
		{
			aabb.draw(camera, 0x00ff00);
		}
	}
		


	if (soulsDebug)
	{
		for (auto& soul : souls)
		{
			soul.drawDebug(camera);
		}
	}

	if (devourerDebug)
	{
		for (auto& devourer : devourers)
		{
			devourer.drawDebug(camera);
		}
	}


	if (playerDebug)
	{
		mainPlayer->drawDebug(camera);
	}

	if (playerRadarDebug)
	{
		playerRadar.drawDebug(camera, souls);
	}

	// UI
	


	collectedSoulsBar.draw(camera, Tmpl8::vec2((camera.getWidth() - collectedSoulsBar.getFullWidth()) / 2.f, 10.f));

	dashResource.draw(camera, Tmpl8::vec2((camera.getWidth() - dashResource.getFullWidth()) / 2.f, 470.f));

	// -- fog of war --

	//camera.drawCircleWorldSpace(mainPlayer->getPosition(), Config::viewDistanceMin, 0xff0ff0);

	//camera.drawCircleWorldSpace(mainPlayer->getPosition(), Config::viewDistanceMax, 0xff0ff0);


	//terrainTileMap->drawFOW(camera);
}
