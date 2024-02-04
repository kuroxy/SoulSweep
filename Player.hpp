#pragma once
#include "template.h"

//forward declaration
namespace Engine
{
	class Camera;
}

class Player
{
public:
	Player(const Tmpl8::vec2& pos, float radius);

	const Tmpl8::vec2& getPosition() const;
	void setPosition(const Tmpl8::vec2& pos);
	void move(const Tmpl8::vec2& direction, float deltaTime);

	float getRadius() const;

	void draw(Engine::Camera& camera, bool debug=false);

private:
	Tmpl8::vec2 m_position;
	Tmpl8::vec2 m_direction;
	float m_playerSpeed;
	float m_collisionRadius;
};

