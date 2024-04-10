#pragma once
#include "template.h"
#include "aabb.hpp"
#include <vector>
#include "ParticleSystem.hpp"
#include "SpriteSheet.hpp"
#include "Config.hpp"

namespace Engine
{
	class Camera;
}



struct blueFireParticles
{
	Engine::BaseParticleSystem flames;
	Engine::BaseParticleSystem core;

	blueFireParticles(Tmpl8::vec2 position)
		: flames{Config::blueFlamesParameters, 50}
		, core{Config::blueCoreParameters, 50}
	{
	
	}

	void setPosition(Tmpl8::vec2 position)
	{
		flames.setPosition(position);
		core.setPosition(position);
	}

	void update(float deltaTime)
	{
		flames.updateParticles(deltaTime);
		core.updateParticles(deltaTime);
	}

	void render(Engine::Camera& camera) const
	{
		flames.renderParticles(camera);
		core.renderParticles(camera);
	}

};







class SoulConduit
{
public:
	SoulConduit() = default;
	SoulConduit(Tmpl8::vec2 position, std::shared_ptr<Engine::SpriteSheet> colliderSheet)
	{
		conduitSheet = colliderSheet;
		conduitCollider = Engine::AABB( position,position + Tmpl8::vec2((float)colliderSheet->getSpriteWidth(), (float)colliderSheet->getSpriteHeight()));

		setPosition(position); // sets the position for the paricles
	}


	
	bool isConduitActive() const { return conduitActivated; }

	void setPosition(const Tmpl8::vec2& position);
	Tmpl8::vec2 getPosition() const { return conduitCollider.min; }
	

	void update(float deltaTime, Tmpl8::vec2 playerPosition);

	void draw(Engine::Camera& camera, bool debug=false) const;

	bool contains(const Tmpl8::vec2& position) const;

	

private:
	std::shared_ptr<Engine::SpriteSheet> conduitSheet;
	Engine::AABB conduitCollider;

	bool conduitActivated = false;

	// maybe make it an array or vector
	blueFireParticles particleNorth{ Tmpl8::vec2(0)};
	blueFireParticles particleEast{ Tmpl8::vec2(0) };
	blueFireParticles particleSouth{ Tmpl8::vec2(0) };
	blueFireParticles particleWest{ Tmpl8::vec2(0) };
};