#include "SimpleMovement.hpp"
#include "Camera.hpp"

void SimpleMovement::update(float deltaTime)
{
	time += deltaTime;
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
	camera.drawCircle(position, 5, color, 0);
}

Tmpl8::vec2 SimpleMovement::seek(Tmpl8::vec2 pos)
{
	return (pos - position).normalized() * maxForce;
}

Tmpl8::vec2 SimpleMovement::flee(Tmpl8::vec2 pos)
{
	return -seek(pos);
}

Tmpl8::vec2 SimpleMovement::wander(float changeSpeed)
{
	Tmpl8::vec2 a = Tmpl8::vec2(simplex.noise((float)seedX + time * changeSpeed, (float)seedY) * 2.f - 1.f, simplex.noise((float)seedX, seedY + time * changeSpeed) * 2.f - 1.f);
	a.normalize();
	a *= maxForce;
	return a;
}
