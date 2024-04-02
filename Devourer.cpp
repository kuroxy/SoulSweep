#include "Devourer.hpp"
#include "Soul.hpp"
#include "Camera.hpp"
#include <format>
#include "Level.hpp"

void Devourer::chooseBehavior(const Level& level , const Player& player, std::vector<Soul>& soulList)
{
	// prettify this <---------------------------------------------------------
	// this will check if it is still consuming 
	if (consumingSoulPtr != nullptr)
	{
		currentState = BehaviorState::ConsumingSoul;
		return;
	}


	//check if collision with player
	// if it collides we just want is to have no behaviour this is similar to consuming since this is just not moving for a certain time but we keep it indefinetly
	float distsq = (position - player.getPosition()).sqrLentgh();
	if (distsq < (collideRadius + player.getCollectRadius()) * (collideRadius + player.getCollectRadius()))
	{
		currentState = BehaviorState::ConsumingSoul;
		consumingSoulPtr = nullptr;
		return;
	}

	// if it collides with a soul it just goes into comsuming
	for (auto& soul : soulList)
	{
		// collision detection
		if (!soul.isEaten && collideWithSoul(soul))
		{
			currentState = BehaviorState::ConsumingSoul;
			consumingSoulPtr = &soul;
			return;
		}

	}

	// chasing soul

	float distance = INFINITY;

	for (auto& soul : soulList)
	{
		// we check if there is a direct line of sight ,,, currently there is no max view distance for the devourer
		if (!level.lineSegmentCollision(position, soul.getPosition()))
		{
			

			float curr = (soul.getPosition() - getPosition()).sqrLentgh();
			if (curr < distance)
			{
				currentState = BehaviorState::ChasingSoul;
				newSoulPosition = true;

				newPlayerPosition = false;
				soulPosition = soul.getPosition();
				distance = curr;
			}
			

		}

	}


	// chasing player
	float playerDistsqr = (player.getPosition() - getPosition()).sqrLentgh();
	if (!level.lineSegmentCollision(position, player.getPosition()) || playerDistsqr > maxPlayerDistance * maxPlayerDistance)
	{
		if (playerDistsqr < distance || playerDistsqr > maxPlayerDistance * maxPlayerDistance)
		{
			currentState = BehaviorState::ChasingPlayer;
			newPlayerPosition = true;
			newSoulPosition = false;
		}
		
		playerPosition = player.getPosition();
		return;

	}


	// chase last seen soul position
	

	if (newSoulPosition)
	{
		if ((position - soulPosition).length() < seekRadius) // if the distance is smaller so it is inside the seek radius is has been visited
			newSoulPosition = false;

		currentState = BehaviorState::ChasingOldSoul;
		return;
	}

	if (newPlayerPosition)
	{
		if ((position - playerPosition).length() < seekRadius)
			newPlayerPosition = false;

		currentState = BehaviorState::ChasingOldPlayer;
		return;
	}

	currentState = BehaviorState::Wandering;

}

void Devourer::actBehavior(float deltaTime)
{
	Tmpl8::vec2 force = Tmpl8::vec2(0.f);


	switch (currentState)
	{
	case Devourer::BehaviorState::ConsumingSoul:
		setVelocity({ 0.f });

		if (consumingSoulPtr == nullptr)
			break;

		consumingSoulPtr->setVelocity({ 0.f });
		consumingSoulProgress += deltaTime;

		if (consumingSoulProgress > consumingSoulTime) // it is done eating
		{
			consumingSoulPtr->isEaten = true;
			consumingSoulProgress = 0.f;
			consumingSoulPtr = nullptr;

		}

		break;
	case Devourer::BehaviorState::ChasingSoul:
		force = seek(soulPosition);

		break;
	case Devourer::BehaviorState::ChasingPlayer:
		force = seek(playerPosition);
		break;
	case Devourer::BehaviorState::ChasingOldSoul:
		force = seek(soulPosition);
		break;
	case Devourer::BehaviorState::ChasingOldPlayer:
		force = seek(playerPosition);
		break;
	case Devourer::BehaviorState::Wandering:
		force = wander(wanderStrength);
		break;
	default:
		break;
	}


	//printf("x: %f, y:%f. Vel x:%f y:%f\n", force.x, force.y, velocity.x, velocity.y);
	SimpleMovement::addForce(force);
}

bool Devourer::collideWithSoul(const Soul& soul) const
{
	float distsq = (position - soul.getPosition()).sqrLentgh();
	return distsq < (collideRadius+soul.getCollisionRadius())*(collideRadius + soul.getCollisionRadius());
}

void Devourer::update(float deltaTime)
{
	actBehavior(deltaTime);

	SimpleMovement::update(deltaTime);
	
	particleSystem->setPosition(position);
	particleSystem->updateParticles(deltaTime);

}

void Devourer::draw(Engine::Camera& camera, bool debug)
{

	particleSystem->renderParticles(camera);

	

	if (debug)
	{
		Tmpl8::vec2 local = camera.worldToLocal(getPosition() + Tmpl8::vec2(-20, -20));
		//camera.drawText(std::format("State: {} ", statesString[static_cast<int>(currentState)]), local.x, local.y, 0xffffff);

		// collision circle
		camera.drawCircleWorldSpace(position, collideRadius, 0x00ffff, 10); 


		Tmpl8::Pixel soulLineColor = newSoulPosition ? 0x00ff00 : 0xff0000;
		Tmpl8::Pixel playerLineColor = newPlayerPosition ? 0x00ff00 : 0xff0000;

		camera.drawLineWorldSpace(position, soulPosition, soulLineColor);
		camera.drawCircleWorldSpace(soulPosition, seekRadius, soulLineColor);

		camera.drawLineWorldSpace(position, playerPosition, playerLineColor);
		camera.drawCircleWorldSpace(playerPosition, seekRadius, playerLineColor);


		camera.drawCircleWorldSpace(position, maxPlayerDistance, 0xffff00);


	}



}


