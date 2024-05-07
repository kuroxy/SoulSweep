#include "ResourceBar.hpp"
#include "Camera.hpp"
#include "template.h"


void ResourceBar::draw(Engine::Camera& camera, Tmpl8::vec2 screenPosition) const
{
	float progress = Tmpl8::Clamp(currentValue / maxValue, 0.f, 1.f);
	
	Tmpl8::vec2 topLeft(screenPosition.x + static_cast<float>(barLeft), screenPosition.y + static_cast<float>(barTop));
	Tmpl8::vec2 buttomRight(screenPosition.x + static_cast<float>(barLeft + (barRight - barLeft) * progress) -1.f, screenPosition.y + static_cast<float>(barBottom));

	camera.getSurface()->Rectangle(topLeft, buttomRight, barColor, 0);

	barTexture->draw(camera.getSurface(), static_cast<int>(screenPosition.x), static_cast<int>(screenPosition.y));
}
