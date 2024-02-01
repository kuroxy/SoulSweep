#include "AssetManager.hpp"
#include <cstdio>

namespace IsoEngine
{
	std::shared_ptr<Texture> AssetManager::getTexture(std::string filename)
	{
		auto texture = m_textures.find(filename);

		if (texture != m_textures.end())
		{
			std::shared_ptr<Texture> tex = texture->second.lock();
			if (tex) { // If the weak_ptr is valid
				return tex;
			}

			printf("Texture:\"%s\" was unloaded", filename.c_str());
		}

		return loadTexture(filename);
	}

	std::shared_ptr<SpriteSheet> AssetManager::getSpriteSheet(std::string filename, int spriteWidth, int spriteHeight)
	{
		auto sheet = m_spriteSheets.find(filename);

		if (sheet != m_spriteSheets.end())
		{
			std::shared_ptr<SpriteSheet> spriteSheet = sheet->second.lock();
			if (spriteSheet) { // If the weak_ptr is valid
				return spriteSheet;
			}

			printf("SpriteSheet:\"%s\" was unloaded", filename.c_str());
		}

		return loadSpriteSheet(filename, spriteWidth, spriteHeight);
	}

	std::shared_ptr<Texture> AssetManager::loadTexture(std::string filename)
	{
		std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(filename);
		m_textures[filename] = newTexture; // this is ok because it gets converted to a weak_ptr
		printf("Texture:\"%s\" is loaded", filename.c_str());
		return newTexture;
	}

	std::shared_ptr<SpriteSheet> AssetManager::loadSpriteSheet(std::string filename, int spriteWidth, int spriteHeight)
	{
		std::shared_ptr<SpriteSheet> newSpriteSheet = std::make_shared<SpriteSheet>(getTexture(filename), spriteWidth, spriteHeight);
		m_spriteSheets[filename] = newSpriteSheet; // this is ok because it gets converted to a weak_ptr
		printf("SpriteSheet:\"%s\" is loaded", filename.c_str());
		return newSpriteSheet;
	}

};


