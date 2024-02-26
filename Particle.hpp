#pragma once
#include "template.h"
#include "surface.h"

namespace Engine
{
	class Camera;

	class Particle
	{
	public:
		Particle();
		~Particle() {};

		void update(float deltaTime);

		void render(Camera& camera);

	private:
		Tmpl8::vec2 position;
		Tmpl8::vec2 velocity;

		int size;
		Tmpl8::Pixel color;

	};
}