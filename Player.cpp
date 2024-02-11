#include "Player.hpp"
#include "Camera.hpp"
#include "Tilemap.hpp"
#include "InputManager.hpp"

using namespace Tmpl8;




void Player::handleInput(const Engine::InputManager& im)
{
	m_direction = vec2(0);
	if (im.isActionPressed("up"))
		m_direction.y -= 1.f;
	if (im.isActionPressed("down"))
		m_direction.y += 1.f;
	if (im.isActionPressed("left"))
		m_direction.x -= 1.f;
	if (im.isActionPressed("right"))
		m_direction.x += 1.f;

	m_vacuumEnabled = im.isActionPressed("vacuum");
	m_vacuumDirection = (im.getWorldMouse() - m_position).normalized();

}

void Player::move(float deltaTime)
{
	float length = m_direction.length();
	if (length == 0)
		return;

	m_position.x += m_direction.x / length * deltaTime * m_playerSpeed;
	m_position.y += m_direction.y / length * deltaTime * m_playerSpeed;
	updateAABB();
}

void Player::update(float deltaTime, const Tilemap& tilemap)
{
	move(deltaTime);

	if (tilemap.boxCollide(m_collisionBox))
	{
		// there is a collisions
		m_position+= tilemap.resolveBoxCollision(m_collisionBox, m_direction);
		updateAABB();
	}

	if (tilemap.boxCollide(m_collisionBox)) // double collision check
	{
		m_position += tilemap.resolveBoxCollision(m_collisionBox, m_direction);
		updateAABB();
	}

}


bool Player::vacuumRange(const Tmpl8::vec2 pos) const
{
	vec2 difference = pos - m_position;
	return (difference.normalized().dot(m_vacuumDirection) > m_vacuumCone && difference.length() < m_maxVacuumDistance);
}

const Tmpl8::vec2& Player::calculateVacuumForce(const Tmpl8::vec2 pos) const
{
	vec2 difference = m_position-pos;
	vec2 forceDirection = difference.normalized();
	float length = difference.length();

	// reversed bc it is soul->player instead of player->soul
	if (m_vacuumEnabled == false || forceDirection.dot(-m_vacuumDirection) < m_vacuumCone || length > m_maxVacuumDistance)
	{
		return vec2(0);
	}

	// using linear for now easiest.
	return forceDirection * (m_maxVacuumForce * (length - m_maxVacuumDistance) / -m_maxVacuumDistance);

}


void Player::draw(Engine::Camera& camera, bool debug)
{
	if (debug)
		m_collisionBox.draw(camera, 0xff00ff);
	
	Tmpl8::Pixel vacuumLine = m_vacuumEnabled ? 0x00ff00 : 0xff0000;
	camera.drawLineWorldSpace(m_collisionBox.center(), m_collisionBox.center() + m_vacuumDirection * m_maxVacuumDistance, vacuumLine);
	//draw sprite stuff
}

void Player::updateAABB()
{
	vec2 half = vec2(m_width / 2, m_height / 2);
	m_collisionBox.min = m_position - half;
	m_collisionBox.max = m_position + half;
}
