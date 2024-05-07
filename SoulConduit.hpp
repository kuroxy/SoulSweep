#pragma once

#include "template.h"
#include "aabb.hpp"
#include <vector>
#include "ParticleSystem.hpp"
#include "SpriteSheet.hpp"
#include "Config.hpp"


// Forward declaration
namespace Engine
{
	class Camera;
}



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
			pSystem.drawParticles(camera);
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
	}


	bool isConduitActive() const { return conduitActivated; }

	Tmpl8::vec2 getPosition() const { return conduitCollider.min; }
	Tmpl8::vec2 getSize() const { return { conduitCollider.width() ,conduitCollider.height() }; }

	float getWidth() const { return conduitCollider.width(); }
	float getHeight() const { return conduitCollider.height(); }

	int getCollectedSouls() const { return collectedSouls; }

	bool contains(const Tmpl8::vec2& position) const { return conduitCollider.contains(position); }

	// This collects a soul, and spawns a visual soul that gets sucked into the center
	void spawnCollectedSoul(const Tmpl8::vec2& position);

	void update(float deltaTime, const Tmpl8::vec2& playerPosition);

	void draw(Engine::Camera& camera) const;
	void drawDebug(Engine::Camera& camera) const;

	

private:
	std::shared_ptr<Engine::SpriteSheet> conduitSheet;
	Engine::AABB conduitCollider;

	bool conduitActivated = false;

	int collectedSouls = 0;

	
	std::vector<soulLerping> suckedSouls;

};