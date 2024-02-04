#include "SoulSweep.hpp"

using namespace Tmpl8;

SoulSweep::SoulSweep(std::shared_ptr<Engine::AssetManager> assetManager, std::unique_ptr<Engine::Camera> camera)
	: m_assetManager { assetManager }
	, m_mainCamera { std::move(camera)}
	, m_mainPlayer { std::make_unique<Player>(vec2(0), 10)}
{
}
