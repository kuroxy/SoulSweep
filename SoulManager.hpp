#pragma once

#include <vector>
#include "Soul.hpp"
#include "Player.hpp"
#include "Tilemap.hpp"
#include <memory>

namespace Tmpl8
{
	class vec2;
}

namespace Engine
{
	class Camera;
}


class SoulManager
{
public:
	SoulManager(Player* player, Tilemap* tilemap)
		: m_playerPtr{ player }
		, m_tileMapPtr{ tilemap }
	{}

	void spawnSoul(const Tmpl8::vec2& spawnPosition);

	//void removeSoul(const Soul& soul);

	void updateSouls(float dt);

	void renderSouls(Engine::Camera& cam);

private:
	Player* m_playerPtr;
	Tilemap* m_tileMapPtr;

	std::vector<Soul> m_souls; // just like this or like this -> std::vector<Soul> m_souls;
	
};