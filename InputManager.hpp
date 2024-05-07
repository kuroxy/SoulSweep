#pragma once

#include <string>
#include <unordered_map>

#include "template.h"


namespace Engine {

class Camera; 


// Class to handle input based on actions rather then mouse or keyboard buttons
class InputManager
{
public:
	InputManager() = default;

	// isActionPressed is true on the the first frame where this action is pressed.
	// isActionReleased is only true on the the frame where this action is released.
	// isActionHeld is true when the button is down/held.
	bool isActionPressed(std::string_view actionName) const;
	bool isActionReleased(std::string_view actionName) const;
	bool isActionHeld(std::string_view actionName) const;


	void addKeyMap(std::string_view, int SDL_Scancode); //https://wiki.libsdl.org/SDL2/SDLScancodeLookup
	void addMouseMap(std::string_view, int SDL_MouseButton); //https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent
	
	void handleMouseUp(int button);
	void handleMouseDown(int button);
	void handleMouseMovement(int x, int y, const Camera& c);

	void handleKeyUp(int key);
	void handleKeyDown(int key);


	Tmpl8::vec2 getLocalMouse() const { return Tmpl8::vec2((float)mousePositionX, (float)mousePositionY); }
	const Tmpl8::vec2& getWorldMouse() const { return mouseWorldPosition; }
	
	void update(const Camera& c);


private:

	// one button can have more actions or two buttons can have the same action, this is why we need a multimap
	std::unordered_multimap<int, std::string> keyMapping;
	std::unordered_multimap<int, std::string> mouseMapping;

	// Could have been an enum instead of a bool, but pressed and held should be true the same time, so for now is 3 variables
	std::unordered_map<std::string, bool> actionPressed;
	std::unordered_map<std::string, bool> actionReleased;
	std::unordered_map<std::string, bool> actionHeld;

	int mousePositionX{ 0 };
	int mousePositionY{ 0 };

	Tmpl8::vec2 mouseWorldPosition{ 0 };

};

}