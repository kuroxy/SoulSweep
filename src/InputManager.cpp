#include "InputManager.hpp"
#include "Camera.hpp"
#include <cstdio>

namespace Engine
{
bool InputManager::isActionPressed(std::string_view actionName) const
{

	auto action = actionPressed.find(std::string{ actionName });

	if (action != actionPressed.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.data());
	return false; 
}

bool InputManager::isActionReleased(std::string_view actionName) const
{
	auto action = actionReleased.find(std::string{ actionName });

	if (action != actionReleased.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.data());
	return false; 
}

bool InputManager::isActionHeld(std::string_view actionName) const
{
	auto action = actionHeld.find(std::string{ actionName });

	if (action != actionHeld.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.data());
	return false;
}

void InputManager::addKeyMap(std::string_view actionName, int SDL_Scancode)
{
	std::string action = std::string{ actionName };
	keyMapping.insert({ SDL_Scancode, action });
	actionPressed[action] = false;
	actionReleased[action] = false;
	actionHeld[action] = false;

}

void InputManager::addMouseMap(std::string_view actionName, int SDL_MouseButton)
{
	std::string action = std::string{ actionName };
	mouseMapping.insert({ SDL_MouseButton, action });
	actionPressed[action] = false;
	actionReleased[action] = false;
	actionHeld[action] = false;
}

void InputManager::handleMouseUp(int button)
{
	auto range = mouseMapping.equal_range(button);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		actionReleased[it->second] = true;
		actionHeld[it->second] = false;
	}
}

void InputManager::handleMouseDown(int button)
{
	auto range = mouseMapping.equal_range(button);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		actionPressed[it->second] = true;
		actionHeld[it->second] = true;
	}
}

void InputManager::handleKeyUp(int key)
{
	auto range = keyMapping.equal_range(key);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		actionReleased[it->second] = true;
		actionHeld[it->second] = false;
	}
}

void InputManager::handleKeyDown(int key)
{
	auto range = keyMapping.equal_range(key);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		actionPressed[it->second] = true;
		actionHeld[it->second] = true;
	}
}

void InputManager::handleMouseMovement(int x, int y, const Camera& c)
{
	mousePositionX = x;
	mousePositionY = y;
	mouseWorldPosition = c.localToWorld(Tmpl8::vec2((float)mousePositionX, (float)mousePositionY));
}

void InputManager::update(const Camera& c)
{
	// We need to update the worldposition, normally would be done in the handleMouseMovement,
	// but camera can move so then worldspace position should be updates. 
	mouseWorldPosition = c.localToWorld(Tmpl8::vec2((float)mousePositionX, (float)mousePositionY)); 

	for (auto& action : actionPressed) {
		action.second = false;
	}

	for (auto& action : actionReleased) {
		action.second = false;
	}
}

}

