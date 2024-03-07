#pragma once
#include "template.h"
#include "surface.h"
#include "Player.hpp"
#include "Tilemap.hpp"
#include "ParticleSystem.hpp"

namespace Engine
{
	class Camera;
}


class Soul
{
public:
	Soul(const Tmpl8::vec2& pos, const Engine::ParticleSystemParams& particleParams)
		: m_position{ pos }
	, particleSystem{ std::make_unique<Engine::BaseParticleSystem>(particleParams, 100)}
	{
	}

	void setVelocity(const Tmpl8::vec2& velocity) { m_velocity = velocity; }
	
	const Tmpl8::vec2 getPosition() const { return m_position; };

	float getCollectRadius() const { return m_collectRadius; }

	void applyForce(const Tmpl8::vec2& force); // maybe can be private

	void update(float dt, const Player& player);

	void actionSelection();

	void draw(Engine::Camera& c, bool debug=false);

	bool isEaten = false;
private:

	Tmpl8::vec2 wander();
	Tmpl8::vec2 seek(Tmpl8::vec2 pos);
	Tmpl8::vec2 flee(Tmpl8::vec2 pos);
	bool vacuumUpdate(const Player& player);

	Tmpl8::vec2 m_position{ 0 };
	Tmpl8::vec2 m_velocity{ 0 };
	Tmpl8::vec2 m_acceleration{ 0 };

	Tmpl8::vec2 m_orientation{ 1,0 };

	float m_mass{ 1.f };
	float m_maxForce{ 20.f };
	float m_maxSpeed{ 30.f };
	float m_maxSpeedVacuumed{ 1000.f };

	float m_collectRadius{ 10.f };

	// wander
	float m_wanderingStrength{ 100.f };
	float m_currentWanderAngle{ Rand(2*Tmpl8::PI)};
	float m_wanderRate{ .5f };


	

	// visuals
	std::unique_ptr<Engine::BaseParticleSystem> particleSystem;

};

