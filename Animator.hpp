#pragma once
#include "SpriteSheet.hpp"
#include <vector>
#include <unordered_map>
#include "Camera.hpp"

namespace Engine
{
	struct anim
	{
		std::vector<int> spriteIndices;
		float timePerFrame;
	};


	class Animator
	{
	public:
		Animator(SpriteSheet& spriteSheet)
			: sprites{ spriteSheet } {};

		std::string getCurrentAnim() const { return currentAnimation; }
		float getAnimationSpeed() const { return animationSpeed; }

		void setAnimationSpeed(float speed) { animationSpeed = speed; }

		void addAnimation(std::string_view animationName, std::vector<int> frames);
		void changeAnimation(std::string_view animationName);

		void update(float deltaTime);
		void draw(Camera& camera, const Tmpl8::vec2& worldPosition, bool flip=false);
		

	private:
		SpriteSheet sprites;

		std::unordered_map<std::string, std::vector<int>> animations;

		std::string currentAnimation = "";
		int currentFrame = 0;

		float animationSpeed = 1.f; // this is how long each frame will be shown.
		float currentTime = animationSpeed;

	};

}