#include "InputManager.hpp"
#include "Camera.hpp"
#include <cstdio>

namespace Engine
{
bool InputManager::isActionDown(const std::string& actionName) const
{
	auto action = m_actionDown.find(actionName);

	if (action != m_actionDown.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.c_str());
	return false; 
}

bool InputManager::isActionUp(const std::string& actionName) const
{
	auto action = m_actionUp.find(actionName);

	if (action != m_actionUp.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.c_str());
	return false; 
}

bool InputManager::isActionPressed(const std::string& actionName) const
{
	auto action = m_actionPressed.find(actionName);

	if (action != m_actionPressed.end())
		return action->second;

	printf("'%s', action not found!\n", actionName.c_str());
	return false;
}

void InputManager::addKeyMap(std::string actionName, int SDL_Scancode)
{
	m_keyMapping.insert({ SDL_Scancode,actionName });
	m_actionDown[actionName] = false;
	m_actionUp[actionName] = false;
	m_actionPressed[actionName] = false;

}

void InputManager::addMouseMap(std::string actionName, int SDL_MouseButton)
{
	m_mouseMapping.insert({ SDL_MouseButton,actionName });
	m_actionDown[actionName] = false;
	m_actionUp[actionName] = false;
	m_actionPressed[actionName] = false;
}

void InputManager::handleMouseUp(int button)
{
	auto range = m_mouseMapping.equal_range(button);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionUp[it->second] = true;
		m_actionPressed[it->second] = false;
	}
}

void InputManager::handleMouseDown(int button)
{
	auto range = m_mouseMapping.equal_range(button);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionDown[it->second] = true;
		m_actionPressed[it->second] = true;
	}
}

void InputManager::handleKeyUp(int key)
{
	auto range = m_keyMapping.equal_range(key);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionUp[it->second] = true;
		m_actionPressed[it->second] = false;
	}
}

void InputManager::handleKeyDown(int key)
{
	auto range = m_keyMapping.equal_range(key);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionDown[it->second] = true;
		m_actionPressed[it->second] = true;
	}
}

void InputManager::setMousePos(int x, int y, const Camera& c)
{
	m_mouseX = x;
	m_mouseY = y;
	m_mouseWorld = c.localToWorld(Tmpl8::vec2(m_mouseX, m_mouseY));
}

void InputManager::update(const Camera& c)
{
	m_mouseWorld = c.localToWorld(Tmpl8::vec2(m_mouseX, m_mouseY)); // safety because camera can move without that mouseMove event is called, because the mouse didn't move. However the world mouse has.

	for (auto& action : m_actionDown) {
		action.second = false;
	}

	for (auto& action : m_actionUp) {
		action.second = false;
	}
}

}

