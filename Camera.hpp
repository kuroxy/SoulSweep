#pragma once
#include <memory>
#include "template.h"
#include "surface.h"


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


		//Scaling before translationk, scale must be positive
		void renderToSurface(Tmpl8::Surface* surface, int scale = 1, int xOffset = 0, int yOffset = 0) const; 

		// drawing textures
		void blitTexture(Texture* texture, int x, int y);

		void renderTextureWorldSpace(Texture* texture, const Tmpl8::vec2& worldSpace);

		void drawCircle(int x, int y, float radius, Tmpl8::Pixel c, const int segments = 50);
		void drawCircleWorldSpace(const Tmpl8::vec2& worldSpace, float radius, Tmpl8::Pixel c, const int segments = 50);
		
		void renderSpriteWorldSpace(SpriteSheet* spriteSheet,int x, int y, const Tmpl8::vec2& worldSpace);

		// if ever want to go back to isometric 

		//Tmpl8::vec2 worldToIsometric(const Tmpl8::vec2& worldSpace) const;
		//Tmpl8::vec2 isometricToWorld(const Tmpl8::vec2& isometricSpace) const;
		//void renderTextureIsometric(Texture* texture, float xIsoSpace, float yIsoSpace);
		//void renderTextureIsometric(Texture* texture, const Tmpl8::vec2& isoSpace);
		//void renderSpriteIsometric(SpriteSheet* spriteSheet, int x, int y, const Tmpl8::vec2& isoSpace); // for isometric stuff


	private:
		Tmpl8::vec2 m_position;
		int m_cameraWidth;
		int m_cameraHeight;
		std::unique_ptr<Tmpl8::Surface> m_cameraBuffer;

	};

};