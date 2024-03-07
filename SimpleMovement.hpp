#pragma once
#include "template.h"
#include "surface.h"
#include "PerlinNoise.hpp"

namespace Engine {
	class Camera;
}




class SimpleMovement {
public:
	SimpleMovement() = default;
	SimpleMovement(const Tmpl8::vec2& pos)
		: position{ pos }
		, perlinX{ (unsigned int)IRand(INT_MAX) }
		, perlinY{ (unsigned int)IRand(INT_MAX) } {}

	SimpleMovement(const Tmpl8::vec2& pos, float maxSpeed, float maxForce, float mass=1.0f) 
		: position{ pos }
		, velocity { 0.f }
		, acceleration{ 0.f }
		, mass{ mass }
		, maxSpeed{ maxSpeed }
		, maxForce{ maxForce }
		, perlinX{ (unsigned int)IRand(INT_MAX) }
		, perlinY{ (unsigned int)IRand(INT_MAX) } {}
		


	const Tmpl8::vec2& getPosition() const { return position; }
	const Tmpl8::vec2& getVelocity() const { return velocity; }

	float getMass() const { return mass; }

	float getMaxSpeed() const { return maxSpeed; }
	float getMaxForce() const { return maxForce; }
	
	void setMass(float newMass) { mass = newMass; }
	void setMaxSpeed(float speed) { maxSpeed = speed; }
	void setMaxForce(float speed) { maxForce = speed; }

	void setPosition(Tmpl8::vec2 pos) { position = pos; }
	void setVelocity(Tmpl8::vec2 vel) { velocity = vel; }

	void update(float deltaTime);

	void addForce(Tmpl8::vec2 force);

	void draw(Engine::Camera& camera, Tmpl8::Pixel color);

	// force calculations
	Tmpl8::vec2 seek(Tmpl8::vec2 pos);
	Tmpl8::vec2 flee(Tmpl8::vec2 pos);
	Tmpl8::vec2 wander(float maxSpeed, float changeSpeed);

protected:
	Tmpl8::vec2 position;
	Tmpl8::vec2 velocity;
	Tmpl8::vec2 acceleration;

	float mass = 1.f;

	float maxSpeed = 0.f;
	float maxForce = 0.f;




	const siv::PerlinNoise perlinX;
	const siv::PerlinNoise perlinY;
	float time{ 0.f };
};