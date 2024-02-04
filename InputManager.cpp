#include "InputManager.hpp"
#include <cassert>


namespace Engine
{
bool InputManager::isActionDown(const std::string& actionName) const
{
	auto action = m_actionDown.find(actionName);

	if (action != m_actionDown.end())
		return action->second;

	assert(false && "Key not found!");  // Assertion message
	return false;  // Default value if the key is not found
}

bool InputManager::isActionUp(const std::string& actionName) const
{
	auto action = m_actionUp.find(actionName);

	if (action != m_actionUp.end())
		return action->second;

	assert(false && "Key not found!");  // Assertion message
	return false;  // Default value if the key is not found
}

void InputManager::addKeyMap(std::string actionName, int SDL_Scancode)
{
	m_keyMapping.insert({ SDL_Scancode,actionName });
	m_actionDown[actionName] = false;
	m_actionUp[actionName] = false;

}

void InputManager::addMouseMap(std::string actionName, int SDL_MouseButton)
{
	m_mouseMapping.insert({ SDL_MouseButton,actionName });
	m_actionDown[actionName] = false;
	m_actionUp[actionName] = false;
}

void InputManager::handleMouseUp(int button)
{
	auto range = m_mouseMapping.equal_range(button);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionUp[it->second] = true;
	}
}

void InputManager::handleMouseDown(int button)
{
	auto range = m_mouseMapping.equal_range(button);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionDown[it->second] = true;
	}
}

void InputManager::handleKeyUp(int key)
{
	auto range = m_keyMapping.equal_range(key);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionUp[it->second] = true;
	}
}

void InputManager::handleKeyDown(int key)
{
	auto range = m_keyMapping.equal_range(key);

	// Iterate over the range and perform some operation
	for (auto it = range.first; it != range.second; ++it) {
		m_actionDown[it->second] = true;
	}
}

void InputManager::clear()
{
	for (auto& action : m_actionDown) {
		action.second = false;
	}

	for (auto& action : m_actionUp) {
		action.second = false;
	}
}


}

