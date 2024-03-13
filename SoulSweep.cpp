#include "SoulSweep.hpp"
#include "Texture.hpp"
#include "InputManager.hpp"
#include <format>
#include "Config.hpp"



Soul& SoulSweep::spawnSoul(const Tmpl8::vec2& spawnPosition, const Tmpl8::vec2& initialVelocity)
{
	souls.push_back(Soul(spawnPosition,
		Config::soulMaxForce,
		Config::soulVacuumSpeed,
		Config::soulFleeSpeed,
		Config::soulDefaultSpeed,
		Config::soulMaxNeighbourRadius,
		Config::soulMinPlayerDist,
		Config::soulMaxPlayerDistance,
		Config::soulWanderSpeed,
		Config::soulCollisionRadius,
		soulParticles));
	
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
		devourerParticles));

	return devourers.back();
}

void SoulSweep::update(float deltaTime, Engine::InputManager im)
{
	if (im.isActionPressed("debugfogofwar"))
		terrainTileMap->toggleFogOfWar();

	mainPlayer->handleInput(im);

	mainPlayer->update(deltaTime, *terrainTileMap); // handles collision

	if (mainPlayer->shouldDropSoul())
	{
		mainPlayer->removeSoul();
		spawnSoul(mainPlayer->getPosition(), mainPlayer->getVacuumDirection()*100.f);
	}


	terrainTileMap->updateVisibility(mainPlayer->getPosition());

	// TODO: maybe can clean this up?
	for (auto soulIter = souls.begin(); soulIter != souls.end();/*no increase we do it manually, bc we can also remove souls*/)
	{
		bool removeSoul = false;

		// Soul Behavior
		soulIter->chooseBehavior(*terrainTileMap, *mainPlayer, souls, devourers);
		soulIter->update(deltaTime, *mainPlayer); //updates position based on velocity and acceleration

		// if eaten remove it from the game.
		if (soulIter->isEaten)
			removeSoul = true;


		// TODODODOODO!!!!!!!!!!!!!!!!! LOOK AT DIFF. WHAT NEEDS TO BE IMPLEMENTED
		
		if (removeSoul)
		{
			soulIter = souls.erase(soulIter);
			continue;
		}

		++soulIter;

	}


	for (auto& devourer : devourers)
	{
		devourer.chooseBehavior(*terrainTileMap, *mainPlayer, souls);
		devourer.update(deltaTime);
	}

}

void SoulSweep::render(Engine::Camera& camera)
{
	// draw order
	// terrain -> SoulConduit -> souls -> ?(monster) -> player -> ?(fog of war)
	terrainTileMap->draw(camera, terrainDebug);
	soulConduit.draw(camera, soulsConduitDebug);


	for (auto& soul : souls)
	{
		soul.draw(camera, soulsDebug);
	}

	for (auto& devourer : devourers)
	{
		devourer.draw(camera, devourerDebug);
	}

	mainPlayer->draw(camera, playerDebug);
}
