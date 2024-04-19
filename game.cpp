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

		im.addKeyMap("enter", SDL_SCANCODE_RETURN);
		im.addKeyMap("enter", SDL_SCANCODE_SPACE);

		//arrow keys
		im.addKeyMap("up", SDL_SCANCODE_UP);
		im.addKeyMap("down", SDL_SCANCODE_DOWN);
		im.addKeyMap("left", SDL_SCANCODE_LEFT);
		im.addKeyMap("right", SDL_SCANCODE_RIGHT);


		im.addKeyMap("debugfogofwar", SDL_SCANCODE_Z);

		im.addMouseMap("leftmouse", SDL_BUTTON_LEFT);

		//im.addMouseMap("left", SDL_BUTTON_LEFT);


		htpSoul.setPosition(Tmpl8::vec2(735.f, 75.f));
		htpSoul.setAttractor(Tmpl8::vec2(785.f, 125.f), 450.f);
	}

	void Game::Shutdown()
	{
	}
	

	


	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{

		switch (currentState)
		{
		case Tmpl8::Game::gameState::TitleScreen:
			titleScreenTick(deltaTime);
			break;
		case Tmpl8::Game::gameState::HelpScreen:
			helpScreenTick(deltaTime);
			break;

		case Tmpl8::Game::gameState::GameScreen:
			gameScreenTick(deltaTime);
			break;

		case Tmpl8::Game::gameState::DeathScreen:
			deathScreenTick(deltaTime);
			break;

		case Tmpl8::Game::gameState::VictoryScreen:
			victoryScreenTick(deltaTime);
			break;

		default:
			break;
		}

	
		mainCamera.renderToSurface(screen);
	
		//screen->Print("Hallo", im.getLocalMouse().x, im.getLocalMouse().y, 0xffff00, 10);

		int mX = im.getLocalMouse().x;
		int mY = im.getLocalMouse().y;


		// update inputmanager for next keypresses
		im.update(mainCamera);
	}

	void Game::loadGame(std::string_view levelPath)
	{
		delete game;
		game = new SoulSweep(levelPath);
		currentState = gameState::GameScreen;
		
		
	}

	void Game::titleScreenTick(float deltaTime)
	{
		mainCamera.clearScreen(0x1f1d1d);
		// drawing position can calculated once in the init function and saved instead of being calculated every frame
		Tmpl8::vec2 drawPos = mainCamera.getSurface()->GetTextCenterPosition("SoulSweep", 50, 10);

		mainCamera.getSurface()->Print("SoulSweep", static_cast<int>(drawPos.x)-2, static_cast<int>(drawPos.y)-2, 0xffffff, 10);
		mainCamera.getSurface()->Print("SoulSweep", static_cast<int>(drawPos.x) - 1, static_cast<int>(drawPos.y) - 1, 0xffffff, 10);
		mainCamera.getSurface()->Print("SoulSweep", static_cast<int>(drawPos.x), static_cast<int>(drawPos.y), 0x55abd4, 10);

		if (mainScreenSelectedMenu == 0)
		{
			mainCamera.getSurface()->PrintCenter("( Start Game )", 300, 0xc7d5eb, 4);
			mainCamera.getSurface()->PrintCenter("How to play", 350, 0xc7d5eb, 4);
		}
		else
		{
			mainCamera.getSurface()->PrintCenter("Start Game", 300, 0xc7d5eb, 4);
			mainCamera.getSurface()->PrintCenter("( How to play )", 350, 0xc7d5eb, 4);
		}
			

		if (im.isActionReleased("down") || im.isActionReleased("up"))
		{
			// actually for up +2 -1 (+2 because to get positive modulo then -1 because we want to go back, leading to the same as ++;
			mainScreenSelectedMenu++;
			mainScreenSelectedMenu %= 2;
		}


		if (im.isActionReleased("enter") )
		{
			if (mainScreenSelectedMenu == 0)
				loadGame("assets/Maps/level1.json");
			else
				currentState = gameState::HelpScreen;
		}

	}

	void Game::helpScreenTick(float deltaTime)
	{
		htpSoul.updateParticles(deltaTime);

		mainCamera.clearScreen(0x1f1d1d);
		howToPlayTexture.CopyToSurface(mainCamera.getSurface(), 0, 0);
		htpSoul.renderParticles(mainCamera);
		mainCamera.getSurface()->PrintCenter("( back to title screen )", 450, 0xc7d5eb, 3);

		if (im.isActionReleased("enter"))
			currentState = gameState::TitleScreen;
	}

	void Game::gameScreenTick(float deltaTime)
	{
		if (game == nullptr)	// if no game is loaded something went wrong, since we can't have in the gamescreenState
			return;
		
		game->update(deltaTime, im, mainCamera);

		// rendering
		mainCamera.clearScreen(0x72751b);
		game->render(mainCamera);

		// state changes

		if (game->deathState())
			currentState = gameState::DeathScreen;

		if (game->victoryState())
			currentState = gameState::VictoryScreen;


	}

	void Game::deathScreenTick(float deltaTime)
	{
		mainCamera.clearScreen(0x1f1d1d);
		mainCamera.getSurface()->PrintCenter("You have been devoured", 100, 0xeb4034, 5);

		mainCamera.getSurface()->PrintCenter("Press ENTER to restart", 420, 0xc7d5eb, 4);

		if (im.isActionReleased("enter"))
			loadGame("assets/Maps/level1.json");
	}

	void Game::victoryScreenTick(float deltaTime)
	{
		mainCamera.clearScreen(0x1f1d1d);
		mainCamera.getSurface()->PrintCenter("Congratulations", 100, 0xcfad40, 5);
		mainCamera.getSurface()->PrintCenter("you have extracted enough souls", 160, 0xcfad40, 4);

		mainCamera.getSurface()->PrintCenter("Press ENTER to restart", 420, 0xc7d5eb, 4);

		if (im.isActionReleased("enter"))
			loadGame("assets/Maps/level1.json");
	}

};