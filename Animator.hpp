#pragma once
#include "SpriteSheet.hpp"
#include <vector>
#include <unordered_map>
#include "Camera.hpp"

namespace Engine
{

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
	Animator(SpriteSheet& spriteSheet)
		: sprites{ spriteSheet } {};

	std::string getCurrentAnim() const { return currentAnimation; }

	bool isFirstFrame() const { return currentAnimation != "" && currentFrame == 0; }
	bool isLastFrame() const { return currentAnimation != "" && currentFrame == (animations.at(currentAnimation).frames.size() - 1); }

	void addAnimation(std::string_view animationName, animationData animation);
	void addAnimation(std::string_view animationName, std::vector<int> frames,float animSpeed, bool loops);

	void changeAnimation(std::string_view animationName);

	void update(float deltaTime);
	void draw(Camera& camera, const Tmpl8::vec2& worldPosition, bool flip=false);
		

private:
	SpriteSheet sprites;

	std::unordered_map<std::string, animationData> animations{};

	std::string currentAnimation = "";
	int currentFrame = 0;

	float currentTime = 0.f;

};
}