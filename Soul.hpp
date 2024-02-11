#pragma once
#include "template.h"
#include "surface.h"
#include "Player.hpp"
#include "Tilemap.hpp"

namespace Engine
{
	class Camera;
}

class Player;


class Soul
{
public:
	Soul(const Tmpl8::vec2& pos, const Player& player, const Tilemap& tilemap)
		: m_position{ pos }
		, m_playerRef{ player }
		, m_tileMapRef{ tilemap }
	{
	}

	const Tmpl8::vec2 getPosition() const { return m_position; };

	void applyForce(const Tmpl8::vec2& force); // maybe can be private

	void update(float dt);

	void actionSelection();

	void draw(Engine::Camera& c, bool debug=false);


private:

	Tmpl8::vec2 wander();
	Tmpl8::vec2 seek(Tmpl8::vec2 pos);
	Tmpl8::vec2 flee(Tmpl8::vec2 pos);
	bool vacuumUpdate(); // bit scuffed maybe looking later if I can clean this function up

	Tmpl8::vec2 m_position{ 0 };
	Tmpl8::vec2 m_velocity{ 0 };
	Tmpl8::vec2 m_acceleration{ 0 };

	Tmpl8::vec2 m_orientation{ 1,0 };

	float m_mass{ 1.f };
	float m_maxForce{ 20.f };
	float m_maxSpeed{ 40.f };
	float m_maxSpeedVacuumed{ 1000.f };

	float m_debugSize{ 10.f };

	// wander
	float m_wanderingStrength{ 100.f };
	float m_currentWanderAngle{ 0.f };
	float m_wanderRate{ .3f };

	const Player& m_playerRef; // this <-- is it ok?
	const Tilemap& m_tileMapRef;



};

