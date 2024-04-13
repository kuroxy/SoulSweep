#include "SoulConduit.hpp"
#include "Camera.hpp"



void SoulConduit::setPosition(const Tmpl8::vec2& position)
{
	conduitCollider.setPosition(position);

	// magic numbers??!?!?!
	fireParticles[0].setPosition(position + Tmpl8::vec2(47, 5));
	fireParticles[1].setPosition(position + Tmpl8::vec2(86, 32));
	fireParticles[2].setPosition(position + Tmpl8::vec2(47 , 59));
	fireParticles[3].setPosition(position + Tmpl8::vec2(8, 32));
}

void SoulConduit::spawnCollectedSoul(const Tmpl8::vec2& position)
{
	suckedSouls.push_back(soulLerping(position, getPosition() + Tmpl8::vec2(47.f, 35.f), 20.f, 500.f));
}

void SoulConduit::update(float deltaTime, const Tmpl8::vec2& playerPosition)
{
	for (int i = 0; i < 4; i++)
	{
		if (fireParticlesUpdate[i])
			fireParticles[i].update(deltaTime);
	}


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

void SoulConduit::draw(Engine::Camera& camera, bool debug) const
{

	if (conduitActivated)
		camera.renderSpriteWorldSpace(*conduitSheet.get(), 1, conduitCollider.min);
	else
		camera.renderSpriteWorldSpace(*conduitSheet.get(), 0, conduitCollider.min);


	for (int i = 0; i < 4; i++)
	{
		fireParticles[i].render(camera);
	}


	for (auto& soulParticle : suckedSouls)
	{
		soulParticle.render(camera);
	}

	
	if (debug)
	{
		camera.drawRectangle(conduitCollider.min, conduitCollider.max, 0x00ff00, 1);
	}

}

bool SoulConduit::contains(const Tmpl8::vec2& position) const
{
	return conduitCollider.contains(position);
}

