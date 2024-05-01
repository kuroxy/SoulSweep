#pragma once
#include <string_view>
#include "ParticleSystem.hpp"

namespace Config
{
	//-- Game Settings --
	constexpr int minSouls = 5;


	//World
	// the spritesheet 
	// 
	// 
	// 
	// 
	// 
	//  (fullpath e.i "assets/large.png")
	constexpr std::string_view TERRAIN_TEXTURE = "assets/large.png";

	// we can only have square sprites so just the width
	constexpr int TERRAIN_SPRITE_SIZE = 32;

	constexpr Tmpl8::Pixel TERRAIN_CHROMA = 0xff00ff; // 0 for no chroma

	// The csv for the map (just the namename like file is "assets/map/thismapL1.csv" then just "thismap")
	constexpr std::string_view MAP_NAME = "TheMap"; //"oldmap";

	// fog of war settings
	constexpr float viewDistanceMin = 75.f; // distance smaller is always visible
	constexpr float viewDistanceMax = 300.f; // after this distance is dark

	// -- Devourer -- 

	constexpr float devourerMaxSpeed = 70.f; // the max speed that devourer goes in px/s
	constexpr float devourerMaxForce = 300.f; // how much force can be added in a single add force call.
	constexpr float devourerConsumingTime = 3.f; // time it takes for the devourer to eat a soul. This time the devourer&soul are frozen in place.
	
	constexpr float devourerVisitRadius = 20.f; // distance to a point to consider it visited. T
	constexpr float devourerMaxPlayerDistance = 700.f; // bigger distance with player-devourer then this var, devourer knows player position.

	constexpr float devourerWanderStrength = .5f; // how fast wandering changes. higher more random, lower the less change over time.

	constexpr float devourerCollideRadius = 10.f; // collider radius of the devourer.

	// -- Soul --


	constexpr float soulMaxForce = 100.f;
	constexpr float soulVacuumSpeed = 1000.f;
	constexpr float soulFleeSpeed = 45.f;
	constexpr float soulDefaultSpeed = 50.f;
	constexpr float soulMaxNeighbourRadius = 100.f;
	constexpr float soulWanderDist = 40.f;
	constexpr float soulCollisionRadius = 10.f;


	constexpr float soulWanderStrength = .5f; // how fast wandering changes. higher more random, lower the less change over time.

	constexpr float soulCollideRadius = 10.f; // collider radius of the devourer.



	// -- Player -- 
	// the speed that the player goes in px/s 
	constexpr float PLAYER_SPEED = 70.f; // 

	constexpr float PLAYER_SIZE = 20.f; // currently only a square player

	constexpr float playerDashRecharge = .2f;
	constexpr float playerDashCost = .5f;
	constexpr float playerDashDuration = .25f;
	constexpr float playerDashSpeed = 250.f;



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



	const static Engine::BaseParticleSystemParams blueFlamesParameters
	{
		.spawnRate = .05f,

		.initialPositionDeviation = 0.f,

		.initialVelocity = Tmpl8::vec2(0.f, -20.f),
		.initialVelocityDeviation = Tmpl8::vec2(3.f, 2.f),

		.particleLifetime = 1.f,
		.particleLifetimeDeviation = .0f,


		.colorRangeStart = Tmpl8::vec3(59.f, 189.f, 244.f),
		.colorRangeEnd = Tmpl8::vec3(24.f, 10.f, 94.f),

		.sizeRangeStart = 7.5f,
		.sizeRangeEnd = 1.1f

	};

	const static Engine::BaseParticleSystemParams blueCoreParameters
	{
		.spawnRate = .05f,

		.initialPositionDeviation = Tmpl8::vec2(0.f, 0.f),

		.initialVelocity = Tmpl8::vec2(0.f,-2.f),
		.initialVelocityDeviation = Tmpl8::vec2(1.f, 1.f),

		.particleLifetime = 1.f,
		.particleLifetimeDeviation = .0f,


		.colorRangeStart = Tmpl8::vec3(255.f, 255.f, 255.f),
		.colorRangeEnd = Tmpl8::vec3(59.f, 189.f, 244.f),

		.transparencyStartTime = 0.f, // will be in a percentage of the total lifespan
		.transparencyStartValue = 1.f, // 0-1 (transparent-solid)

		// endtime is when the transparency range ends (after this time the particle will have the endvalue);
		.transparencyEndTime = 1.f,
		.transparencyEndValue = 1.f,


		.sizeRangeStart = 1.1f,
		.sizeRangeEnd = 7.2f

	};


}