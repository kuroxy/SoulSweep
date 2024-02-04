#pragma once
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "Player.hpp"

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

	void MouseUp(int button);
	void MouseDown(int button);
	void KeyUp(int key);
	void KeyDown(int key);
private:
	Surface* screen;
	int mouseX;
	int mouseY;
};

}; // namespace Tmpl8