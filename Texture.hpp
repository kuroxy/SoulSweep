#pragma once
#include <memory>
#include <string>
#include "surface.h"

namespace Engine {

class Camera;

class Texture
{
public:
	Texture() = delete;
	Texture(std::string_view filename);
	Texture(std::string_view filename, Tmpl8::Pixel chromaColor);
	Texture(int width, int height, Tmpl8::Pixel color);

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	bool hasTransparency() const { return isTransparent; }
	
	// setChromaKey sets that color to transparent then set isTranspartent to true
	void setChromaKey(Tmpl8::Pixel colorKey);


	// copies a part of the texture to a surface,
	void PartialCopyToSurface(Tmpl8::Surface* surface, int xDst, int yDst, int x1, int y1, int x2, int y2, bool forceOpaque = false, bool flip = false);

	// copies texture to surface(with transparency if chromakey is set) if forceOpaque is true is just copies without transparency
	void draw(Tmpl8::Surface* surface, int x, int y, bool forceOpaque=false) const;
	void draw(const Camera& camera, const Tmpl8::vec2& WorldSpace);


private:
	int width{ 0 };
	int height{ 0 };
	std::unique_ptr<Tmpl8::Pixel[]> pixelBuffer;

	bool isTransparent{ false };
};

};
