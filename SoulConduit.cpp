#include "SoulConduit.hpp"
#include "Camera.hpp"



void SoulConduit::setPosition(const Tmpl8::vec2& position)
{
	conduitCollider.setPosition(position);
	particleNorth->setPosition(position + Tmpl8::vec2(conduitSheet->getSpriteWidth() / 2, 0));
	particleEast->setPosition (position + Tmpl8::vec2(conduitSheet->getSpriteWidth()    , conduitSheet->getSpriteHeight() / 2));
	particleSouth->setPosition(position + Tmpl8::vec2(conduitSheet->getSpriteWidth() / 2, conduitSheet->getSpriteHeight()));
	particleWest->setPosition (position + Tmpl8::vec2(0									, conduitSheet->getSpriteHeight() / 2));
}

void SoulConduit::update(float deltaTime, Tmpl8::vec2 playerPosition)
{
	particleNorth->updateParticles(deltaTime);
	particleEast->updateParticles(deltaTime);
	particleSouth->updateParticles(deltaTime);
	particleWest->updateParticles(deltaTime);

	conduitActivated = conduitCollider.contains(playerPosition);
}

void SoulConduit::draw(Engine::Camera& camera, bool debug) const
{

	if (conduitActivated)
		camera.renderSpriteWorldSpace(*conduitSheet.get(), 1, conduitCollider.min);
	else
		camera.renderSpriteWorldSpace(*conduitSheet.get(), 0, conduitCollider.min);

	particleNorth->renderParticles(camera);
	particleEast->renderParticles(camera);
	particleSouth->renderParticles(camera);
	particleWest->renderParticles(camera);
	
	
	if (debug)
	{
		camera.drawBoxWorldSpace(conduitCollider.min, conduitCollider.max, 0x00ff00);
	}

}

bool SoulConduit::contains(const Tmpl8::vec2& position) const
{
	return conduitCollider.contains(position);
}
