#pragma once
#include "template.h"
#include "surface.h"

//forward declaration
namespace Engine
{
	class Camera;
}



class Entity
{
public:
	Entity(const Tmpl8::vec2& pos, float radius);

	const Tmpl8::vec2& getPosition() const;
	void setPosition(const Tmpl8::vec2& pos);

	float getRadius() const;
	void setRadius(float radius);


	bool collides(const Entity& o) const;

	void debugDraw(Engine::Camera& camera, Tmpl8::Pixel c);

	virtual void draw(Engine::Camera& camera, bool debug = false);


private:
	Tmpl8::vec2 m_position;
	float m_collisionRadius;
};

