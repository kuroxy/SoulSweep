#pragma once
#include "Texture.hpp"
#include <memory>

namespace Engine
{

	class SpriteSheet
	{
	public:
		SpriteSheet(std::shared_ptr<Texture> fullTexture, int spriteWidth, int spriteHeight);

		int getSpriteWidth() const;
		int getSpriteHeight() const;
		
		int getColumns() const;
		int getRows() const; 
		

		void drawSprite(Tmpl8::Surface* surface, int x, int y, int xDst, int yDst, bool useTransparency=true) const;

		void drawSprite(Tmpl8::Surface* surface, int spriteIndex, int xDst, int yDst, bool useTransparency = true) const;

	private:
		// why shared? Because AssetManager manages assets so it should hold a weak ptr to even this texture.
		std::shared_ptr<Texture> m_sheet; 
		const int m_cellWidth{ 0 };
		const int m_cellHeight{ 0 };

		const int m_columns{ 0 };
		const int m_rows{ 0 };

	};

};