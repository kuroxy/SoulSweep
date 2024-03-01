#pragma once
#include <string>
#include <unordered_map>
#include "template.h"


namespace Engine {

	class Camera; 

	class InputManager
	{
	public:
		InputManager() = default;

		// isActionPressed is true on the the first frame where this action is pressed.
		// isActionReleased is only true on the the frame where this action is released.
		// isActionHeld is true when the button is down/held.
		bool isActionPressed(const std::string& actionName) const;
		bool isActionReleased(const std::string& actionName) const;
		bool isActionHeld(const std::string& actionName) const;

		void addKeyMap(const std::string&, int SDL_Scancode); //https://wiki.libsdl.org/SDL2/SDLScancodeLookup
		void addMouseMap(const std::string&, int SDL_MouseButton); //https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent

		void handleMouseUp(int button);
		void handleMouseDown(int button);
		void handleKeyUp(int key);
		void handleKeyDown(int key);

		const Tmpl8::vec2& getLocalMouse() const { return Tmpl8::vec2((float)m_mouseX, (float)m_mouseY); }
		const Tmpl8::vec2& getWorldMouse() const { return m_mouseWorld; }

		void setMousePos(int x, int y, const Camera& c);
		void update(const Camera& c);


	private:
		std::unordered_multimap<int, std::string> m_keyMapping;
		std::unordered_multimap<int, std::string> m_mouseMapping;

		std::unordered_map<std::string, bool> m_actionDown;
		std::unordered_map<std::string, bool> m_actionUp;
		std::unordered_map<std::string, bool> m_actionPressed;

		int m_mouseX{ 0 };
		int m_mouseY{ 0 };

		Tmpl8::vec2 m_mouseWorld{ 0 };

	};

}