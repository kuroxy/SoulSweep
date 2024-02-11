#include "Soul.hpp"
#include "Camera.hpp"
#include "Player.hpp"



void Soul::applyForce(const Tmpl8::vec2& force)
{
	Tmpl8::vec2 clampedForce = force.normalized() * Tmpl8::Min<float>(m_maxForce, force.length());
	m_acceleration += Tmpl8::vec2(clampedForce.x/m_mass, clampedForce.y/m_mass);
	
}



void Soul::update(float dt)
{
	bool vacuumed = vacuumUpdate();
	
	float maxVel = vacuumed ? m_maxSpeedVacuumed : m_maxSpeed;


	m_velocity += m_acceleration* dt;
	m_velocity = m_velocity.normalized() * Tmpl8::Min<float>(maxVel, m_velocity.length());
	
	m_position += m_velocity* dt;
	if (m_velocity.x != 0.f && m_velocity.y != 0.f)
		m_orientation = m_velocity.normalized();

	m_acceleration=Tmpl8::vec2(0.0f);

}

void Soul::actionSelection()
{
	// prio 1: collision avoidance
	// prio 2: flee from monster (if in view)
	// prio 3: flee from player if soul has seen the player activate their vacuum (if in view)
	// prio 4: wander+separation
	
	//applyForce(flee(fleepos));
	applyForce(wander());

}

bool Soul::vacuumUpdate()
{
	Tmpl8::vec2 force = m_playerRef.calculateVacuumForce(m_position);
	
	m_acceleration += Tmpl8::vec2(force.x / m_mass, force.y / m_mass); // we ignore the max force restriction

	return (force.x != 0.f || force.y != 0.f);

}

Tmpl8::vec2 Soul::wander()
{
	float random = (Rand(2.f) - 1) / 2;
	m_currentWanderAngle -= random* m_wanderRate;
	

	return (m_orientation * (2.f * m_wanderingStrength) + Tmpl8::vec2((float)cos(m_currentWanderAngle), (float)sin(m_currentWanderAngle))* m_wanderingStrength).normalized()*m_maxForce;
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
	c.drawCircleWorldSpace(m_position, m_debugSize, 0x00ffff, 10); // body

	c.drawLineWorldSpace(m_position, m_position + m_orientation * 5, 0x00ffff); // direction line

	if (debug)
	{
		// wanderCircles
		//c.drawCircleWorldSpace(m_position + m_orientation * (2 * m_wanderingStrength), m_wanderingStrength, 0xff00ff, 20);

		//c.drawCircleWorldSpace(m_position + m_orientation * (2.f * m_wanderingStrength) + Tmpl8::vec2((float)cos(m_currentWanderAngle), (float)sin(m_currentWanderAngle)) * m_wanderingStrength, 2.f, 0xffffff, 10);
	
		// vacuumRange
		//Tmpl8::Pixel color = m_playerRef.vacuumRange(m_position) ? 0x00ff00 : 0xff0000;
		//c.drawLineWorldSpace(m_playerRef.getPosition(), m_position, color); 
	


	}



}

