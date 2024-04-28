#include "Player.hpp"
#include "Camera.hpp"
#include "InputManager.hpp"
#include <format>
#include <string>
#include "Level.hpp"


using namespace Tmpl8;




void Player::handleInput(const Engine::InputManager& im)
{
	// if player is dead we will ignore player input
	if (dead)
		return;


	m_vacuumEnabled = im.isActionHeld("vacuum") && !im.isActionHeld("dropsoul");
	m_dropSoul = im.isActionPressed("dropsoul");
	m_vacuumDirection = (im.getWorldMouse() - m_position).normalized();


	

	if (im.isActionPressed("space") && dashResource >= dashCost && currentDashDuration <= 0.f)
	{
		dashResource -= dashCost;
		currentDashDuration = dashDuration;

		dashDirection = (im.getWorldMouse() - m_position).normalized();
		return;	// If we dash other info will be ignored so we can return
	}


	moveDirection = vec2(0.f);
	
	if (im.isActionHeld("up"))
		moveDirection.y -= 1.f;
	if (im.isActionHeld("down"))
		moveDirection.y += 1.f;
	if (im.isActionHeld("left"))
		moveDirection.x -= 1.f;
	if (im.isActionHeld("right"))
		moveDirection.x += 1.f;

	moveDirection.normalize();
}

void Player::update(float deltaTime)
{
	// if the player is dead just or the animation is not finished
	// change animation to dead (this will not reset the animation)
	if (dead)
	{
		anim.changeAnimation("death");
		anim.update(deltaTime);
		return;
	}

	// Dash

	dashResource += dashRechargeSpeed * deltaTime;
	dashResource = std::clamp(dashResource, 0.f, 1.f);
	if (currentDashDuration > 0.f)
		currentDashDuration -= deltaTime;


	// Vacuum Particles

	vacuumParticles.setPosition(m_collisionBox.center() + m_vacuumDirection * m_maxVacuumDistance);
	vacuumParticles.setAttractorPostion(m_collisionBox.center());

	vacuumParticles.setAttractorStrenth(0.f);
	
	if (m_vacuumEnabled)
	{
		vacuumParticles.setAttractorStrenth(30000.f);
		timeTillNextSpawn -= deltaTime;
		if (timeTillNextSpawn <= 0.f)
		{
			timeTillNextSpawn = spawnTime;
			vacuumParticles.spawnParticle();
		}
	}

	for (auto& p : vacuumParticles.getParticles())
	{
		if (!p.enabled)
			continue;

		Tmpl8::vec2 direction = (m_collisionBox.center() - p.position);

		// if moves to far away from the 
		if ((direction.normalized().dot(p.velocity.normalized()) < .5 && p.velocity.length() > 1.f) || direction.length() < 30.f)
		{
			p.currentLifeTime = 0.f;
			p.enabled = false;
		}

	}

	vacuumParticles.updateParticles(deltaTime);


	// Movement
	velocity = Tmpl8::vec2(0.f);

	if (currentDashDuration > 0.f)
		velocity = dashDirection * dashSpeed;
	else
		velocity = moveDirection * m_playerSpeed;

	m_position += velocity * deltaTime;
	updateAABB();


	// update animations

	anim.update(deltaTime);

	if (velocity.sqrLentgh() < .001f)
		anim.changeAnimation("idle");
	else
	{
		anim.changeAnimation("walk");

		if (velocity.x > 0.f)
			flipCharacter = false;
		else if (velocity.x < 0.f)
			flipCharacter = true;
	}


}

void Player::checkCollisions(std::vector<Engine::AABB> colliders)
{
	Engine::AABB aabb = getAABB();
	Tmpl8::vec2 vel = getVelocity();

	// Check if the player is colliding with any of the colliders.
	for (auto& collider : colliders)
	{
		if (!aabb.intersect(collider)) continue; // Optimize collisions.

		if (fabs(vel.x) > fabs(vel.y))
		{
			checkHorizontalCollisions(collider, aabb, vel);
			checkVerticalCollisions(collider, aabb, vel);
		}
		else
		{
			checkVerticalCollisions(collider, aabb, vel);
			checkHorizontalCollisions(collider, aabb, vel);
		}

	}

	setPosition(aabb.min+ vec2(m_width / 2, m_height / 2));
	velocity = vel;
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

void Player::draw(Engine::Camera& camera)
{	
	anim.draw(camera, m_position - Tmpl8::vec2(sprites.getSpriteWidth() / 2.f, sprites.getSpriteHeight() / 2.f+5.f), flipCharacter);
	// +5 is offset for the sprite to align the feet with the bottom of the hitbox

	vacuumParticles.renderParticles(camera);
}

void Player::drawDebug(Engine::Camera& camera)
{
	m_collisionBox.draw(camera, 0xff00ff);
	camera.drawCircle(m_position, m_collectRadius, 0xffffff, 1);
	Tmpl8::Pixel vacuumLine = m_vacuumEnabled ? 0x00ff00 : m_dropSoul ? 0x0000ff : 0xff0000;
	camera.drawLine(m_collisionBox.center(), m_collisionBox.center() + m_vacuumDirection * m_maxVacuumDistance, vacuumLine);
}

bool Player::checkVerticalCollisions(const Engine::AABB& collider, Engine::AABB& aabb, Tmpl8::vec2& vel) const
{
	// Written by Jeremiah
	// https://github.com/jpvanoosten/tmpl8/blob/Physics_Test
	// Explained in the discord events

	if (vel.y > 0) // Moving down...
	{
		if (aabb.intersect(collider.topEdge().shrink(3)))
		{
			// Compute intersection.
			const float overlap = collider.min.y - aabb.max.y;
			aabb += vec2{ 0, overlap };

			vel.y = 0.0f;

			return true;
		}
	}
	else if (vel.y < 0) // Moving up
	{
		if (aabb.intersect(collider.bottomEdge().shrink(3)))
		{
			// Compute intersection.
			const float overlap = collider.max.y - aabb.min.y;
			aabb += vec2{ 0, overlap };

			vel.y = 0.0f;
			return true;
		}
	}

	return false;
}

bool Player::checkHorizontalCollisions(const Engine::AABB& collider, Engine::AABB& aabb, Tmpl8::vec2& vel) const
{
	// Written by Jeremiah
	// https://github.com/jpvanoosten/tmpl8/blob/Physics_Test
	// Explained in the discord events

	if (vel.x > 0) // Moving Right...
	{
		if (aabb.intersect(collider.leftEdge().shrink(3)))
		{
			// Compute intersection.
			const float overlap = collider.min.x - aabb.max.x;
			aabb += vec2{ overlap, 0 };

			vel.x = 0.0f;
			return true;
		}
	}
	else if (vel.x < 0) // Moving left...
	{
		if (aabb.intersect(collider.rightEdge().shrink(3)))
		{
			// Compute intersection.
			const float overlap = collider.max.x - aabb.min.x;
			aabb += vec2{ overlap, 0 };

			vel.x = 0.0f;
			return true;
		}
	}

	return false;
}



void Player::updateAABB()
{
	vec2 half = vec2(m_width / 2, m_height / 2);
	m_collisionBox.min = m_position - half;
	m_collisionBox.max = m_position + half;
}
