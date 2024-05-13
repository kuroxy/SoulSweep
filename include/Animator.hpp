#pragma once

#include <vector>
#include <unordered_map>

#include "SpriteSheet.hpp"


namespace Engine
{

class Camera;




struct animationData
{
	animationData(std::vector<int> frames = {}, float animSpeed = 1.f, bool looping = true)
		: frames{ frames }
		, animationSpeed{ animSpeed }
		, isLooping{ looping } {}

	std::vector<int> frames{};
	float animationSpeed{ 1.f };
	bool isLooping{ true };
};


class Animator
{
public:
	Animator(std::shared_ptr<Engine::SpriteSheet> spriteSheet)
		: sprites{ spriteSheet } {};

	std::string getCurrentAnim() const { return currentAnimation; }

	bool isFirstFrame() const { return currentAnimation != "" && currentFrame == 0; }
	bool isLastFrame() const { return currentAnimation != "" && currentFrame == (animations.at(currentAnimation).frames.size() - 1); }

	void addAnimation(std::string_view animationName, animationData animation);
	void addAnimation(std::string_view animationName, std::vector<int> frames,float animSpeed, bool loops);

	// Does not reset the animation if you call with the same animation it is currently running, unless reset is true
	void changeAnimation(std::string_view animationName, bool reset = false);


	void update(float deltaTime);

	// Can only handle horizontal flip
	void draw(Camera& camera, const Tmpl8::vec2& worldPosition, bool flip=false);
		

private:
	std::shared_ptr<Engine::SpriteSheet> sprites;

	std::unordered_map<std::string, animationData> animations{};

	std::string currentAnimation = "";
	int currentFrame = 0;

	// Counts down if this reaches 0 then next frame will show.
	float timeUntilNextFrame = 0.f;

};
}