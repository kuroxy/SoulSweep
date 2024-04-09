#pragma once
#include "template.h"
#include "aabb.hpp"
#include <vector>
#include "ParticleSystem.hpp"
#include "SpriteSheet.hpp"


namespace Engine
{
	class Camera;
}


class SoulConduit
{
public:
	SoulConduit() = default;
	SoulConduit(Tmpl8::vec2 position, std::shared_ptr<Engine::SpriteSheet> colliderSheet, Engine::ParticleSystemParams SoulFireParams)
	{
		conduitSheet = colliderSheet;
		conduitCollider = Engine::AABB( position,position + Tmpl8::vec2((float)colliderSheet->getSpriteWidth(), (float)colliderSheet->getSpriteHeight()));


		particleNorth = new Engine::BaseParticleSystem(SoulFireParams, 50);
		particleEast = new Engine::BaseParticleSystem(SoulFireParams, 50);
		particleSouth = new Engine::BaseParticleSystem(SoulFireParams, 50);
		particleWest = new Engine::BaseParticleSystem(SoulFireParams, 50);
		setPosition(position); // sets the position for the paricles
	}

	~SoulConduit()
	{
		delete particleNorth;
		delete particleEast;
		delete particleSouth;
		delete particleWest;
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
	Engine::BaseParticleSystem* particleNorth{ nullptr };
	Engine::BaseParticleSystem* particleEast{ nullptr };
	Engine::BaseParticleSystem* particleSouth{ nullptr };
	Engine::BaseParticleSystem* particleWest{ nullptr };
};