#include "Soul.hpp"
#include "Camera.hpp"

void Soul::chooseBehavior(const Tilemap& map, const Player& player, std::vector<Soul>& soulList, std::vector<Devourer>& devourerList)
{
	// ----     Flee - SeekPlayer - AvoidNeigbours - Wander    ----

	// see devourer will flee or see player with active vacuum
	for (auto& devourer : devourerList)
	{
		if (!map.lineSegmentCollide(position, devourer.getPosition()))
		{
			fleePosition = devourer.getPosition();
			currentState = BehaviorState::Flee;
			return;
		}
	}

	
	if (player.isVacuumEnabled() && !map.lineSegmentCollide(position, player.getPosition()))
	{
		fleePosition = player.getPosition();
		currentState = BehaviorState::Flee;
		return;
	}

	// player too far away movetoward the player
	float playerDist = (player.getPosition() - getPosition()).length();
	if ((currentState == BehaviorState::SeekPlayer && playerDist > minPlayerDistance) || playerDist > maxPlayerDistance)
	{
		playerPosition = player.getPosition();
		currentState = BehaviorState::SeekPlayer;
		return;
	}

	// check neigbours and player move away from the closest one
	float distance = INFINITY;

	for (auto& soul : soulList)
	{
		if (&soul != this && (soul.getPosition()-getPosition()).sqrLentgh() < distance)
		{	
			distance = (soul.getPosition() - getPosition()).sqrLentgh();
			neigboursPosition = soul.getPosition();
		}
	}
	
	if (sqrt(distance) < seekRadius)
	{
		currentState = BehaviorState::AvoidNeigbours;
		return;
	}

	// wander
	currentState = BehaviorState::Wandering;

}

void Soul::actBehavior(float deltaTime)
{
	Tmpl8::vec2 force = Tmpl8::vec2(0.f);


	switch (currentState)
	{
	case Soul::BehaviorState::Flee:
		force = flee(fleePosition);
		break;
	case Soul::BehaviorState::SeekPlayer:
		force = seek(playerPosition);
		break;
	case Soul::BehaviorState::AvoidNeigbours:
		force = flee(neigboursPosition);
		break;
	case Soul::BehaviorState::Wandering:
		force = wander(wanderStrength);
		break;
	default:
		break;
	}


	
	addForce(force);
}

void Soul::applyVacuumForce(const Player& player)
{
	Tmpl8::vec2 force = player.calculateVacuumForce(getPosition());

	acceleration += Tmpl8::vec2(force.x / mass, force.y / mass); // we ignore the max force restriction

	beingVacuumed = (force.x != 0.f || force.y != 0.f);
}

void Soul::update(float deltaTime, const Player& player)
{
	actBehavior(deltaTime);

	applyVacuumForce(player);

	switch (currentState)
	{
	case Soul::BehaviorState::Flee:
		setMaxSpeed(fleeMaxSpeed);
		break;
	case Soul::BehaviorState::SeekPlayer:
	case Soul::BehaviorState::AvoidNeigbours:
	case Soul::BehaviorState::Wandering:
	default:
		setMaxSpeed(defaultMaxSpeed);
		break;
	}


	SimpleMovement::update(deltaTime);

	particleSystem->setPosition(position);
	particleSystem->updateParticles(deltaTime);
}

void Soul::draw(Engine::Camera& camera, bool debug)
{
	particleSystem->renderParticles(camera);



	if (debug)
	{
		Tmpl8::vec2 local = camera.worldToLocal(getPosition() + Tmpl8::vec2(-20, -20));
		//camera.drawText(std::format("State: {} ", statesString[static_cast<int>(currentState)]), local.x, local.y, 0xffffff);

		// collision circle
		camera.drawCircleWorldSpace(position, collideRadius, 0x00ffff, 10);


		// todo other saved positions

		camera.drawCircleWorldSpace(position, minPlayerDistance, 0xffff00);
		camera.drawCircleWorldSpace(position, maxPlayerDistance, 0xffff00);

	}
}
