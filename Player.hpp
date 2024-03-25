#pragma once
#include "template.h"
#include "aabb.hpp"

//forward declaration
namespace Engine
{
	class Camera;
	class InputManager;
}




class Level;

class Player
{
public:
	Player(const Tmpl8::vec2& pos, float playerSpeed, float width, float height)
		: m_position{ pos }
		, m_direction{ 0 }
		, m_playerSpeed{ playerSpeed }
		, m_width{ width }
		, m_height{ height }
	{
		updateAABB();
	}


	const Tmpl8::vec2& getPosition() const { return m_position; }
	void setPosition(const Tmpl8::vec2& pos) { m_position = pos; updateAABB(); }

	bool canCollectSoul() const { return m_currentSouls < m_maxSouls; }
	void collectSoul() { m_currentSouls = Tmpl8::Min<int>(m_currentSouls + 1, m_maxSouls); }
	void removeSoul() { m_currentSouls = Tmpl8::Max<int>(m_currentSouls - 1, 0); }
	float getCollectRadius() const { return m_collectRadius; }

	const Engine::AABB& getAABB() const { return m_collisionBox; }
	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; }
	const Tmpl8::vec2& getVacuumDirection() const { return m_vacuumDirection; }
	bool isVacuumEnabled() const { return m_vacuumEnabled; }

	bool shouldDropSoul() const { return m_currentSouls > 0 && m_dropSoul; }

	void handleInput(const Engine::InputManager& im);

	void move(float deltaTime);
	void update(float deltaTime, const Level& tilemap);

	bool vacuumRange(const Tmpl8::vec2 pos) const;
	const Tmpl8::vec2 calculateVacuumForce(const Tmpl8::vec2 pos) const;

	void draw(Engine::Camera& camera, bool debug=false);

private:
	Tmpl8::vec2 m_position{ 0 };
	Tmpl8::vec2 m_direction{ 0 }; // last movedirection
	float m_playerSpeed;
	float m_width;
	float m_height;

	Engine::AABB m_collisionBox;
	void updateAABB();

	
	bool m_vacuumEnabled = false;
	bool m_dropSoul = false;
	Tmpl8::vec2 m_vacuumDirection{ 1,0 };
	float m_vacuumCone = .86f; // cone angle you can calculate by cos(angle) 

	float m_collectRadius{ 12 };
	int m_maxSouls{ 5 };
	int m_currentSouls{ 0 };

	float m_maxVacuumForce = 1500.f;
	float m_maxVacuumDistance = 150.f;

};

