#pragma once
#include <memory>
#include <string>
#include "surface.h"

namespace IsoEngine {


class Texture
{
public:
	Texture(const std::string& filename);
	Texture(int width, int height, Tmpl8::Pixel color);

	int getWidth() const;
	int getHeight() const;

	bool hasTransparency() const;

	void setChromaKey(Tmpl8::Pixel colorKey);


	// copies texture to surface(with transparency if chromakey is set) !!this should be normally used!!
	void CopyToSurface(Tmpl8::Surface* surface, int x, int y) const;

	// copies texture to surface with transparency
	void CopyTransparent(Tmpl8::Surface* surface, int x, int y) const;

	// copies every pixel to surface even if chromakeyed
	void CopyOpaque(Tmpl8::Surface* surface, int x, int y) const;


private:
	int m_width{ 0 }, m_height{ 0 };
	std::unique_ptr<Tmpl8::Pixel[]> m_buffer;

	bool m_transparent{ false };
};

};
