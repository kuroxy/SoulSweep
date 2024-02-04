#include "Texture.hpp"
#include "FreeImage.h"
#include "surface.h"
#include "template.h"

namespace Engine
{

Texture::Texture(const std::string& filename)
{
	const char* filecstr = filename.c_str();
	// thank you surface.h
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(filecstr, 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filecstr);
	FIBITMAP* tmp = FreeImage_Load(fif, filecstr);
	FIBITMAP* dib = FreeImage_ConvertTo32Bits(tmp);
	FreeImage_Unload(tmp);
	m_width = FreeImage_GetWidth(dib);
	m_height = FreeImage_GetHeight(dib);
	m_buffer = std::make_unique<Tmpl8::Pixel[]>(m_width * m_height);

	Tmpl8::Pixel* raw_pointer = m_buffer.get();
	if (raw_pointer)
	{
		for (int y = 0; y < m_height; y++)
		{
			if (m_width != 0)
			{
				unsigned char* line = FreeImage_GetScanLine(dib, m_height - 1 - y);
				memcpy(raw_pointer + (y * m_width), line, m_width * sizeof(Tmpl8::Pixel));
			}
		}
	}
	FreeImage_Unload(dib);
}

Texture::Texture(int width, int height, Tmpl8::Pixel color)
	: m_width{ width }
	, m_height{ height }
	, m_buffer{ std::make_unique<Tmpl8::Pixel[]>(m_width * m_height) }
{
	for (int i = 0; i < m_width*m_height; i++) // fill buffer with given color
	{
		m_buffer[i] = color;
	}
}

int Texture::getWidth() const
{
	return m_width;
}

int Texture::getHeight() const
{
	return m_height;
}

bool Texture::hasTransparency() const
{
	return m_transparent;
}

void Texture::setChromaKey(Tmpl8::Pixel colorKey)
{
	bool changedPixel = false;

	//loops through all pixels checks if they are the same as colorKey, if so set alpha channel to 0 and m_transparent to true
	for (int i = 0; i < m_width*m_height; i++)
	{
		Tmpl8::Pixel pixel = m_buffer[i]&0xffffff;
		if (pixel == colorKey)
		{
			m_buffer[i] = pixel;
			changedPixel = true;
		}

	}

	m_transparent = changedPixel;
}

void Texture::CopyToSurface(Tmpl8::Surface* surface, int x, int y) const
{
	// if there is a transparent pixel use CopyTransparent else CopyOpaque since copyOpaque is faster?! (Is memcopy faster then for loop?)
	if (m_transparent)
		CopyTransparent(surface, x, y);
	else
		CopyOpaque(surface, x, y);
}

void Texture::CopyTransparent(Tmpl8::Surface* surface, int x, int y) const
{
	// based on Tmpl8::surface
	Tmpl8::Pixel* dst = surface->GetBuffer();
	Tmpl8::Pixel* src = m_buffer.get();
	if ((src) && (dst))
	{
		int srcwidth = m_width;
		int srcheight = m_height;
		int dstwidth = surface->GetWidth();
		int dstheight = surface->GetHeight();

		if ((srcwidth + x) > dstwidth)
			srcwidth = dstwidth - x;

		if ((srcheight + y) > dstheight)
			srcheight = dstheight - y;

		if (x < 0)
			src -= x, srcwidth += x, x = 0;

		if (y < 0)
			src -= y * m_width, srcheight += y, y = 0;


		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += x + dstwidth * y;
			for (int iy = 0; iy < srcheight; iy++)
			{
				for (int ix = 0; ix < srcwidth; ix++)
				{
					if ((src[ix] & 0xff000000) ) // if this is not zero then alpha channel is not 0
						dst[ix] = src[ix];
					
				}
				dst += dstwidth;
				src += m_width;
			}
		}
	}
}

void Texture::CopyOpaque(Tmpl8::Surface* surface, int x, int y) const
{
	// based on Tmpl8::surface
	Tmpl8::Pixel* dst = surface->GetBuffer();
	Tmpl8::Pixel* src = m_buffer.get();
	if ((src) && (dst))
	{
		int srcwidth = m_width;
		int srcheight = m_height;
		int dstwidth = surface->GetWidth();
		int dstheight = surface->GetHeight();

		if ((srcwidth + x) > dstwidth) 
			srcwidth = dstwidth - x;

		if ((srcheight + y) > dstheight) 
			srcheight = dstheight - y;

		if (x < 0) 
			src -= x, srcwidth += x, x = 0;

		if (y < 0) 
			src -= y * m_width, srcheight += y, y = 0;


		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += x + dstwidth * y;
			for (int y = 0; y < srcheight; y++)
			{
				memcpy(dst, src, srcwidth * 4);
				dst += dstwidth;
				src += m_width;
			}
		}
	}
}

void Texture::PartialCopyToSurface(Tmpl8::Surface* surface, int xDst, int yDst, int x1, int y1, int x2, int y2, bool useTransparency)
{
	Tmpl8::Pixel* src = surface->GetBuffer();
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

	// iew
	for (int y = 0; y < yMax - yMin; y++)
	{
		for (int x = 0; x < xMax - xMin; x++)
		{
			Tmpl8::Pixel c = src[y * (x2 - x1) + x]; // should be safe because it was clipped down

			if (!(c & 0xff000000) && useTransparency)
				continue;

			dst[yDst * dstWidth + xDst] = c;
			

		}
	}
}

};