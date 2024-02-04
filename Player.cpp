#include "Player.hpp"
#include "Config.hpp"
#include "Camera.hpp"

using namespace Tmpl8;

Player::Player(const Tmpl8::vec2& pos, float radius)
	: m_position{ pos }
	, m_direction{ 0 }
	, m_collisionRadius{ radius }
	, m_playerSpeed{ Config::playerSpeed }
{
}

const Tmpl8::vec2& Player::getPosition() const
{
	return m_position;
}

void Player::setPosition(const Tmpl8::vec2& pos)
{
	m_position = pos;
}

void Player::move(const Tmpl8::vec2& direction, float deltaTime)
{
	float length = direction.length();
	if (length == 0)
		return;

	m_position.x += direction.x / length * deltaTime * m_playerSpeed;
	m_position.y += direction.y / length * deltaTime * m_playerSpeed;
}


float Player::getRadius() const
{
	return m_collisionRadius;
}

void Player::draw(Engine::Camera& camera, bool debug)
{
	if (debug)
		camera.drawCircleWorldSpace(m_position, m_collisionRadius, 0xff00ff, 20);

	//draw sprite stuff
}
