#include "Soul.hpp"
#include "Camera.hpp"
#include "Level.hpp"

void Soul::chooseBehavior(const Level& level, const Player& player, std::vector<Soul>& soulList, std::vector<Devourer>& devourerList)
{
	// ----     Flee - SeekPlayer - AvoidNeigbours - Wander    ----

	// see devourer will flee or see player with active vacuum
	for (auto& devourer : devourerList)
	{
		if (!level.lineSegmentCollision(position, devourer.getPosition()))
		{
			fleePosition = devourer.getPosition();
			setFlee();
			return;
		}
	}

	
	if (player.isVacuumEnabled() && !level.lineSegmentCollision(position, player.getPosition()))
	{
		fleePosition = player.getPosition();
		setFlee();
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
		setAvoidNeighbours();
		return;
	}

	// if nothing else we will wander;
	setWandering();

}

void Soul::actBehavior(float deltaTime)
{
	Tmpl8::vec2 force = Tmpl8::vec2(0.f);


	switch (currentState)
	{
	case Soul::BehaviorState::Flee:
		force = flee(fleePosition);
		break;
	case Soul::BehaviorState::AvoidNeigbours:
		force = flee(neigboursPosition);
		break;
	case Soul::BehaviorState::Wandering:
		currentWanderTime -= deltaTime;
		if (currentWanderTime <= 0.f || (wanderPosition - getPosition()).length() < wanderPointDistance)
			generateWanderingPoint();

		force = seek(wanderPosition);
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

void Soul::draw(Engine::Camera& camera)
{
	particleSystem->drawParticles(camera);
}

void Soul::drawDebug(Engine::Camera& camera)
{
	// collision circle
	camera.drawCircle(position, collideRadius, 0x00ffff, 1);

	camera.drawCircle(fleePosition, 1, 0xff0000); // flee position
	camera.drawCircle(neigboursPosition, 1, 0xffff00); // neighbour pos
	camera.drawCircle(playerPosition, 1, 0x00ff00);
	camera.drawCircle(wanderPosition, wanderPointDistance, 0x0000ff, 2);
	camera.drawLine(getPosition(), wanderPosition, 0x0000ff);
}

void Soul::setFlee() 
{

	// no special behaviour
	switch (currentState)
	{
	case Soul::BehaviorState::Flee:
		break;
	case Soul::BehaviorState::AvoidNeigbours:
		break;
	case Soul::BehaviorState::Wandering:
		break;
	default:
		break;
	}


	currentState = BehaviorState::Flee;
}

void Soul::setAvoidNeighbours()
{
	switch (currentState)
	{
	case Soul::BehaviorState::Flee:
		break;
	case Soul::BehaviorState::AvoidNeigbours:
		break;
	case Soul::BehaviorState::Wandering:
		break;
	default:
		break;
	}

	currentState = BehaviorState::AvoidNeigbours;
}

void Soul::setWandering()
{
	switch (currentState)
	{
	case Soul::BehaviorState::Flee:
		// only here special behaviour, when getting out of the flee state we generate a new wanderpoint
		generateWanderingPoint();
		break;
	case Soul::BehaviorState::AvoidNeigbours:
		break;
	case Soul::BehaviorState::Wandering:
		break;
	default:
		break;
	}

	currentState = BehaviorState::Wandering;
}

void Soul::generateWanderingPoint()
{
	// Generates a point within the the wanderArea (which is the level area)
	wanderPosition = Tmpl8::vec2(Rand(wanderArea.max.x - wanderArea.min.x) + wanderArea.min.x, Rand(wanderArea.max.y - wanderArea.min.y) + wanderArea.min.y);
	currentWanderTime = wanderTime;

}
