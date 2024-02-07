#include "Camera.hpp"
#include "Texture.hpp"
#include "SpriteSheet.hpp"
#include <cstdio> //printf


using namespace Tmpl8;

namespace Engine {

Camera::Camera(int screenWidth, int screenHeight)
	: m_position{ 0 }
	, m_cameraWidth{ screenWidth }
	, m_cameraHeight{ screenHeight }
	, m_cameraBuffer{ std::make_unique<Surface>(m_cameraWidth,m_cameraHeight)}
{
}

int Camera::getWidth() const
{
	return m_cameraWidth;
}

int Camera::getHeight() const
{
	return m_cameraHeight;
}

const Tmpl8::vec2 Camera::getPosition() const
{
	return m_position;
}

Tmpl8::vec2 Camera::worldToLocal(const Tmpl8::vec2& worldSpace) const
{
	return Tmpl8::vec2(worldSpace.x - m_position.x, worldSpace.y-m_position.y);
}

Tmpl8::vec2 Camera::localToWorld(const Tmpl8::vec2& localSpace) const
{
	return Tmpl8::vec2(localSpace.x + m_position.x, localSpace.y + m_position.y);
}



void Camera::setPosition(const Tmpl8::vec2& pos)
{
	m_position = pos;
}

void Camera::Fill(Tmpl8::Pixel c)
{
	m_cameraBuffer->Clear(c);
}

void Camera::renderToSurface(Tmpl8::Surface* surface, int scale, int xOffset, int yOffset) const
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



void Camera::blitTexture(Texture* texture, int x, int y)
{
	texture->CopyToSurface(m_cameraBuffer.get(), x, y);
}


void Camera::renderTextureWorldSpace(Texture* texture, const Tmpl8::vec2& WorldSpace)
{
	float localSpaceX = WorldSpace.x - m_position.x;
	float localSpaceY = WorldSpace.y - m_position.y;

	texture->CopyToSurface(m_cameraBuffer.get(), (int)localSpaceX, (int)localSpaceY);
}

void Camera::drawLine(int x1, int y1, int x2, int y2, Tmpl8::Pixel color)
{
	m_cameraBuffer->Line((float)x1, (float)y1, (float)x2, (float)y2, color);
}

void Camera::drawLineWorldSpace(const Tmpl8::vec2& position1, const Tmpl8::vec2& position2, Tmpl8::Pixel color)
{
	vec2 local = worldToLocal(position1);
	vec2 local2 = worldToLocal(position2);

	m_cameraBuffer->Line(local.x, local.y, local2.x, local2.y, color);
}

void Camera::drawBox(int x1, int y1, int x2, int y2, Tmpl8::Pixel color)
{
	m_cameraBuffer->Box((int)x1, (int)y1, (int)x2, (int)y2, color);
}

void Camera::drawBoxWorldSpace(const Tmpl8::vec2& position1, const Tmpl8::vec2& position2, Tmpl8::Pixel color)
{
	vec2 local = worldToLocal(position1);
	vec2 local2 = worldToLocal(position2);
	m_cameraBuffer->Box((int)local.x, (int)local.y, (int)local2.x, (int)local2.y, color);
}



void Camera::drawCircle(float x, float y, float radius, Tmpl8::Pixel c, const int segments)
{
	float step = 2 * Tmpl8::PI / segments;

	for (int i = 0; i < segments; i++)
	{
		// Calculate start point of the line segment
		float x1 = radius * (float)cos(i * step) + x;
		float y1 = radius * (float)sin(i * step) + y;

		// Calculate end point of the line segment
		float x2 = radius * (float)cos((i + 1) * step) + x;
		float y2 = radius * (float)sin((i + 1) * step) + y;

		// Draw the line segment
		m_cameraBuffer->Line(x1, y1, x2, y2, c);
	}
}

void Camera::drawCircleWorldSpace(const Tmpl8::vec2& worldSpace, float radius, Tmpl8::Pixel c, const int segments)
{
	vec2 local = worldToLocal(worldSpace);
	drawCircle(local.x,local.y,radius,c,segments);
}



void Camera::renderSpriteWorldSpace(SpriteSheet* spritesheet, int x, int y, const Tmpl8::vec2& worldSpace)
{
	float localSpaceX = worldSpace.x - m_position.x;
	float localSpaceY = worldSpace.y - m_position.y;;

	spritesheet->drawSprite(m_cameraBuffer.get(), x, y, (int)localSpaceX, (int)localSpaceY);
}

void Camera::renderSpriteWorldSpace(SpriteSheet* spritesheet, int spriteIndex, const Tmpl8::vec2& worldSpace)
{
	int localSpaceX = worldSpace.x - m_position.x;
	int localSpaceY = worldSpace.y - m_position.y;;

	spritesheet->drawSprite(m_cameraBuffer.get(), spriteIndex, localSpaceX, localSpaceY);
}


};

/*
Tmpl8::vec2 Camera::worldToIsometric(const Tmpl8::vec2& worldSpace) const
{
	int halfWidth = m_tileWidth / 2;
	int quarterWidth = halfWidth / 2;

	// magic math
	float isoX = (worldSpace.x / halfWidth + worldSpace.y / quarterWidth) / 2;
	float isoY = worldSpace.y / quarterWidth - isoX;


	return Tmpl8::vec2(isoX, isoY);
}

Tmpl8::vec2 Camera::isometricToWorld(const Tmpl8::vec2& isometricSpace) const
{
	int halfWidth = m_tileWidth / 2;
	int quarterWidth = halfWidth / 2;

	float worldX = (isometricSpace.x - isometricSpace.y) * halfWidth;
	float worldY = (isometricSpace.y + isometricSpace.x) * quarterWidth;

	return Tmpl8::vec2(worldX, worldY);
}

void Camera::renderSpriteIsometric(SpriteSheet* spriteSheet, int x, int y, const Tmpl8::vec2& isoSpace)
{
	vec2 worldSpace = isometricToWorld(isoSpace);
	renderSpriteWorldSpace(spriteSheet, x, y, worldSpace);
}

void Camera::renderTextureIsometric(Texture* texture, const Tmpl8::vec2& IsoSpace)
{
	vec2 worldSpace = isometricToWorld(IsoSpace);
	renderTextureIsometric(texture, worldSpace);
}

void Camera::renderTextureIsometric(Texture* texture, float xIsoSpace, float yIsoSpace)
{
	renderTextureIsometric(texture, vec2{ xIsoSpace,yIsoSpace });
}
*/