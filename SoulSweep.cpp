#include "SoulSweep.hpp"
#include "Texture.hpp"
#include "InputManager.hpp"
#include <format>
#include "Config.hpp"



Soul& SoulSweep::spawnSoul(Tmpl8::vec2 spawnPosition)
{
	souls.push_back(Soul(spawnPosition));
	return souls.back();
}

void SoulSweep::update(float deltaTime, Engine::InputManager im)
{
	if (im.isActionDown("debugfogofwar"))
		terrainTileMap->toggleFogOfWar();

	mainPlayer->handleInput(im);

	mainPlayer->update(deltaTime, *terrainTileMap); // handles collision

	if (mainPlayer->shouldDropSoul())
	{
		mainPlayer->removeSoul();
		spawnSoul(mainPlayer->getPosition());
	}


	terrainTileMap->updateVisibility(mainPlayer->getPosition());

	// TODO: fix this shii up.
	for (std::vector<Soul>::iterator it = souls.begin(); it != souls.end();/*no increase we do it manually, bc we can also remove souls*/)
	{
		it->actionSelection();
		it->update(deltaTime, *mainPlayer); //updates position based on velocity and acceleration

		// checks if soul can be collected
		
		if (soulConduit.contains(it->getPosition()))
		{
			collectedSouls++;
			it = souls.erase(it);
			continue; // we skip the ++it
		}

		if (mainPlayer->isVacuumEnabled() && mainPlayer->canCollectSoul())
		{
			// calculate it the collect radius of the players hits the soul
			float rad = mainPlayer->getCollectRadius() + it->getCollectRadius();
			if ((mainPlayer->getPosition() - it->getPosition()).sqrLentgh() < rad * rad)
			{
				mainPlayer->collectSoul();
				// now need to remove 

				it = souls.erase(it);
				continue; // we skip the ++it

			}
		}

		++it;

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
