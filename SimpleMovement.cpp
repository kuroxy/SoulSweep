#include "SimpleMovement.hpp"
#include "Camera.hpp"

void SimpleMovement::update(float deltaTime)
{
	velocity += acceleration * deltaTime;
	velocity = velocity.normalized() * Tmpl8::Min<float>(maxSpeed, velocity.length());

	position += velocity * deltaTime;

	acceleration = Tmpl8::vec2(0.0f);
}

void SimpleMovement::addForce(Tmpl8::vec2 force)
{
	Tmpl8::vec2 clampedForce = force.normalized() * Tmpl8::Min<float>(maxForce, force.length());
	acceleration += Tmpl8::vec2(clampedForce.x / mass, clampedForce.y / mass);
}

void SimpleMovement::draw(Engine::Camera& camera, Tmpl8::Pixel color)
{
	camera.drawFillCircleWorldSpace(position, 5, color, false);
}

Tmpl8::vec2 SimpleMovement::seek(Tmpl8::vec2 pos)
{
	return (pos - position).normalized() * maxForce;
}

Tmpl8::vec2 SimpleMovement::flee(Tmpl8::vec2 pos)
{
	return -seek(pos);
}
