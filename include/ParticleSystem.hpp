#pragma once

#include <vector>

#include "template.h"
#include "surface.h"


namespace Engine
{
// Forward Decl
class Camera; 


// Resources used to create the particles system:
// https://www.youtube.com/watch?v=GK0jHlv3e3w&t=678s
// https://gameprogrammingpatterns.com/object-pool.html

struct BaseParticleSystemParams
{
		
	float spawnRate{ 0.f }; //time between new particles // zero for no automatic spawning
	Tmpl8::vec2 initialPositionDeviation{ 0.f }; // the range that it is alowed to go negative or positive of the initialPosition

	Tmpl8::vec2 initialVelocity{ 0 };
	Tmpl8::vec2 initialVelocityDeviation{ 0 }; // same as positionDeviation

	float particleLifetime{ 1.f }; // in seconds
	float particleLifetimeDeviation{ 0.f }; // biggest amount it can deviate from particleLifetime


	float particleMass{ 1.f };
	float particleMassDeviation{ 0.f }; // max deviation from the mass given by the particleMass


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
	struct Particle
	{
		bool enabled = false;
		Tmpl8::vec2 position;
		Tmpl8::vec2 velocity;

		float size = 0;
		Tmpl8::Pixel color = 0;

		float maxLifeTime = 1.f;
		float currentLifeTime = 0.f;

		float mass = 1.f; // Used force calculation. Heavier harder to move, lighter easier to move.
	};

	BaseParticleSystem(const BaseParticleSystemParams& params, int poolSize)
		: parameters{ params }
	{
		particlePool.resize(poolSize);
	}


	const Tmpl8::vec2& getPosition() const { return position; }
	std::vector<Particle>& getParticles() { return particlePool; }



	void setPosition(Tmpl8::vec2 newPosition) { position = newPosition; }

	void setAttractorPostion(const Tmpl8::vec2 position) { attractorPosition = position; }
	void setAttractorStrenth(float strength) { attractorStrength = strength; }
	void setAttractor(const Tmpl8::vec2 position, float strenth) { attractorPosition = position; attractorStrength = strenth; }

	void spawnParticle();

	void updateParticles(float deltaTime);

	void drawParticles(Camera& camera) const;

	
	
	// Public because this way it is easier to change the parameters 
	// then a getters/setter. Since BaseParticleSystemParams is only here to hold data.
	BaseParticleSystemParams parameters;
private:
	Tmpl8::vec2 position; // World space
		
	float timeTillNextSpawn = parameters.spawnRate;

	std::vector<Particle> particlePool;

	// We can attract particles to a position. Use negative strength to get repulsion
	Tmpl8::vec2 attractorPosition{ 0.f }; 
	float attractorStrength{ 0.f };


	// Currently does not work when you try to call more than 1 particle per frame
	Particle* getNewParticle(); // gets a available particle and returns a pointer to it, if there is none returns nullptr

	// sets particle to inactive ready to be used again
	void resetParticle(Particle* particle);

};
}