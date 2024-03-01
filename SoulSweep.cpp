#include "SoulSweep.hpp"
#include "Texture.hpp"
#include "InputManager.hpp"
#include <format>
#include "Config.hpp"



Soul& SoulSweep::spawnSoul(const Tmpl8::vec2& spawnPosition, const Tmpl8::vec2& initialVelocity)
{
	souls.push_back(Soul(spawnPosition, soulParticles));
	souls.back().setVelocity(initialVelocity);
	return souls.back();
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
		
		soulIter->actionSelection();
		soulIter->update(deltaTime, *mainPlayer); //updates position based on velocity and acceleration

		// checks if soul can be collected
		
		if (soulConduit.contains(soulIter->getPosition()))
		{
			collectedSouls++;
			soulIter = souls.erase(soulIter);
			continue; // we skip the ++it
		}

		if (mainPlayer->isVacuumEnabled() && mainPlayer->canCollectSoul())
		{
			// calculate it the collect radius of the players hits the soul
			float rad = mainPlayer->getCollectRadius() + soulIter->getCollectRadius();
			if ((mainPlayer->getPosition() - soulIter->getPosition()).sqrLentgh() < rad * rad)
			{
				mainPlayer->collectSoul();
				// now need to remove 

				soulIter = souls.erase(soulIter);
				continue; // we skip the ++it

			}
		}

		++soulIter;

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

	mainPlayer->draw(camera, playerDebug);
}
