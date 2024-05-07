#include "SoulConduit.hpp"
#include "Camera.hpp"




void SoulConduit::spawnCollectedSoul(const Tmpl8::vec2& position)
{
	suckedSouls.push_back(soulLerping(position, getPosition() + Tmpl8::vec2(47.f, 35.f), 20.f, 500.f));
}

void SoulConduit::update(float deltaTime, const Tmpl8::vec2& playerPosition)
{

	for (auto& soulParticle : suckedSouls)
	{
		soulParticle.update(deltaTime);
	}



	for (auto soulParticleIter = suckedSouls.begin(); soulParticleIter != suckedSouls.end();/*no increase we do it manually, bc we can also remove souls*/)
	{
		soulParticleIter->update(deltaTime);

		if (soulParticleIter->isDone())
		{
			soulParticleIter = suckedSouls.erase(soulParticleIter);
			collectedSouls++;
			continue;
		}

		++soulParticleIter;

	}


	conduitActivated = conduitCollider.contains(playerPosition);
}



void SoulConduit::draw(Engine::Camera& camera) const
{
	// If the conduit is activated we draw the second sprite which has an indicator
	int spriteIndex = conduitActivated ? 1 : 0;

	conduitSheet->draw(camera, spriteIndex, conduitCollider.min);


	for (auto& soulParticle : suckedSouls)
	{
		soulParticle.render(camera);
	}


}

void SoulConduit::drawDebug(Engine::Camera& camera) const
{
	camera.drawRectangle(conduitCollider.min, conduitCollider.max, 0x00ff00, 1);
}


