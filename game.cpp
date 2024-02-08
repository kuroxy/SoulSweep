#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "assetManager.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Soul.hpp"
#include <SDL.h>
#include "Tilemap.hpp"


namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------


	//Engine::AssetManager am;
	Engine::Camera mainCamera{ ScreenWidth,ScreenHeight };

	std::shared_ptr<Engine::Texture> sheetTexture;
	std::shared_ptr<Engine::SpriteSheet> mainSheet;

	Tilemap* tm;

	Player mainPlayer{ {0}, 10 };

	std::vector<Soul> souls;

	void Game::Init()
	{
		im.addKeyMap("up", SDL_SCANCODE_W);
		im.addKeyMap("down", SDL_SCANCODE_S);
		im.addKeyMap("left", SDL_SCANCODE_A);
		im.addKeyMap("right", SDL_SCANCODE_D);

		im.addKeyMap("debugup", SDL_SCANCODE_UP);
		im.addKeyMap("debugdown", SDL_SCANCODE_DOWN);
		im.addKeyMap("debugleft", SDL_SCANCODE_LEFT);
		im.addKeyMap("debugright", SDL_SCANCODE_RIGHT);

		//im.addMouseMap("left", SDL_BUTTON_LEFT);

		for (int i = 0; i < 10; i++)
		{
			souls.push_back(Soul(Tmpl8::vec2(Rand(100)+100, Rand(100) + 100)));
		}

		sheetTexture = std::make_shared<Engine::Texture>("assets/Tilesheet/bicubic.png");
		sheetTexture->setChromaKey(0xff00ff);

		mainSheet = std::make_shared<Engine::SpriteSheet>(sheetTexture, 31, 31);

		tm = new Tilemap(mainSheet, 50, 50, 31, 31);
		tm->setTile(5, 5, 0, true);
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
		if (im.isActionPressed("up"))
			dir.y -= 1.f;
		if (im.isActionPressed("down"))
			dir.y += 1.f;
		if (im.isActionPressed("left"))
			dir.x -= 1.f;
		if (im.isActionPressed("right"))
			dir.x += 1.f;

		mainPlayer.move(dir, clampedDT);
		

		dir = vec2(0);
		if (im.isActionPressed("debugup"))
			dir.y -= 1.f;
		if (im.isActionPressed("debugdown"))
			dir.y += 1.f;
		if (im.isActionPressed("debugleft"))
			dir.x -= 1.f;
		if (im.isActionPressed("debugright"))
			dir.x += 1.f;

		mainCamera.setPosition(mainCamera.getPosition() + dir * 20 * clampedDT);


		for (auto& soul : souls)
		{
			soul.actionSelection(mainPlayer.getPosition());
			soul.update(clampedDT);
		}

		// rendering
		mainCamera.Fill(0);

		tm->draw(mainCamera, true);
		mainPlayer.draw(mainCamera, true);

		tm->lineSegmentCollideDebug(mainPlayer.getPosition(), vec2(mouseX, mouseY), mainCamera);
		for (auto& soul : souls)
		{
			soul.draw(mainCamera, true);
		}

		mainCamera.renderToSurface(screen);



		im.clear();
	}
};