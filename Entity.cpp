#include "Entity.hpp"
#include "Camera.hpp"

Entity::Entity(const Tmpl8::vec2& pos, float radius)
	: m_position{pos}
	, m_collisionRadius{radius}
{
}

const Tmpl8::vec2& Entity::getPosition() const
{
	return m_position;
}

void Entity::setPosition(const Tmpl8::vec2& pos)
{
}

float Entity::getRadius() const
{
	return m_collisionRadius;
}

void Entity::setRadius(float radius)
{
	m_collisionRadius = radius;
}

bool Entity::collides(const Entity& o) const
{
	float distsq = (m_position - o.m_position).sqrLentgh();

	return distsq <= (m_collisionRadius* m_collisionRadius + o.m_collisionRadius* o.m_collisionRadius);
}

void Entity::debugDraw(Engine::Camera& camera, Tmpl8::Pixel c)
{
	camera.drawCircleWorldSpace(m_position, m_collisionRadius, c, 20);
}

void Entity::draw(Engine::Camera& camera, bool debug)
{
	if (debug)
		debugDraw(camera, 0xff00ff);
}


