#pragma once
#include "template.h"
#include "aabb.hpp"

namespace Engine
{
	class Camera;
}


class SoulConduit
{
public:
	SoulConduit() = default;
	SoulConduit(Tmpl8::vec2 position, Tmpl8::vec2 size)
		: collider{position,position+size}
	{}
	
	
	Tmpl8::vec2& getPosition() { return collider.min; }

	void draw(Engine::Camera& camera, bool debug=false) const;

	bool contains(const Tmpl8::vec2& position) const;


private:
	Engine::AABB collider;
};