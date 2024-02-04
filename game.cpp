#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "assetManager.hpp"
#include "Camera.hpp"
#include "Player.hpp"

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
		
	}

	void Game::Shutdown()
	{
	}
	



	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{

		//update / movement


		// rendering
		mainCamera.Fill(0);

		mainPlayer.draw(mainCamera, true);

		mainCamera.renderToSurface(screen);


	}
};