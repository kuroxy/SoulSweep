#include "Texture.hpp"
#include "FreeImage.h"
#include "surface.h"
#include "template.h"
#include "Camera.hpp"
#include <stdexcept> // For std::runtime_error

namespace Engine
{

Texture::Texture(std::string_view filename)
{
	std::string filstr = std::string{ filename };
	const char* filecstr = filstr.c_str();
	// thank you surface.h
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(filecstr, 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filecstr);
	FIBITMAP* tmp = FreeImage_Load(fif, filecstr);
	FIBITMAP* dib = FreeImage_ConvertTo32Bits(tmp);
	FreeImage_Unload(tmp);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	pixelBuffer = std::make_unique<Tmpl8::Pixel[]>(width * height);

	Tmpl8::Pixel* raw_pointer = pixelBuffer.get();
	if (raw_pointer)
	{
		for (int y = 0; y < height; y++)
		{
			if (width != 0)
			{
				unsigned char* line = FreeImage_GetScanLine(dib, height - 1 - y);
				memcpy(raw_pointer + (y * width), line, width * sizeof(Tmpl8::Pixel));
			}
		}
	}
	FreeImage_Unload(dib);

	if (width == 0 && height == 0)
	{
		throw std::runtime_error("Failed to load texture: " + filstr);
	}

}

Texture::Texture(std::string_view filename, Tmpl8::Pixel chromaColor)
	: Texture(filename)
{
	
	setChromaKey(chromaColor);
}

Texture::Texture(int width, int height, Tmpl8::Pixel color)
	: width{ width }
	, height{ height }
	, pixelBuffer{ std::make_unique<Tmpl8::Pixel[]>(width * height) }
{
	for (int i = 0; i < width*height; i++) // fill buffer with given color
	{
		pixelBuffer[i] = color;
	}
}


void Texture::setChromaKey(Tmpl8::Pixel colorKey)
{
	bool changedPixel = false;

	//loops through all pixels checks if they are the same as colorKey, if so set alpha channel to 0 and m_transparent to true
	for (int i = 0; i < width*height; i++)
	{
		Tmpl8::Pixel pixel = pixelBuffer[i]&0xffffff;
		if (pixel == colorKey)
		{
			pixelBuffer[i] = pixel;
			changedPixel = true;
		}

	}

	isTransparent = changedPixel;
}

void Texture::draw(Tmpl8::Surface* surface, int x, int y, bool forceOpaque) const
{
	Tmpl8::Pixel* dst = surface->GetBuffer();
	Tmpl8::Pixel* src = pixelBuffer.get();
	if ((src) && (dst))
	{
		int srcwidth = width;
		int srcheight = height;
		int dstwidth = surface->GetWidth();
		int dstheight = surface->GetHeight();

		if ((srcwidth + x) > dstwidth)
			srcwidth = dstwidth - x;

		if ((srcheight + y) > dstheight)
			srcheight = dstheight - y;

		if (x < 0)
			src -= x, srcwidth += x, x = 0;

		if (y < 0)
			src -= y * width, srcheight += y, y = 0;


		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += x + dstwidth * y;
			for (int iy = 0; iy < srcheight; iy++)
			{
				for (int ix = 0; ix < srcwidth; ix++)
				{
					// there is no transparency or we force it to be opaque. Or last if the pixel does not contain any transparency
					if (!isTransparent || forceOpaque || (src[ix] & 0xff000000))
						dst[ix] = src[ix];
				}
				dst += dstwidth;
				src += width;
			}
		}
	}
}

void Texture::draw(const Camera& camera, const Tmpl8::vec2& worldSpace)
{
	Tmpl8::vec2 local = camera.worldToScreen(worldSpace);

	draw(camera.getSurface(), static_cast<int>(local.x), static_cast<int>(local.y));
}

void Texture::PartialCopyToSurface(Tmpl8::Surface* surface, int xDst, int yDst, int x1, int y1, int x2, int y2, bool forceOpaque, bool flip)
{
	Tmpl8::Pixel* src = pixelBuffer.get();
	Tmpl8::Pixel* dst = surface->GetBuffer();

	int dstWidth = surface->GetWidth();
	int dstHeight = surface->GetHeight();

	// boundry checking
	if (xDst > dstWidth || yDst > dstHeight)
		return;

	if (xDst + (x2-x1) < 0 || yDst + (y2 - y1) < 0)
		return;

	// clipping offscreen stuff
	int xMin = Tmpl8::Max<int>(xDst, 0);
	int xMax = Tmpl8::Min<int>(xDst + (x2 - x1), dstWidth);

	int yMin = Tmpl8::Max<int>(yDst, 0);
	int yMax = Tmpl8::Min<int>(yDst + (y2 - y1), dstHeight);

	
	for (int y = 0; y < yMax - yMin; y++)
	{
		for (int x = 0; x < xMax - xMin; x++)
		{
			int srcPixelX = x1 + x - xDst + xMin;
			int srcPixelY = y1 + y - yDst + yMin;

			Tmpl8::Pixel c = src[srcPixelY * width + srcPixelX]; // should be safe because it was clipped down

			// we skip if we, do have transparency and we dont force it to we opaque and the pixel is transparent.
			if ((isTransparent && !forceOpaque) && !(c & 0xff000000))
				continue;

			if (flip)
				dst[(y + yMin) * dstWidth + xMax - x] = c;
			else
				dst[(y + yMin) * dstWidth + xMin + x] = c;

		}

	}


}

};