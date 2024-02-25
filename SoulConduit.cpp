#include "SoulConduit.hpp"
#include "Camera.hpp"

void SoulConduit::draw(Engine::Camera& camera, bool debug) const
{

	camera.drawBoxWorldSpace(collider.min, collider.max, 0x00ff00); // for now no texture is used so we draw the debug variant

	if (debug)
	{
		camera.drawBoxWorldSpace(collider.min, collider.max, 0x00ff00);
	}
}

bool SoulConduit::contains(const Tmpl8::vec2& position) const
{
	return collider.contains(position);
}
