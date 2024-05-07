#pragma once

#include <memory>

#include "template.h"
#include "surface.h"


namespace Engine {


class Camera
{
public:
	Camera() = delete;
	Camera(int screenWidth, int screenHeight)
		: cameraSurface{ std::make_unique<Tmpl8::Surface>(screenWidth, screenHeight) }
	{
	}

	Tmpl8::Surface* getSurface() const { return cameraSurface.get(); }

	int getWidth() const { return cameraSurface->GetWidth(); }
	int getHeight() const { return cameraSurface->GetHeight(); }

	const Tmpl8::vec2& getPosition() const { return position; }
	void setPosition(const Tmpl8::vec2& pos) { position = pos; }


	// Special methods
	Tmpl8::vec2 worldToScreen(const Tmpl8::vec2& worldPosition) const;
	Tmpl8::vec2 localToWorld(const Tmpl8::vec2& localSpace) const;

		
	// Drawing methods
	void clearScreen(const Tmpl8::Pixel& color) { cameraSurface->Clear(color); }

	void drawPixel(const Tmpl8::vec2& position, Tmpl8::Pixel color);
	void drawLine(const Tmpl8::vec2& pos1, const Tmpl8::vec2& pos2, Tmpl8::Pixel color);
		
	void drawRectangle(const Tmpl8::vec2& pos1, const Tmpl8::vec2& pos2, Tmpl8::Pixel color, int width = 0); // width==0 filled in
	void drawCircle(const Tmpl8::vec2& position, float radius, Tmpl8::Pixel color, int width = 0); // width==0 filled in

	void drawText(std::string_view, const Tmpl8::vec2& position, Tmpl8::Pixel color, int width = 0);


	void darkenPixel(int x, int y, int amount);
	void drawBarDarken(int x1, int y1, int x2, int y2, int amount);
	void drawBarDarkenWorldSpace(const Tmpl8::vec2& position1, const Tmpl8::vec2& position2, int amount);


	// Render methods
	//Scaling before translation, scale must be positive
	void renderToSurface(Tmpl8::Surface* surface, int scale = 1, int xOffset = 0, int yOffset = 0) const; 


private:
	std::unique_ptr<Tmpl8::Surface> cameraSurface;
	Tmpl8::vec2 position = { 0.f };
};

};