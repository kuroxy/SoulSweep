#include "game.h"
#include "surface.h"
#include <cstdio> //printf

#include "Texture.hpp"
#include "IsoCamera.hpp"

namespace Tmpl8
{
	IsoEngine::Texture* t1;
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		t1 = new IsoEngine::Texture{ 10,10, 0xffffff};
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}


	IsoEngine::IsoCamera* a = new IsoEngine::IsoCamera(10, 10, 10);

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		// clear the graphics window
		screen->Clear(0);



		t1->CopyToSurface(screen, 10, 10);

	}
};