#pragma once
#include "template.h"
#include "surface.h"
#include <vector>
#include <queue>

namespace Engine
{
	class Camera;


	struct ParticleSystemParams
	{
		
		float spawnRate{ 0.f }; //time between new particles // zero for no automatic spawning
		Tmpl8::vec2 initialPositionDeviation{ 0.f }; // the range that it is alowed to go negative or positive of the initialPosition

		Tmpl8::vec2 initialVelocity{ 0 };
		Tmpl8::vec2 initialVelocityDeviation{ 0 }; // same as positionDeviation

		float particleLifetime{ 1.f }; // in seconds
		float particleLifetimeDeviation{ 0.f }; // biggest amount it can deviate from particleLifetime


		// Appearance 
		// 
		// just use the floats as ints
		Tmpl8::vec3 colorRangeStart{ 255.f };
		Tmpl8::vec3 colorRangeEnd{ 255.f };

		// start time is when it will start (before this time it it will have the start value)
		float transparencyStartTime{0.f}; // will be in a percentage of the total lifespan
		float transparencyStartValue{ 1.f }; // 0-1 (transparent-solid)
		// endtime is when the transparency range ends (after this time the particle will have the endvalue);
		float transparencyEndTime{ 1.f };
		float transparencyEndValue{ 1.f };


		float sizeRangeStart{ 1.f };
		float sizeRangeEnd{ 1.f };

	};


	


	class BaseParticleSystem {
	public:
		BaseParticleSystem(const ParticleSystemParams& params, int poolSize)
			: parameters{ params } 
		{
			particlePool.resize(poolSize);
		}

		void spawnParticle();

		void updateParticles(float deltaTime);

		void renderParticles(Camera& camera) const;

		const Tmpl8::vec2& getPosition() const { return position; }
		void setPosition(Tmpl8::vec2 newPosition) { position = newPosition; }

	private:
		
		struct Particle
		{
			bool enabled = false;
			Tmpl8::vec2 position;
			Tmpl8::vec2 velocity;

			float size = 0;
			Tmpl8::Pixel color = 0;

			float maxLifeTime = 1.f;
			float currentLifeTime = 0.f;

		};

		Tmpl8::vec2 position;
		ParticleSystemParams parameters;
		float timeTillNextSpawn = parameters.spawnRate;

		std::vector<Particle> particlePool;

		// Currently does not work when you try to call more than 1 particle per frame
		Particle* getNewParticle(); // gets a available particle and returns a pointer to it, if there is none returns nullptr

		void resetParticle(Particle* particle);

	};
}