#pragma once
#include "Texture.hpp"
#include "SpriteSheet.hpp"
#include <unordered_map>
#include <memory>
#include <string>

namespace Engine
{

	class AssetManager
	{

	public:
		AssetManager() = default;

		std::shared_ptr<Texture> getTexture(std::string filename);
		std::shared_ptr<SpriteSheet> getSpriteSheet(std::string filename, int spriteWidth, int spriteHeight, Tmpl8::Pixel chroma=0);


	private:
		std::shared_ptr<Texture> loadTexture(std::string filename);
		std::shared_ptr<SpriteSheet> loadSpriteSheet(std::string filename, int spriteWidth, int spriteHeight);

		std::unordered_map<std::string, std::weak_ptr<Texture>> m_textures;
		std::unordered_map<std::string, std::weak_ptr<SpriteSheet>> m_spriteSheets;
	};

};