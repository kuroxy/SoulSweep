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

Soul& SoulSweep::spawnRandomSoul(float distance)
{
	float i = Rand(Tmpl8::PI);
	Tmpl8::vec2 position(cos(i), sin(i));
	return spawnSoul(mainPlayer->getPosition() + position * distance, Tmpl8::vec2(0));
}

Devourer& SoulSweep::spawnRandomDevourer(float distance)
{
	float i = Rand(Tmpl8::PI);
	Tmpl8::vec2 position(cos(i), sin(i));
	return spawnDevourer(mainPlayer->getPosition() + position * distance);
}

void SoulSweep::update(float deltaTime, Engine::InputManager im, Engine::Camera& camera)
{
	// update camera

	camera.setPosition(lerp(camera.getPosition(), mainPlayer->getPosition()-Tmpl8::vec2(camera.getWidth(), camera.getHeight()) * .5f, deltaTime*2) );



	if (im.isActionPressed("debugfogofwar"))
		fogOfWarDisabled = !fogOfWarDisabled;

	if (!fogOfWarDisabled)
	{
		level->updateFogOfWar(mainPlayer->getPosition(), Config::viewDistanceMin, Config::viewDistanceMax);
	}

	level->updateSoulConduit(deltaTime, mainPlayer->getPosition());

	mainPlayer->handleInput(im);

	mainPlayer->update(deltaTime, *level.get()); // handles collision

	if (mainPlayer->shouldDropSoul())
	{
		mainPlayer->removeSoul();
		spawnSoul(mainPlayer->getPosition(), mainPlayer->getVacuumDirection()*100.f);
	}


	//terrainTileMap->updateVisibility(mainPlayer->getPosition());

	// TODO: maybe can clean this up?
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
		spawnRandomSoul(Config::spawnDistance);
	}


	// game state

	if (mainPlayer->isDead() && deathTimer > 0.f)
		deathTimer -= deltaTime;

	if (level->getSoulAmount() >= collectSoulsGoal && victoryTimer > 0.f)
		victoryTimer -= deltaTime;



	//UI

	collectedSoulsBar.setValue(level->getSoulAmount());


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
		devourer.draw(camera, devourerDebug);
	}

	mainPlayer->draw(camera, playerDebug);

	if (!mainPlayer->isDead())
		playerRadar.draw(camera);

	if (!fogOfWarDisabled)
	{
		level->drawFogOfWar(camera);
	}


	// DEBUG STUFF


	if (terrainDebug)
		level->drawCollision(camera);


	if (soulsDebug)
	{
		for (auto& soul : souls)
		{
			soul.drawDebug(camera);
		}
	}


	if (playerDebug)
	{
		playerRadar.drawDebug(camera, souls);
	}

	// UI
	


	collectedSoulsBar.draw(camera, Tmpl8::vec2((camera.getWidth() - collectedSoulsBar.getFullWidth()) / 2.f, 10.f), 0x6600ff);




	// -- fog of war --

	//camera.drawCircleWorldSpace(mainPlayer->getPosition(), Config::viewDistanceMin, 0xff0ff0);

	//camera.drawCircleWorldSpace(mainPlayer->getPosition(), Config::viewDistanceMax, 0xff0ff0);


	//terrainTileMap->drawFOW(camera);
}
