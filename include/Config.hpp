#pragma once

#include "ParticleSystem.hpp"

namespace Config
{
	// --- Game Settings ---

	constexpr int minSouls = 5;

	// -- Fog of war settings --
	constexpr float viewDistanceMin = 75.f; // distance smaller is always visible
	constexpr float viewDistanceMax = 300.f; // after this distance is dark


	// -- Devourer -- 

	constexpr float devourerMaxSpeed = 70.f;			// the max speed that devourer goes in px/s
	constexpr float devourerMaxForce = 300.f;			// how much force can be added in a single add force call.
	constexpr float devourerConsumingTime = 3.f;		// time it takes for the devourer to eat a soul. This time the devourer&soul are frozen in place.
	constexpr float devourerVisitRadius = 20.f;			// distance to a point to consider it as being visited.
	constexpr float devourerMaxPlayerDistance = 700.f;	// bigger distance with player-devourer then this var, devourer knows player position.
	constexpr float devourerWanderStrength = .5f;		// how fast wandering changes. higher more random, lower the less change over time.
	constexpr float devourerCollideRadius = 10.f;		// radius of the collision circle based on the center position, used for collision between souls and the player

	// -- Soul --

	constexpr float soulMaxForce = 100.f;				// how much force can be added in a single add force call.
	constexpr float soulVacuumSpeed = 1000.f;			// max speed souls can move while being sucked by the vacuum
	constexpr float soulFleeSpeed = 45.f;				// max speed when fleeing from a devourer or vacuum
	constexpr float soulDefaultSpeed = 50.f;			// max speed when wandering arround the map
	constexpr float soulMaxNeighbourRadius = 100.f;		// the distance before moving away from another soul, this way they don't clump on top of each other
	constexpr float soulWanderDist = 40.f;				// the distance between it self and a wander point, to be considered to have that point to be considered as being visited
	constexpr float soulCollisionRadius = 10.f;			// radius of the collision circle based on the center position, used for collision between devourer and the player


	// -- Player -- 

	constexpr float PLAYER_SPEED = 70.f;				// speed that the player goes in px/s 
	constexpr float PLAYER_SIZE = 20.f;					// height and width of the player collider

	constexpr float playerDashRecharge = .2f;			// How fast the charge bar refills, in percentage of the whole bar per sec. (1.f means 100% in 1 second)
	constexpr float playerDashCost = .5f;				// cost of a dash in percentage of the whole bar
	constexpr float playerDashDuration = .25f;			// How long the dast last
	constexpr float playerDashSpeed = 250.f;			// speed in px/s the dash is.		(playerDashDuration*playerDashSpeed, is the distance the player travels per dash)



	// -- ParticlesSystems --


	const static Engine::BaseParticleSystemParams soulParticleParams
	{
		.spawnRate = .05f,
		.initialPositionDeviation = 0.f,

		.initialVelocity = Tmpl8::vec2(0.f),
		.initialVelocityDeviation = Tmpl8::vec2(5.f, 5.f),

		.particleLifetime = 1.f,
		.particleLifetimeDeviation = .0f,

		.particleMass = .1f,
		.particleMassDeviation = 0.f,

		.colorRangeStart = Tmpl8::vec3(255.f),
		.colorRangeEnd = Tmpl8::vec3(200.f),

		.sizeRangeStart = 10.5f,
		.sizeRangeEnd = 2.5f,

	};

	const static Engine::BaseParticleSystemParams devourerParticleParams
	{
		.spawnRate = .05f,
		.initialPositionDeviation = 0.f,

		.initialVelocity = Tmpl8::vec2(0.f),
		.initialVelocityDeviation = Tmpl8::vec2(5.f, 5.f),

		.particleLifetime = 1.f,
		.particleLifetimeDeviation = .0f,

		.particleMass = .1f,
		.particleMassDeviation = 0.f,

		.colorRangeStart = Tmpl8::vec3(40.f, 15.f, 15.f),
		.colorRangeEnd = Tmpl8::vec3(20.f, 5.f , 5.f),

		.sizeRangeStart = 10.5f,
		.sizeRangeEnd = 2.5f,
	};


	const static Engine::BaseParticleSystemParams vacuumParticles
	{
		.spawnRate = 0.f,

		.initialPositionDeviation = Tmpl8::vec2(40.f, 40.f),

		.initialVelocity = Tmpl8::vec2(0.f,0.f),
		.initialVelocityDeviation = Tmpl8::vec2(0.f,0.f),

		.particleLifetime = 1.0f,
		.particleLifetimeDeviation = .2f,


		.colorRangeStart = Tmpl8::vec3(150.f, 150.f, 150.f),
		.colorRangeEnd = Tmpl8::vec3(230.f, 230.f, 230.f),


		.sizeRangeStart = 1.f,
		.sizeRangeEnd = 1.f

	};



}