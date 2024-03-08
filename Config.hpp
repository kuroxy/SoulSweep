#pragma once
#include <string_view>

namespace Config
{
	//World
	// the spritesheet texture (fullpath e.i "assets/large.png")
	constexpr std::string_view TERRAIN_TEXTURE = "assets/large.png";

	// we can only have square sprites so just the width
	constexpr int TERRAIN_SPRITE_SIZE = 32;

	constexpr Tmpl8::Pixel TERRAIN_CHROMA = 0xff00ff; // 0 for no chroma

	// The csv for the map (just the namename like file is "assets/map/thismapL1.csv" then just "thismap")
	constexpr std::string_view MAP_NAME = "map1";


	// -- Devourer -- 

	constexpr float devourerMaxSpeed = 50.f; // the max speed that devourer goes in px/s
	constexpr float devourerMaxForce = 100.f; // how much force can be added in a single add force call.
	constexpr float devourerConsumingTime = 2.f; // time it takes for the devourer to eat a soul. This time the devourer&soul are frozen in place.
	
	constexpr float devourerVisitRadius = 40.f; // distance to a point to consider it visited. T
	constexpr float devourerMaxPlayerDistance = 500.f; // bigger distance with player-devourer then this var, devourer knows player position.

	constexpr float devourerCollideRadius = 10.f; // collider radius of the devourer.


	// -- Player -- 
	// the speed that the player goes in px/s 
	constexpr float PLAYER_SPEED = 60.f; // TODO: player speed slows on having more souls

	constexpr float PLAYER_SIZE = 20.f; // currently only a square player

}