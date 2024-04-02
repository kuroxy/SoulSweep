#include "Player.hpp"
#include "Camera.hpp"
#include "Tilemap.hpp"
#include "InputManager.hpp"
#include <format>
#include <string>
#include "Level.hpp"


using namespace Tmpl8;




void Player::handleInput(const Engine::InputManager& im)
{
	m_direction = vec2(0);
	if (im.isActionHeld("up"))
		m_direction.y -= 1.f;
	if (im.isActionHeld("down"))
		m_direction.y += 1.f;
	if (im.isActionHeld("left"))
		m_direction.x -= 1.f;
	if (im.isActionHeld("right"))
		m_direction.x += 1.f;

	if (m_direction.x == 0.f && m_direction.y == 0.f)
	{
		anim.changeAnimation("idle");
	}
	else
	{
		anim.changeAnimation("walk");
		flipCharacter = m_direction.x < 0.f;
	}

	


	m_vacuumEnabled = im.isActionHeld("vacuum") && !im.isActionHeld("dropsoul");
	m_dropSoul = im.isActionPressed("dropsoul");
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

void Player::update(float deltaTime, const Level& level)
{
	anim.update(deltaTime);
	move(deltaTime);

	if (level.aabbCollision(m_collisionBox))
	{
		// there is a collisions
		m_position+= level.resolveBoxCollision(m_collisionBox, m_direction);
		updateAABB();
	}

	// double collision check
	// because the first can the player into a new collider, this one resolves this one.
	if (level.aabbCollision(m_collisionBox)) 
	{
		m_position += level.resolveBoxCollision(m_collisionBox, m_direction);
		updateAABB();
	}

	// most of the times this will be enough.
	// if not player has a frame inside a collider 
	// next frame will solve this collision

}


bool Player::vacuumRange(const Tmpl8::vec2 pos) const
{
	vec2 difference = pos - m_position;
	return (difference.normalized().dot(m_vacuumDirection) > m_vacuumCone && difference.length() < m_maxVacuumDistance);
}

const Tmpl8::vec2 Player::calculateVacuumForce(const Tmpl8::vec2 pos) const
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
	{
		m_collisionBox.draw(camera, 0xff00ff);
		camera.drawCircleWorldSpace(m_position, m_collectRadius, 0xffffff);
	}
		
	anim.draw(camera, m_position - Tmpl8::vec2(sprites.getSpriteWidth() / 2, sprites.getSpriteHeight() / 2), flipCharacter);

	//camera.renderSpriteWorldSpace(&sprites, 0, m_position - Tmpl8::vec2(sprites.getSpriteWidth() / 2, sprites.getSpriteHeight() / 2));

	Tmpl8::Pixel vacuumLine = m_vacuumEnabled ? 0x00ff00 : m_dropSoul ? 0x0000ff : 0xff0000;
	camera.drawLineWorldSpace(m_collisionBox.center(), m_collisionBox.center() + m_vacuumDirection * m_maxVacuumDistance, vacuumLine);
	
	camera.drawText(std::format("Souls: {} / {}", m_currentSouls, m_maxSouls), 10, 10, 0x00ffff);
}

void Player::updateAABB()
{
	vec2 half = vec2(m_width / 2, m_height / 2);
	m_collisionBox.min = m_position - half;
	m_collisionBox.max = m_position + half;
}
