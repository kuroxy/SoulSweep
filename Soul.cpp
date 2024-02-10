#include "Soul.hpp"
#include "Camera.hpp"


Soul::Soul(const Tmpl8::vec2& pos)
	: m_position{ pos }
{
}



void Soul::applyForce(const Tmpl8::vec2& force)
{
	Tmpl8::vec2 clampedForce = force.normalized() * Tmpl8::Min<float>(m_maxForce, force.length());
	m_acceleration += Tmpl8::vec2(clampedForce.x/m_mass, clampedForce.y/m_mass);
}



void Soul::update(float dt)
{

	m_velocity += m_acceleration* dt;
	m_velocity = m_velocity.normalized() * Tmpl8::Min<float>(m_maxForce, m_velocity.length());
	
	m_position += m_velocity* dt;
	if (m_velocity.x != 0.f && m_velocity.y != 0.f)
		m_orientation = m_velocity.normalized();

	m_acceleration=Tmpl8::vec2(0.0f);

}

void Soul::actionSelection(Tmpl8::vec2 fleepos)
{
	// prio 1: collision avoidance
	// prio 2: flee from monster (if in view)
	// prio 3: flee from player if soul has seen the player activate their vacuum (if in view)
	// prio 4: wander+separation
	
	//applyForce(flee(fleepos));
	applyForce(wander());

}

Tmpl8::vec2 Soul::wander()
{
	float random = (Rand(2.f) - 1) / 2;
	m_currentWanderAngle -= random* m_wanderRate;
	

	return (m_orientation * (2.f * m_wanderingStrength) + Tmpl8::vec2(cos(m_currentWanderAngle), sin(m_currentWanderAngle))* m_wanderingStrength).normalized()*m_maxForce;
}

Tmpl8::vec2 Soul::seek(Tmpl8::vec2 pos)
{
	return (pos - m_position).normalized() * m_maxForce;
}

Tmpl8::vec2 Soul::flee(Tmpl8::vec2 pos)
{
	return -seek(pos);
}




void Soul::draw(Engine::Camera& c, bool debug)
{
	c.drawCircleWorldSpace(m_position, m_debugSize, 0x00ffff, 10);

	c.drawLineWorldSpace(m_position, m_position + m_orientation * 5, 0x00ffff);

	if (debug)
	{
		c.drawCircleWorldSpace(m_position + m_orientation * (2 * m_wanderingStrength), m_wanderingStrength, 0xff00ff, 20);

		c.drawCircleWorldSpace(m_position + m_orientation * (2.f * m_wanderingStrength) + Tmpl8::vec2(cos(m_currentWanderAngle), sin(m_currentWanderAngle)) * m_wanderingStrength, 2.f, 0xffffff, 10);
	}

}

