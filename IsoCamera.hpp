#pragma once
#include <memory>
#include "template.h"
#include "surface.h"

//forward declaration
class Texture;


class IsoCamera
{
public:
	IsoCamera(int screenWidth, int screenHeight, int tileWidth);

	int getWidth() const;
	int getHeight() const;
	const Tmpl8::vec2 getPosition() const;

	Tmpl8::vec2 worldToIsometric(const Tmpl8::vec2& worldSpace) const;
	Tmpl8::vec2 isometricToWorld(const Tmpl8::vec2& isometricSpace) const;

	void setPosition(const Tmpl8::vec2& pos);

	void renderToSurface(Tmpl8::Surface* surface, int scale=1, int xOffset=0, int yOffset=0) const; //Scaling before translation scale must be postive
	
	// drawing textures
	void blitTexture(Texture* texture, int x, int y);

	void renderTextureWorldSpace(Texture* texture, float xWorldSpace, float yWorldSpace);
	void renderTextureWorldSpace(Texture* texture, const Tmpl8::vec2& WorldSpace);

	void renderTextureIsometric(Texture* texture, float xIsoSpace, float yIsoSpace);
	void renderTextureIsometric(Texture* texture, const Tmpl8::vec2& IsoSpace);


private:
	Tmpl8::vec2 m_position;
	int m_cameraWidth;
	int m_cameraHeight;
	std::unique_ptr<Tmpl8::Surface> m_cameraBuffer;

	int m_tileWidth;
};

