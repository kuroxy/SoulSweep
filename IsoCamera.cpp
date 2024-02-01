#include "IsoCamera.hpp"
#include "Texture.hpp"
#include "SpriteSheet.hpp"
#include <cstdio> //printf


using namespace Tmpl8;

namespace IsoEngine{

IsoCamera::IsoCamera(int screenWidth, int screenHeight, int tileWidth)
	: m_position{ 0 }
	, m_cameraWidth{ screenWidth }
	, m_cameraHeight{ screenHeight }
	, m_cameraBuffer{ std::make_unique<Surface>(m_cameraWidth,m_cameraHeight)}
	, m_tileWidth{ tileWidth }
{
}

int IsoCamera::getWidth() const
{
	return m_cameraWidth;
}

int IsoCamera::getHeight() const
{
	return m_cameraHeight;
}

const Tmpl8::vec2 IsoCamera::getPosition() const
{
	return m_position;
}

Tmpl8::vec2 IsoCamera::worldToIsometric(const Tmpl8::vec2& worldSpace) const
{
	int halfWidth = m_tileWidth / 2;
	int quarterWidth = halfWidth / 2;

	// magic math
	float isoX = (worldSpace.x / halfWidth + worldSpace.y / quarterWidth) / 2;
	float isoY = worldSpace.y / quarterWidth - isoX;


	return Tmpl8::vec2(isoX, isoY);
}

Tmpl8::vec2 IsoCamera::isometricToWorld(const Tmpl8::vec2& isometricSpace) const
{
	int halfWidth = m_tileWidth / 2;
	int quarterWidth = halfWidth / 2;

	float worldX = (isometricSpace.x - isometricSpace.y) * halfWidth;
	float worldY = (isometricSpace.y + isometricSpace.x) * quarterWidth;

	return Tmpl8::vec2(worldX, worldY);
}


void IsoCamera::setPosition(const Tmpl8::vec2& pos)
{
	m_position = pos;
}

void IsoCamera::renderToSurface(Tmpl8::Surface* surface, int scale, int xOffset, int yOffset) const
{
	if (scale <= 0)
	{
		printf("IsoCamera::renderToSurface, Scale is smaller or equal to zero.");
		return;
	}

	Pixel* src = m_cameraBuffer->GetBuffer();
	Pixel* dst = surface->GetBuffer();

	int dstWidth = surface->GetWidth();
	int dstHeight = surface->GetHeight();

	// boundry checking
	if (xOffset > dstWidth || yOffset > dstHeight)
		return;

	if (xOffset+m_cameraWidth*scale < 0 || yOffset + m_cameraHeight * scale < 0)
		return;

	// clipping offscreen stuff
	int xMin = Max<int>(xOffset, 0);
	int xMax = Min<int>(xOffset + m_cameraWidth * scale, dstWidth);
	xMax = (xMax - xOffset) / scale + xOffset; // Transforming it back into non scaled

	int yMin = Max<int>(yOffset, 0);
	int yMax = Min<int>(yOffset + m_cameraHeight * scale, dstHeight);
	yMax = (yMax - yOffset) / scale + yOffset; // Transforming it back into non scaled

	// iew
	for (int y = 0; y < yMax-yMin; y++)
	{
		for (int x = 0; x < xMax - xMin; x++)
		{
			Tmpl8::Pixel c = src[y * m_cameraWidth + x]; // should be safe because it can only be clipped down.
		

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



void IsoCamera::blitTexture(Texture* texture, int x, int y)
{
	texture->CopyToSurface(m_cameraBuffer.get(), x, y);
}

void IsoCamera::renderTextureWorldSpace(Texture* texture, float xWorldSpace, float yWorldSpace)
{
	int localSpaceX = xWorldSpace - m_position.x;
	int localSpaceY = yWorldSpace - m_position.y;

	texture->CopyToSurface(m_cameraBuffer.get(), localSpaceX, localSpaceY);
}

void IsoCamera::renderTextureWorldSpace(Texture* texture, const Tmpl8::vec2& WorldSpace)
{
	renderTextureWorldSpace(texture, WorldSpace.x, WorldSpace.y);
}

void IsoCamera::renderTextureIsometric(Texture* texture, float xIsoSpace, float yIsoSpace)
{
	renderTextureIsometric(texture, vec2{ xIsoSpace,yIsoSpace });
}

void IsoCamera::renderTextureIsometric(Texture* texture, const Tmpl8::vec2& IsoSpace)
{
	vec2 worldSpace = isometricToWorld(IsoSpace);
	renderTextureIsometric(texture, worldSpace);
}

void IsoCamera::renderSpriteWorldSpace(SpriteSheet* spritesheet, int x, int y, const Tmpl8::vec2& worldSpace)
{
	vec2 local = worldToIsometric(worldSpace);

	spritesheet->drawSprite(m_cameraBuffer.get(), x, y, (int)local.x, (int)local.y);
}

void IsoCamera::renderSpriteIsometric(SpriteSheet* spriteSheet, int x, int y, const Tmpl8::vec2& isoSpace)
{
	vec2 worldSpace = isometricToWorld(isoSpace);
	renderSpriteWorldSpace(spriteSheet, x, y, worldSpace);
}

};