#pragma once
#include "template.h"
#include "surface.h"

namespace Engine
{
	class Camera;
}



class Soul
{
public:
	Soul(const Tmpl8::vec2& pos);

	void applyForce(const Tmpl8::vec2& force);
	void update(float dt);

	void actionSelection(Tmpl8::vec2 fleepos);
	Tmpl8::vec2 wander();
	Tmpl8::vec2 seek(Tmpl8::vec2 pos);
	Tmpl8::vec2 flee(Tmpl8::vec2 pos);

	void draw(Engine::Camera& c, bool debug=false);


private:
	Tmpl8::vec2 m_position{ 0 };
	Tmpl8::vec2 m_velocity{ 0 };
	Tmpl8::vec2 m_acceleration{ 0 };

	Tmpl8::vec2 m_orientation{ 1,0 };

	float m_mass{ 1.f };
	float m_maxForce{ 20.f };
	float m_maxSpeed{ 50.f };

	float m_debugSize{ 10.f };

	// wander
	float m_wanderingStrength{ 100.f };
	float m_currentWanderAngle{ 0.f };
	float m_wanderRate{ .3f };
};

