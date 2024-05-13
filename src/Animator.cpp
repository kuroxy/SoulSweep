#include "Animator.hpp"

void Engine::Animator::addAnimation(std::string_view animationName, animationData animation)
{
	std::string name = std::string{ animationName };
	animations[name] = animation;
}

void Engine::Animator::addAnimation(std::string_view animationName, std::vector<int> frames, float animSpeed, bool isLooping)
{
	addAnimation(animationName, { frames, animSpeed, isLooping });
}

void Engine::Animator::changeAnimation(std::string_view animationName, bool reset)
{
	std::string anim = std::string{ animationName };
	if (anim == currentAnimation && !reset)
		return;

	currentAnimation = anim;
	timeUntilNextFrame = animations[currentAnimation].animationSpeed;
	currentFrame = 0;

}

void Engine::Animator::update(float deltaTime)
{
	// we skip if no animation is selected since then nothing should happen
	// or if the animation is not looping and on its last frame
	if (currentAnimation == "" || (!animations[currentAnimation].isLooping && currentFrame == animations[currentAnimation].frames.size()-1))
		return;

	
	timeUntilNextFrame -= deltaTime;
	if (timeUntilNextFrame <= 0.f)
	{
		timeUntilNextFrame = animations[currentAnimation].animationSpeed;
		currentFrame++;

		if (currentFrame >= animations[currentAnimation].frames.size())
		{
			currentFrame = 0;
		}
	}

}

void Engine::Animator::draw(Camera& camera, const Tmpl8::vec2& worldPosition, bool flip)
{
	// we skip if no animation is selected since then nothing should happen
	if (currentAnimation == "")
		return;


	int frameIndex = animations[currentAnimation].frames[currentFrame];
	sprites->draw(camera, frameIndex, worldPosition, flip);

}
