#pragma once
#include <memory>
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "Player.hpp"

class SoulSweep
{
public:
	SoulSweep(std::shared_ptr<Engine::AssetManager> assetManager, std::unique_ptr<Engine::Camera> camera);

	void gameTick();




private:
	std::shared_ptr<Engine::AssetManager> m_assetManager;
	std::unique_ptr<Engine::Camera> m_mainCamera;
	std::unique_ptr<Player> m_mainPlayer;
};
