#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "assetManager.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Soul.hpp"
#include <SDL.h>
#include "Tilemap.hpp"
#include "aabb.hpp"

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------


	std::shared_ptr<Engine::Texture> sheetTexture;
	std::shared_ptr<Engine::SpriteSheet> mainSheet;

	Tilemap* tm;

	Player mainPlayer{ {100.f,100.f}, 60.f, 20.f,20.f };

	std::vector<Soul> souls;

	void Game::Init()
	{
		

		mainCamera = Engine::Camera(ScreenWidth, ScreenHeight);

		im.addKeyMap("up", SDL_SCANCODE_W);
		im.addKeyMap("down", SDL_SCANCODE_S);
		im.addKeyMap("left", SDL_SCANCODE_A);
		im.addKeyMap("right", SDL_SCANCODE_D);
		im.addKeyMap("vacuum", SDL_SCANCODE_F);
		im.addMouseMap("vacuum", SDL_BUTTON_LEFT);

		im.addKeyMap("debugup", SDL_SCANCODE_UP);
		im.addKeyMap("debugdown", SDL_SCANCODE_DOWN);
		im.addKeyMap("debugleft", SDL_SCANCODE_LEFT);
		im.addKeyMap("debugright", SDL_SCANCODE_RIGHT);

		im.addMouseMap("leftmouse", SDL_BUTTON_LEFT);

		//im.addMouseMap("left", SDL_BUTTON_LEFT);

		for (int i = 0; i < 10; i++)
		{
			souls.push_back(Soul(Tmpl8::vec2(Rand(100)+100, Rand(100) + 100), mainPlayer, *tm));
		}

		sheetTexture = std::make_shared<Engine::Texture>("assets/large.png");
		sheetTexture->setChromaKey(0xff00ff);

		mainSheet = std::make_shared<Engine::SpriteSheet>(sheetTexture, 32, 32);


		tm = new Tilemap(mainSheet, "assets/map/mapLayer1.csv", "assets/map/mapLayer2.csv");
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

		mainPlayer.handleInput(im);

		mainPlayer.update(clampedDT, *tm);

		vec2 dir = vec2(0);
		if (im.isActionPressed("debugup"))
			dir.y -= 1.f;
		if (im.isActionPressed("debugdown"))
			dir.y += 1.f;
		if (im.isActionPressed("debugleft"))
			dir.x -= 1.f;
		if (im.isActionPressed("debugright"))
			dir.x += 1.f;

		mainCamera.setPosition(mainCamera.getPosition() + dir * 100 * clampedDT);


		/*if (im.isActionPressed("leftmouse"))
		{
			vec2 grid = tm->worldToGrid(im.getWorldMouse());
			tm->setTile(grid.x, grid.y, 2, true);
		}*/

		for (auto& soul : souls)
		{
			soul.actionSelection();
			soul.update(clampedDT);
		}

		// rendering
		mainCamera.Fill(0x72751b);

		

		

		tm->draw(mainCamera, true);
		mainPlayer.draw(mainCamera, true);

		vec2 a = im.getWorldMouse();

	
		for (auto& soul : souls)
		{
			soul.draw(mainCamera, true);
		}

		mainCamera.renderToSurface(screen);



		im.update(mainCamera);
	}
};