#pragma once

#include <memory>

#include "Texture.hpp"


namespace Engine
{
// Forward decl
class Camera;


class SpriteSheet
{
public:
	SpriteSheet() = default;

	SpriteSheet(std::shared_ptr<Texture> fullTexture, int spriteWidth, int spriteHeight)
		: texture{ fullTexture }
		, cellWidth{ spriteWidth }
		, cellHeight{ spriteHeight }
		, columns{ texture->getWidth() / spriteWidth }
		, rows{ texture->getHeight() / spriteHeight }
	{
	}

	//creates a texture and loads it from disk,
	SpriteSheet(std::string_view filename, int spriteWidth, int spriteHeight, Tmpl8::Pixel chromaColor)
		: texture{ std::make_shared<Texture>(filename) }
		, cellWidth{ spriteWidth }
		, cellHeight{ spriteHeight }
		, columns{ texture->getWidth() / spriteWidth }
		, rows{ texture->getHeight() / spriteHeight }
	{
		texture->setChromaKey(chromaColor);
	}


	int getSpriteWidth() const { return cellWidth; }
	int getSpriteHeight() const { return cellHeight; }
		
	int getColumns() const { return columns; }
	int getRows() const { return rows; }
		
	// draws a sprite from the spriteSheet to the surface. x, y are sprite position
	void draw(Tmpl8::Surface* surface, int x, int y, int xDst, int yDst, bool forceOpaque = false, bool flip=false) const;
	// draws a sprite from the spriteSheet to the surface. Uses index instead of position
	void draw(Tmpl8::Surface* surface, int spriteIndex, int xDst, int yDst, bool forceOpaque = false , bool flip = false) const;

	// Draws them in worldspace
	void draw(Engine::Camera& camera, int x, int y, const Tmpl8::vec2& worldSpace, bool flip = false);
	void draw(Engine::Camera& camera, int spriteIndex, const Tmpl8::vec2& worldSpace, bool flip = false);


private:
	std::shared_ptr<Texture> texture{ nullptr };
	const int cellWidth{ 0 };
	const int cellHeight{ 0 };

	const int columns{ 0 };
	const int rows{ 0 };
};
};