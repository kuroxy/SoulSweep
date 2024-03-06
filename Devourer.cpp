#include "Devourer.hpp"
#include "Soul.hpp"
#include "Camera.hpp"

void Devourer::chooseBehavior(const Tilemap& map, const Player& player, const std::vector<Soul>& soulList)
{
	// prettify this <---------------------------------------------------------
	// this will check if it is still consuming 
	if (consumingSoulPtr != nullptr)
	{
		currentState = BehaviorState::ConsumingSoul;
		return;
	}

	// if it collides with a soul it just goes into comsuming
	for (auto& soul : soulList)
	{
		// collision detection
		if (collideWithSoul(soul))
		{
			currentState = BehaviorState::ConsumingSoul;
			return;
		}

	}

	// we don't need to check collision with player because that is not a state and the game will handle game over etc..

	// chasing soul

	for (auto& soul : soulList)
	{
		// we check if there is a direct line of sight ,,, currently there is no max view distance for the devourer
		if (!map.lineSegmentCollide(position, soul.getPosition()))
		{
			currentState = BehaviorState::ChasingSoul;
			newSoulPosition = true;

			newPlayerPosition = false;
			soulPosition = soul.getPosition();
			return;

		}

	}

	// chasing player
	if (!map.lineSegmentCollide(position, player.getPosition()))
	{
		currentState = BehaviorState::ChasingPlayer;
		newPlayerPosition = true;
		newSoulPosition = false;
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

void Devourer::actBehavior()
{
	Tmpl8::vec2 force = Tmpl8::vec2(0.f);


	switch (currentState)
	{
	case Devourer::BehaviorState::ConsumingSoul:
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
		break;
	default:
		break;
	}



	SimpleMovement::addForce(force);
}

bool Devourer::collideWithSoul(const Soul& soul) const
{
	float distsq = (position - soul.getPosition()).sqrLentgh();
	return distsq < (collideRadius+soul.getCollectRadius());
}

void Devourer::update(float deltaTime)
{
	
	SimpleMovement::update(deltaTime);
	
	particleSystem->setPosition(position);
	particleSystem->updateParticles(deltaTime);

}

void Devourer::draw(Engine::Camera& camera, bool debug)
{

	particleSystem->renderParticles(camera);

	if (debug)
	{
		// collision circle
		camera.drawCircleWorldSpace(position, collideRadius, 0x00ffff, 10); 


		Tmpl8::Pixel soulLineColor = newSoulPosition ? 0x00ff00 : 0xff0000;
		Tmpl8::Pixel playerLineColor = newPlayerPosition ? 0x00ff00 : 0xff0000;

		camera.drawLineWorldSpace(position, soulPosition, soulLineColor);
		camera.drawCircleWorldSpace(soulPosition, seekRadius, soulLineColor);

		camera.drawLineWorldSpace(position, playerPosition, playerLineColor);
		camera.drawCircleWorldSpace(playerPosition, seekRadius, playerLineColor);

	}



}


