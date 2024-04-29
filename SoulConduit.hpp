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
	struct soulLerping
	{
		soulLerping() = default;
		soulLerping(Tmpl8::vec2 startPos, Tmpl8::vec2 endPos, float _speed, float attractorStrenth)
		{
			pSystem.setPosition(startPos);
			pSystem.setAttractor(endPos, attractorStrenth);
			endPoint = endPos;
			startPoint = startPos;
			speed = _speed / (endPos - startPos).length(); // calulcates speed, based on distance, so it will move towards the endpoint with _speed px/s
		}

		void update(float deltaTime)
		{
			pSystem.setPosition(startPoint* (1.f - progress) + endPoint*progress);
			pSystem.updateParticles(deltaTime);

			progress += deltaTime * speed;
			progress = std::clamp(progress, 0.f, 1.f);

			pSystem.parameters.sizeRangeStart = Config::soulParticleParams.sizeRangeStart * (1.f - progress) + 3.1f * progress;

			pSystem.parameters.sizeRangeEnd = Config::soulParticleParams.sizeRangeEnd * (1.f - progress) + 1.1f * progress;


			pSystem.parameters.particleLifetime = Config::soulParticleParams.particleLifetime * (1.f - progress) + .5f * progress;


			for (auto& p : pSystem.getParticles())
			{
				if (!p.enabled)
					continue;

				Tmpl8::vec2 direction = (endPoint - p.position);
				
				// if moves to far away from the 
				if (direction.normalized().dot(p.velocity.normalized()) < .5 && direction.length() > 3.f)
				{
					p.currentLifeTime = 0.f;
					p.enabled = false;
				}
					
			}

		}

		bool isDone() const { return progress == 1.f; }

		void render(Engine::Camera& camera) const
		{
			pSystem.renderParticles(camera);
		}

		Engine::BaseParticleSystem pSystem{ Config::soulParticleParams, 50 };
		Tmpl8::vec2 endPoint;
		Tmpl8::vec2 startPoint;
		float progress = 0.f;
		float speed = 1.f;

	};



public:
	SoulConduit() = default;
	SoulConduit(Tmpl8::vec2 position, std::shared_ptr<Engine::SpriteSheet> colliderSheet)
	{
		conduitSheet = colliderSheet;
		conduitCollider = Engine::AABB(position, position + Tmpl8::vec2((float)colliderSheet->getSpriteWidth(), (float)colliderSheet->getSpriteHeight()));

		setPosition(position); // sets the position for the paricles
	}


	
	bool isConduitActive() const { return conduitActivated; }

	void setPosition(const Tmpl8::vec2& position);
	Tmpl8::vec2 getPosition() const { return conduitCollider.min; }
	Tmpl8::vec2 getSize() const { return { conduitCollider.width() ,conduitCollider.height() }; }

	float getWidth() const { return conduitCollider.width(); }
	float getHeight() const { return conduitCollider.height(); }

	void spawnCollectedSoul(const Tmpl8::vec2& position);
	int getCollectedSouls() const { return collectedSouls; }


	void update(float deltaTime, const Tmpl8::vec2& playerPosition);

	void draw(Engine::Camera& camera, bool debug=false) const;

	bool contains(const Tmpl8::vec2& position) const;

	

private:
	std::shared_ptr<Engine::SpriteSheet> conduitSheet;
	Engine::AABB conduitCollider;

	bool conduitActivated = false;

	int collectedSouls = 0;

	// maybe make it an array or vector

	blueFireParticles fireParticles[4] = { Tmpl8::vec2(0) ,Tmpl8::vec2(0) ,Tmpl8::vec2(0) ,Tmpl8::vec2(0) };
	bool fireParticlesUpdate[4] = { false, false, false, false };


	std::vector<soulLerping> suckedSouls;

};