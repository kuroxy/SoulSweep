#include "Camera.hpp"
#include "Texture.hpp"
#include "SpriteSheet.hpp"
#include <cstdio> //printf
#include <cmath>

using namespace Tmpl8;

namespace Engine {


Tmpl8::vec2 Camera::worldToScreen(const Tmpl8::vec2& worldPosition) const
{
	return worldPosition - position;
}


Tmpl8::vec2 Camera::localToWorld(const Tmpl8::vec2& localSpace) const
{
	return Tmpl8::vec2((float)(int)localSpace.x + position.x, (float)(int)localSpace.y + position.y);
}

void Camera::drawPixel(const Tmpl8::vec2& worldPosition, Tmpl8::Pixel color)
{
	Tmpl8::vec2 screenPos = worldToScreen(worldPosition);

	cameraSurface->Plot(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), color);
}

void Camera::drawLine(const Tmpl8::vec2& worldPos1, const Tmpl8::vec2& worldPos2, Tmpl8::Pixel color)
{
	Tmpl8::vec2 screenPos1 = worldToScreen(worldPos1);
	Tmpl8::vec2 screenPos2 = worldToScreen(worldPos2);

	cameraSurface->Line(screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y, color);
}

void Camera::drawRectangle(const Tmpl8::vec2& worldPos1, const Tmpl8::vec2& worldPos2, Tmpl8::Pixel color, int width)
{

	Tmpl8::vec2 screenPos1 = worldToScreen(worldPos1);
	Tmpl8::vec2 screenPos2 = worldToScreen(worldPos2);


	cameraSurface->Rectangle(static_cast<int>(screenPos1.x), static_cast<int>(screenPos1.y),
		static_cast<int>(screenPos2.x), static_cast<int>(screenPos2.y), color, width);
}

void Camera::drawCircle(const Tmpl8::vec2& worldPosition, float radius, Tmpl8::Pixel color, int width)
{
	Tmpl8::vec2 screenPos = worldToScreen(worldPosition);

	cameraSurface->Circle(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), static_cast<int>(radius), color, width);
}

void Camera::drawText(std::string_view text, const Tmpl8::vec2& worldPosition, Tmpl8::Pixel color, int width)
{
	Tmpl8::vec2 screenPos = worldToScreen(worldPosition);

	cameraSurface->Print(text, static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), color, width);
}



void Camera::darkenPixel(int x, int y, int amount)
{
	Tmpl8::Pixel color = cameraSurface->GetBuffer()[x + y * getWidth()];
	int r = (color&0xff0000)>>16;
	int g = (color&0x00ff00)>>8;
	int b = color&0x0000ff;

	float f = Tmpl8::Clamp(255-amount, 0, 255) / 255.0f;
	r = static_cast<int>(r * f);
	g = static_cast<int>(g * f);
	b = static_cast<int>(b * f);
	color = (r << 16) | (g << 8) | b;
	cameraSurface->GetBuffer()[x + y * getWidth()] = color;
}


void Camera::renderToSurface(Tmpl8::Surface* surface, int scale, int xOffset, int yOffset) const
{
	if (scale <= 0)
	{
		printf("Camera::renderToSurface, Scale is smaller or equal to zero.");
		return;
	}

	Pixel* src = cameraSurface->GetBuffer();
	Pixel* dst = surface->GetBuffer();

	int dstWidth = surface->GetWidth();
	int dstHeight = surface->GetHeight();

	// boundry checking
	if (xOffset > dstWidth || yOffset > dstHeight)
		return;

	if (xOffset+getWidth()*scale < 0 || yOffset + getHeight() * scale < 0)
		return;

	// clipping offscreen stuff
	int xMin = Max<int>(xOffset, 0);
	int xMax = Min<int>(xOffset + getWidth() * scale, dstWidth);
	xMax = (xMax - xOffset) / scale + xOffset; // Transforming it back into non scaled

	int yMin = Max<int>(yOffset, 0);
	int yMax = Min<int>(yOffset + getHeight() * scale, dstHeight);
	yMax = (yMax - yOffset) / scale + yOffset; // Transforming it back into non scaled

	// iew
	for (int y = 0; y < yMax-yMin; y++)
	{
		for (int x = 0; x < xMax - xMin; x++)
		{
			Tmpl8::Pixel c = src[y * getWidth() + x]; // should be safe because it can only be clipped down.
		

			for (int iy = 0; iy < scale; iy++)
			{
				int yDst = (y+yMin) * scale + xOffset + iy;

				if (yDst >= dstHeight)
					break;

				for (int ix = 0; ix < scale; ix++)
				{
					int xDst = (x+xMin) * scale + xOffset + ix;
					
					if (xDst >= dstWidth) 
						break;

					
					dst[yDst * dstWidth + (xDst)] = c;
				}
			}
		}
	}
}







void Camera::drawBarDarken(int x1, int y1, int x2, int y2, int amount)
{
	x1 = Tmpl8::Clamp(x1, 0, getWidth() - 1);
	x2 = Tmpl8::Clamp(x2, 0, getWidth() - 1);

	y1 = Tmpl8::Clamp(y1, 0, getHeight() - 1);
	y2 = Tmpl8::Clamp(y2, 0, getHeight() - 1);

	for (int y = y1; y <= y2; y++)
	{
		for (int x = x1; x <= x2; x++)
		{
			darkenPixel(x, y, amount);
		}
	}
}

void Camera::drawBarDarkenWorldSpace(const Tmpl8::vec2& position1, const Tmpl8::vec2& position2, int amount)
{
	vec2 local = worldToScreen(position1);
	vec2 local2 = worldToScreen(position2);
	drawBarDarken((int)local.x, (int)local.y, (int)local2.x, (int)local2.y, amount);
}

};