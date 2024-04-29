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
		bool isActionPressed(std::string_view actionName) const;
		bool isActionReleased(std::string_view actionName) const;
		bool isActionHeld(std::string_view actionName) const;

		void addKeyMap(std::string_view, int SDL_Scancode); //https://wiki.libsdl.org/SDL2/SDLScancodeLookup
		void addMouseMap(std::string_view, int SDL_MouseButton); //https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent

		void handleMouseUp(int button);
		void handleMouseDown(int button);
		void handleKeyUp(int key);
		void handleKeyDown(int key);

		Tmpl8::vec2 getLocalMouse() const { return Tmpl8::vec2((float)m_mouseX, (float)m_mouseY); }
		const Tmpl8::vec2& getWorldMouse() const { return m_mouseWorld; }

		void setMousePos(int x, int y, const Camera& c);
		void update(const Camera& c);


	private:
		// one button can have more actions or two buttons can have the same action, this is why we need a multimap
		std::unordered_multimap<int, std::string> m_keyMapping;
		std::unordered_multimap<int, std::string> m_mouseMapping;

		std::unordered_map<std::string, bool> m_actionPressed;
		std::unordered_map<std::string, bool> m_actionReleased;
		std::unordered_map<std::string, bool> m_actionHeld;

		int m_mouseX{ 0 };
		int m_mouseY{ 0 };

		Tmpl8::vec2 m_mouseWorld{ 0 };

	};

}