#include "SoulConduit.hpp"
#include "Camera.hpp"



void SoulConduit::setPosition(const Tmpl8::vec2& position)
{
	conduitCollider.setPosition(position);

	// magic numbers??!?!?!
	particleNorth.setPosition(position + Tmpl8::vec2(47, 5));
	particleEast.setPosition(position + Tmpl8::vec2(86, 32));
	particleSouth.setPosition(position + Tmpl8::vec2(47 , 59));
	particleWest.setPosition(position + Tmpl8::vec2(8, 32));
}

void SoulConduit::update(float deltaTime, Tmpl8::vec2 playerPosition)
{
	particleNorth.update(deltaTime);
	particleEast.update(deltaTime);
	particleSouth.update(deltaTime);
	particleWest.update(deltaTime);

	conduitActivated = conduitCollider.contains(playerPosition);
}

void SoulConduit::draw(Engine::Camera& camera, bool debug) const
{

	if (conduitActivated)
		camera.renderSpriteWorldSpace(*conduitSheet.get(), 1, conduitCollider.min);
	else
		camera.renderSpriteWorldSpace(*conduitSheet.get(), 0, conduitCollider.min);

	particleNorth.render(camera);
	particleEast.render(camera);
	particleSouth.render(camera);
	particleWest.render(camera);
	
	
	if (debug)
	{
		camera.drawBoxWorldSpace(conduitCollider.min, conduitCollider.max, 0x00ff00);
	}

}

bool SoulConduit::contains(const Tmpl8::vec2& position) const
{
	return conduitCollider.contains(position);
}
