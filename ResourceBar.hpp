#pragma once
#include <memory>

#include "Texture.hpp"


namespace Engine
{
	class Camera;
}
namespace Tmpl8
{
	class vec2;
}


class ResourceBar
{
public:
	ResourceBar(std::shared_ptr < Engine::Texture > texture, int barLeft, int barTop, int barRight, int barBottom, float maxValue, Tmpl8::Pixel barColor)
		: barTexture{ texture }
		, barLeft{ barLeft }
		, barTop{ barTop }
		, barRight{ barRight }
		, barBottom{ barBottom }
		, maxValue{ maxValue } 
		, barColor{ barColor}  {}

	void setMaxValue(float maxValue) {
		this->maxValue = maxValue;
	}
	
	int getFullWidth() const { return barTexture->getWidth(); }
	int getFullHeight() const { return barTexture->getHeight(); }

	void setValue(float value) { currentValue = value; }

	float getValue() const { return currentValue; }


	void draw(Engine::Camera& camera, Tmpl8::vec2 screenPosition) const;

private:
	std::shared_ptr < Engine::Texture > barTexture;
	// from which pixel the bar should start and where it should end. Because resource bars can have some decoration to the right/left of it.
	
	// offsets of the topleft of the texture. This forms the rectangle where the bar is being drawn.
	int barLeft = 0;
	int barTop = 0;

	int barRight = 10;
	int barBottom = 10;



	// the range of the resource bar
	float maxValue = 100.f;
	float currentValue = 0.f;

	Tmpl8::Pixel barColor = 0;

};