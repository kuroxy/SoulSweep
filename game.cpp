#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "Camera.hpp"
#include "Player.hpp"
#include "Soul.hpp"
#include <SDL.h>
#include "SoulSweep.hpp"
#include "Level.hpp"

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	
	
	SoulSweep game = SoulSweep();

	void Game::Init()
	{

		mainCamera = Engine::Camera(ScreenWidth, ScreenHeight);

		im.addKeyMap("up", SDL_SCANCODE_W);
		im.addKeyMap("down", SDL_SCANCODE_S);
		im.addKeyMap("left", SDL_SCANCODE_A);
		im.addKeyMap("right", SDL_SCANCODE_D);
		im.addKeyMap("vacuum", SDL_SCANCODE_F);
		im.addMouseMap("vacuum", SDL_BUTTON_LEFT);
		im.addMouseMap("dropsoul", SDL_BUTTON_RIGHT);

		im.addKeyMap("debugup", SDL_SCANCODE_UP);
		im.addKeyMap("debugdown", SDL_SCANCODE_DOWN);
		im.addKeyMap("debugleft", SDL_SCANCODE_LEFT);
		im.addKeyMap("debugright", SDL_SCANCODE_RIGHT);

		im.addKeyMap("debugfogofwar", SDL_SCANCODE_Z);

		im.addMouseMap("leftmouse", SDL_BUTTON_LEFT);

		//im.addMouseMap("left", SDL_BUTTON_LEFT);

		

		game.Initialize();


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

		vec2 dir = vec2(0);
		if (im.isActionHeld("debugup"))
			dir.y -= 1.f;
		if (im.isActionHeld("debugdown"))
			dir.y += 1.f;
		if (im.isActionHeld("debugleft"))
			dir.x -= 1.f;
		if (im.isActionHeld("debugright"))
			dir.x += 1.f;

		mainCamera.setPosition(mainCamera.getPosition() + dir * 100 * clampedDT);


		game.update(clampedDT, im, mainCamera);


		// rendering
		mainCamera.clearScreen(0x72751b);

		

		game.render(mainCamera);
		mainCamera.renderToSurface(screen);
	
		//screen->Print("Hallo", im.getLocalMouse().x, im.getLocalMouse().y, 0xffff00, 10);

		int mX = im.getLocalMouse().x;
		int mY = im.getLocalMouse().y;


		// update inputmanager for next keypresses
		im.update(mainCamera);
	}
};