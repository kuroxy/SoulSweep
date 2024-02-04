#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "assetManager.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include <SDL.h>


namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------


	Engine::AssetManager am{};
	Engine::Camera mainCamera{ ScreenWidth,ScreenWidth };

	Player mainPlayer{ {0}, 10 };

	void Game::Init()
	{
		im.addKeyMap("forward", SDL_SCANCODE_W);
		im.addKeyMap("backward", SDL_SCANCODE_S);
		im.addKeyMap("left", SDL_SCANCODE_A);

		im.addMouseMap("left", SDL_BUTTON_LEFT);
		im.addKeyMap("right", SDL_SCANCODE_D);
	}

	void Game::Shutdown()
	{
	}
	



	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		float clampedDT = Min(deltaTime / 1000, .032f); // slowest is 30fps clamping solves more lag spike issues

		//update / movement

		vec2 dir{ 0 };
		if (im.isActionPressed("forward"))
			dir.y -= 1.f;
		if (im.isActionPressed("backward"))
			dir.y += 1.f;
		if (im.isActionPressed("left"))
			dir.x -= 1.f;
		if (im.isActionPressed("right"))
			dir.x += 1.f;
		mainPlayer.move(dir, clampedDT);


		// rendering
		mainCamera.Fill(0);

		mainPlayer.draw(mainCamera, true);

		mainCamera.renderToSurface(screen);



		im.clear();
	}
};