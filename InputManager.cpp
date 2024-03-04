#include "InputManager.hpp"
#include "Camera.hpp"
#include <cstdio>

namespace Engine
{
bool InputManager::isActionPressed(std::string_view actionName) const
{

	auto action = m_actionPressed.find(std::string{ actionName });

	if (action != m_actionPressed.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.data());
	return false; 
}

bool InputManager::isActionReleased(std::string_view actionName) const
{
	auto action = m_actionReleased.find(std::string{ actionName });

	if (action != m_actionReleased.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.data());
	return false; 
}

bool InputManager::isActionHeld(std::string_view actionName) const
{
	auto action = m_actionHeld.find(std::string{ actionName });

	if (action != m_actionHeld.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.data());
	return false;
}

void InputManager::addKeyMap(std::string_view actionName, int SDL_Scancode)
{
	std::string action = std::string{ actionName };
	m_keyMapping.insert({ SDL_Scancode, action });
	m_actionPressed[action] = false;
	m_actionReleased[action] = false;
	m_actionHeld[action] = false;

}

void InputManager::addMouseMap(std::string_view actionName, int SDL_MouseButton)
{
	std::string action = std::string{ actionName };
	m_mouseMapping.insert({ SDL_MouseButton, action });
	m_actionPressed[action] = false;
	m_actionReleased[action] = false;
	m_actionHeld[action] = false;
}

void InputManager::handleMouseUp(int button)
{
	auto range = m_mouseMapping.equal_range(button);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionReleased[it->second] = true;
		m_actionHeld[it->second] = false;
	}
}

void InputManager::handleMouseDown(int button)
{
	auto range = m_mouseMapping.equal_range(button);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionPressed[it->second] = true;
		m_actionHeld[it->second] = true;
	}
}

void InputManager::handleKeyUp(int key)
{
	auto range = m_keyMapping.equal_range(key);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionReleased[it->second] = true;
		m_actionHeld[it->second] = false;
	}
}

void InputManager::handleKeyDown(int key)
{
	auto range = m_keyMapping.equal_range(key);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionPressed[it->second] = true;
		m_actionHeld[it->second] = true;
	}
}

void InputManager::setMousePos(int x, int y, const Camera& c)
{
	m_mouseX = x;
	m_mouseY = y;
	m_mouseWorld = c.localToWorld(Tmpl8::vec2((float)m_mouseX, (float)m_mouseY));
}

void InputManager::update(const Camera& c)
{
	m_mouseWorld = c.localToWorld(Tmpl8::vec2((float)m_mouseX, (float)m_mouseY)); // safety because camera can move without that mouseMove event is called, because the mouse didn't move. However the world mouse has.

	for (auto& action : m_actionPressed) {
		action.second = false;
	}

	for (auto& action : m_actionReleased) {
		action.second = false;
	}
}

}

