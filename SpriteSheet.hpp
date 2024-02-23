#pragma once
#include "Texture.hpp"
#include <memory>

namespace Engine
{

	class SpriteSheet
	{
	public:
		SpriteSheet() = default;

		SpriteSheet(std::shared_ptr<Texture> fullTexture, int spriteWidth, int spriteHeight)
			: m_sheet{ fullTexture }
			, m_cellWidth{ spriteWidth }
			, m_cellHeight{ spriteHeight }
			, m_columns{ m_sheet->getWidth() / spriteWidth }
			, m_rows{ m_sheet->getHeight() / spriteHeight }
		{
		}

		SpriteSheet(std::string_view filename, int spriteWidth, int spriteHeight, Tmpl8::Pixel chromaColor)
			: m_sheet{ std::make_shared<Texture>(filename) }
			, m_cellWidth{ spriteWidth }
			, m_cellHeight{ spriteHeight }
			, m_columns{ m_sheet->getWidth() / spriteWidth }
			, m_rows{ m_sheet->getHeight() / spriteHeight }
		{
			m_sheet->setChromaKey(chromaColor);
		}


		int getSpriteWidth() const;
		int getSpriteHeight() const;
		
		int getColumns() const;
		int getRows() const; 
		

		void drawSprite(Tmpl8::Surface* surface, int x, int y, int xDst, int yDst, bool useTransparency=true) const;

		void drawSprite(Tmpl8::Surface* surface, int spriteIndex, int xDst, int yDst, bool useTransparency = true) const;

	private:
		// why shared? Because AssetManager manages assets so it should hold a weak ptr to even this texture.
		std::shared_ptr<Texture> m_sheet{ nullptr };
		const int m_cellWidth{ 0 };
		const int m_cellHeight{ 0 };

		const int m_columns{ 0 };
		const int m_rows{ 0 };

	};

};