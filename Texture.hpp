#pragma once
#include <memory>
#include <string>
#include "surface.h"

namespace Engine {


class Texture
{
public:
	Texture() = default;
	Texture(std::string_view filename);
	Texture(std::string_view filename, Tmpl8::Pixel chromaColor);
	Texture(int width, int height, Tmpl8::Pixel color);

	int getWidth() const;
	int getHeight() const;

	bool hasTransparency() const;
		
	void setChromaKey(Tmpl8::Pixel colorKey);


	// copies texture to surface(with transparency if chromakey is set) !!this should be normally used!!
	void CopyToSurface(Tmpl8::Surface* surface, int x, int y) const;

	// copies texture to surface with transparency // maybe getting removed
	void CopyTransparent(Tmpl8::Surface* surface, int x, int y) const;

	// copies every pixel to surface even if chromakeyed // maybe getting removed
	void CopyOpaque(Tmpl8::Surface* surface, int x, int y) const;

	void PartialCopyToSurface(Tmpl8::Surface* surface, int xDst, int yDst, int x1, int y1, int x2, int y2, bool useTransparency = true, bool flip=false);


private:
	int m_width{ 0 }, m_height{ 0 };
	std::unique_ptr<Tmpl8::Pixel[]> m_buffer;

	bool m_transparent{ false };
};

};
