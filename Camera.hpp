#pragma once
#include <memory>
#include "template.h"
#include "surface.h"
#include <string>

namespace Engine {

	//forward declaration
	class Texture;
	class SpriteSheet;

	class Camera
	{
	public:
		Camera(int screenWidth, int screenHeight);

		int getWidth() const;
		int getHeight() const;
		const Tmpl8::vec2 getPosition() const;

		Tmpl8::vec2 worldToLocal(const Tmpl8::vec2& worldSpace) const;
		Tmpl8::vec2 localToWorld(const Tmpl8::vec2& localSpace) const;

		
		void setPosition(const Tmpl8::vec2& pos);

		void Fill(Tmpl8::Pixel c);

		void drawText(const std::string str, int x, int y, Tmpl8::Pixel color);

		void darkenPixel(int x, int y, int amount);


		//Scaling before translationk, scale must be positive
		void renderToSurface(Tmpl8::Surface* surface, int scale = 1, int xOffset = 0, int yOffset = 0) const; 

		// drawing textures
		void blitTexture(Texture* texture, int x, int y);

		void renderTextureWorldSpace(const Texture& texture, const Tmpl8::vec2& WorldSpace);

		void drawLine(int x1, int y1, int x2, int y2, Tmpl8::Pixel color);
		void drawLineWorldSpace(const Tmpl8::vec2& position1, const Tmpl8::vec2& position2, Tmpl8::Pixel color);

		void drawBox(int x1, int y1, int x2, int y2, Tmpl8::Pixel color);
		void drawBoxWorldSpace(const Tmpl8::vec2& position1, const Tmpl8::vec2& position2, Tmpl8::Pixel color);

		void drawBar(int x1, int y1, int x2, int y2, Tmpl8::Pixel color);
		void drawBarWorldSpace(const Tmpl8::vec2& position1, const Tmpl8::vec2& position2, Tmpl8::Pixel color);

		void drawBarDarken(int x1, int y1, int x2, int y2, int amount);
		void drawBarDarkenWorldSpace(const Tmpl8::vec2& position1, const Tmpl8::vec2& position2, int amount);

		void drawCircle(float x, float y, float radius, Tmpl8::Pixel c, const int segments = 50);
		void drawCircleWorldSpace(const Tmpl8::vec2& worldSpace, float radius, Tmpl8::Pixel c, const int segments = 50);

		void drawFillCircle(float x, float y, float radius, Tmpl8::Pixel color, bool alpha=false);
		void drawFillCircleWorldSpace(const Tmpl8::vec2& position, float radius, Tmpl8::Pixel color, bool alpha=false);

		void renderSpriteWorldSpace(SpriteSheet* spriteSheet,int x, int y, const Tmpl8::vec2& worldSpace);

		void renderSpriteWorldSpace(SpriteSheet* spritesheet, int spriteIndex, const Tmpl8::vec2& worldSpace);



	private:
		Tmpl8::vec2 m_position;
		int m_cameraWidth;
		int m_cameraHeight;
		std::unique_ptr<Tmpl8::Surface> m_cameraBuffer;

	};

};