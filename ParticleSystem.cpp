#include "ParticleSystem.hpp"
#include "template.h"
#include "Camera.hpp"

using namespace Engine;

void BaseParticleSystem::spawnParticle()
{
    Particle* newParticle = getNewParticle();
    if (newParticle == nullptr)
        return; 

    newParticle->enabled = true;
    // makes it in the range of -initialPositionDeviation.x to + initialPositionDeviation.x
    float xDiff = Rand(parameters.initialPositionDeviation.x * 2) - parameters.initialPositionDeviation.x; 
    float yDiff = Rand(parameters.initialPositionDeviation.y * 2) - parameters.initialPositionDeviation.y;

    newParticle->position = position + Tmpl8::vec2(xDiff,yDiff);

    // now again but with velocity
    xDiff = Rand(parameters.initialVelocityDeviation.x * 2) - parameters.initialPositionDeviation.x;
    yDiff = Rand(parameters.initialVelocityDeviation.y * 2) - parameters.initialPositionDeviation.y;

    newParticle->velocity = Tmpl8::vec2(parameters.initialVelocity) + Tmpl8::vec2(xDiff, yDiff);


    float lifetimeDiff = Rand(parameters.particleLifetimeDeviation * 2) - parameters.particleLifetimeDeviation;

    newParticle->maxLifeTime = parameters.particleLifetime + lifetimeDiff;
    newParticle->currentLifeTime = 0;


    // size & color will be calculated in the updateloop.

}

void Engine::BaseParticleSystem::updateParticles(float deltaTime)
{
    if (parameters.spawnRate != 0)
    {
        timeTillNextSpawn -= deltaTime;
        if (timeTillNextSpawn <= 0)
        {
            timeTillNextSpawn = parameters.spawnRate;
            spawnParticle();
        }
    }

    for (auto& particle : particlePool)
    {
        // checking if particle is active
        if (!particle.enabled)
            continue;  

        particle.position += particle.velocity * deltaTime;
        particle.currentLifeTime += deltaTime;

        if (particle.currentLifeTime > particle.maxLifeTime)
        {
            resetParticle(&particle);
            continue;
        }

        float lifeTimePercentage = particle.currentLifeTime / particle.maxLifeTime;


        particle.size = (parameters.sizeRangeEnd-parameters.sizeRangeStart) * lifeTimePercentage + parameters.sizeRangeStart;

        int r = (parameters.colorRangeEnd.x -parameters.colorRangeStart.x) * lifeTimePercentage + parameters.colorRangeStart.x;

        int g = (parameters.colorRangeEnd.y - parameters.colorRangeStart.y) * lifeTimePercentage + parameters.colorRangeStart.y;

        int b = (parameters.colorRangeEnd.z - parameters.colorRangeStart.z) * lifeTimePercentage + parameters.colorRangeStart.z;

        float transparency = (Tmpl8::Clamp(lifeTimePercentage, parameters.transparencyStartTime, parameters.transparencyEndTime)- parameters.transparencyStartTime) /(parameters.transparencyEndTime-parameters.transparencyStartTime); // .5,.1,.7

        transparency = transparency * (parameters.transparencyEndValue - parameters.transparencyStartValue) + parameters.transparencyStartValue;

        particle.color = (((int)(transparency*255.f)) << 24) | (r << 16) | (g << 8) | b;
    }
}

void Engine::BaseParticleSystem::renderParticles(Camera& camera)
{
    for (auto& particle : particlePool)
    {
        // checking if particle is active
        if (!particle.enabled)
            continue;
        
        camera.drawFillCircleWorldSpace(particle.position, particle.size, particle.color, true);
    }
}

BaseParticleSystem::Particle* BaseParticleSystem::getNewParticle()
{
    if (!availableParticles.empty())
    {
        Particle* particle = availableParticles.front();
        availableParticles.pop();
        return particle;
    }
    return nullptr; // we also can increase the poolSize dynamically but I will leave it for now.
}

void BaseParticleSystem::resetParticle(Particle* particle)
{
    particle->enabled = false;
    availableParticles.push(particle);
}
