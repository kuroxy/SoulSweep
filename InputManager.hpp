#pragma once
#include <string>
#include <unordered_map>

namespace Engine {

	class InputManager
	{
	public:
		InputManager() = default;

		bool isActionDown(const std::string& actionName) const;
		bool isActionUp(const std::string& actionName) const;

		void addKeyMap(std::string actionName, int SDL_Scancode); //https://wiki.libsdl.org/SDL2/SDLScancodeLookup
		void addMouseMap(std::string actionName, int SDL_MouseButton); //https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent

		void handleMouseUp(int button);
		void handleMouseDown(int button);
		void handleKeyUp(int key);
		void handleKeyDown(int key);

		void clear();

	private:
		std::unordered_multimap<int, std::string> m_keyMapping;
		std::unordered_multimap<int, std::string> m_mouseMapping;

		std::unordered_map<std::string, bool> m_actionDown;
		std::unordered_map<std::string, bool> m_actionUp;

	};

}