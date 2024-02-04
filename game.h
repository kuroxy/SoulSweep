#pragma once
#include "InputManager.hpp"


namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );

	void MouseMove(int x, int y) { mouseX = x, mouseY = y; }

	void MouseUp(int button) { im.handleMouseUp(button); }
	void MouseDown(int button) { im.handleMouseDown(button); }
	void KeyUp(int key) { im.handleKeyUp(key); }
	void KeyDown(int key) { im.handleKeyDown(key); }
private:
	Surface* screen;
	Engine::InputManager im;
	int mouseX;
	int mouseY;
};

}; // namespace Tmpl8