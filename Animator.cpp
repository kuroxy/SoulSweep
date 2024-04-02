#include "Animator.hpp"

void Engine::Animator::addAnimation(std::string_view animationName, std::vector<int> frames)
{
	std::string name = std::string{ animationName };
	animations[name] = frames;
}

void Engine::Animator::changeAnimation(std::string_view animationName)
{
	std::string anim = std::string{ animationName };
	if (anim == currentAnimation)
		return;

	currentTime = animationSpeed;
	currentFrame = 0;

	currentAnimation = anim;
}

void Engine::Animator::update(float deltaTime)
{
	// we skip if no animation is selected since then nothing should happen
	if (currentAnimation == "")
		return;

	currentTime -= deltaTime;
	if (currentTime <= 0.f)
	{
		currentTime = animationSpeed;
		currentFrame++;

		if (currentFrame >= animations[currentAnimation].size())
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


	int frameIndex = animations[currentAnimation][currentFrame];
	camera.renderSpriteWorldSpace(&sprites, frameIndex, worldPosition, flip);

}
