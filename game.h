#pragma once
#include "InputManager.hpp"
#include "Camera.hpp"
#include "SoulSweep.hpp"
#include "Config.hpp"


namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );

	void MouseMove(int x, int y) {
		im.setMousePos
		(x, y, mainCamera);
	}

	void MouseUp(int button) { im.handleMouseUp(button); }
	void MouseDown(int button) { im.handleMouseDown(button); }
	void KeyUp(int key) { im.handleKeyUp(key); }
	void KeyDown(int key) { im.handleKeyDown(key); }

	void loadGame(std::string_view levelPath);
private:
	enum class gameState
	{
		TitleScreen,
		HelpScreen,
		GameScreen,
		DeathScreen,
		VictoryScreen
	};

	void titleScreenTick(float deltaTime);
	void helpScreenTick(float deltaTime);
	void gameScreenTick(float deltaTime);
	void deathScreenTick(float deltaTime);
	void victoryScreenTick(float deltaTime);

	Surface* screen;

	Engine::InputManager im;
	Engine::Camera mainCamera{ ScreenWidth, ScreenHeight};

	gameState currentState = gameState::TitleScreen;
	SoulSweep* game = nullptr;

	int mainScreenSelectedMenu = 0;


	Engine::Texture howToPlayTexture{ "assets/Textures/htp.png" }; // how to play instruction Image

	Engine::BaseParticleSystem htpSoul{ Config::soulParticleParams,  50 }; // Soul example in the instruction image

};

}; // namespace Tmpl8