#pragma once

#include <vector>

#include "template.h"
#include "surface.h"


// Forward declaration
namespace Engine
{
	class Camera;
}

class Soul;


class Radar
{
public:
	Radar(float orbitRadius,float orbitSpeed)
		: orbitRadius{orbitRadius}
		, orbitSpeed{orbitSpeed} {}


	void update(float deltaTime, const Tmpl8::vec2& playerPos, std::vector<Soul>& souls);

	void draw(Engine::Camera& camera);
	void drawDebug(Engine::Camera& camera, std::vector<Soul>& souls);

private:
	float orbitRadius = 10.f;
	float orbitSpeed = Tmpl8::PI; // in radians per second
	
	
	Tmpl8::vec2 positionCenter;
	Tmpl8::vec2 positionDraw;
	float currentAngle = 0.f; // radians

	float maxSize = 7.f;
	float minSize = 2.f;
	float currentSize = 5.f;
	Tmpl8::Pixel color = 0xffffff;
	

};
