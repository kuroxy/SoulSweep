#include "Radar.hpp"
#include "Soul.hpp"
#include "Camera.hpp"
#include <cmath>

void Radar::update(float deltaTime, const Tmpl8::vec2& playerPos, std::vector<Soul>& souls)
{
	positionCenter = playerPos;

	currentAngle += orbitSpeed * deltaTime;
	if (currentAngle > 2.f * Tmpl8::PI)
		currentAngle -= 2.f * Tmpl8::PI;

	positionDraw = { positionCenter.x + std::cos(currentAngle) * orbitRadius, positionCenter.y + std::sin(currentAngle) * orbitRadius };

	Tmpl8::vec2 direction = (positionDraw - positionCenter).normalized();
	float biggestDot = 0;
	for (auto& soul : souls)
	{
		float product = direction.dot((soul.getPosition() - positionCenter).normalized());

		biggestDot = std::max(product, biggestDot* biggestDot);

	}


	if (biggestDot > .9)
	{
		currentSize = (maxSize - minSize) * ((biggestDot - .9f)/.1f) + minSize;
	}
	else
	{
		currentSize = minSize;
	}


}

void Radar::draw(Engine::Camera& camera)
{
	camera.drawCircle(positionDraw, currentSize, color);
}

void Radar::drawDebug(Engine::Camera& camera, std::vector<Soul>& souls)
{
	for (auto& soul : souls)
	{
		camera.drawLine(soul.getPosition(), positionCenter, 0x0000ff);
	}
}
