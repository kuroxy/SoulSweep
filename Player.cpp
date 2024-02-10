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




void Player::draw(Engine::Camera& camera, bool debug)
{
	if (debug)
		m_collisionBox.draw(camera, 0xff00ff);
		
	//draw sprite stuff
}

void Player::updateAABB()
{
	m_collisionBox.min = m_position;
	m_collisionBox.max = m_position + vec2(m_width, m_height);
}
