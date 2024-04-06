#pragma once
#include <string_view>

namespace Config
{
	//-- Game Settings --
	constexpr int minSouls = 10;
	constexpr float spawnDistance = 600.f;

	//World
	// the spritesheet texture (fullpath e.i "assets/large.png")
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

	constexpr float devourerMaxSpeed = 55.f; // the max speed that devourer goes in px/s
	constexpr float devourerMaxForce = 300.f; // how much force can be added in a single add force call.
	constexpr float devourerConsumingTime = 2.f; // time it takes for the devourer to eat a soul. This time the devourer&soul are frozen in place.
	
	constexpr float devourerVisitRadius = 40.f; // distance to a point to consider it visited. T
	constexpr float devourerMaxPlayerDistance = 700.f; // bigger distance with player-devourer then this var, devourer knows player position.

	constexpr float devourerWanderStrength = .5f; // how fast wandering changes. higher more random, lower the less change over time.

	constexpr float devourerCollideRadius = 10.f; // collider radius of the devourer.

	// -- Soul --


	constexpr float soulMaxForce = 100.f;
	constexpr float soulVacuumSpeed = 1000.f;
	constexpr float soulFleeSpeed = 45.f;
	constexpr float soulDefaultSpeed = 30.f;
	constexpr float soulMaxNeighbourRadius = 20.f;
	constexpr float soulMinPlayerDist = 400.f;
	constexpr float soulMaxPlayerDistance = 600.f;
	constexpr float soulWanderSpeed = .5f;
	constexpr float soulCollisionRadius = 10.f;


	constexpr float soulWanderStrength = .5f; // how fast wandering changes. higher more random, lower the less change over time.

	constexpr float soulCollideRadius = 10.f; // collider radius of the devourer.



	// -- Player -- 
	// the speed that the player goes in px/s 
	constexpr float PLAYER_SPEED = 60.f; // TODO: player speed slows on having more souls

	constexpr float PLAYER_SIZE = 20.f; // currently only a square player

}