#include "SoulManager.hpp"

void SoulManager::spawnSoul(const Tmpl8::vec2& spawnPosition)
{
	m_souls.push_back(Soul(spawnPosition, m_playerPtr, m_tileMapPtr));
}


void SoulManager::updateSouls(float dt)
{
	for (std::vector<Soul>::iterator it=m_souls.begin(); it!=m_souls.end();/*no increase we do it manually, bc we can also remove souls*/)
	{
		it->actionSelection();
		it->update(dt); //updates position based on velocity and acceleration

		// checks if soul can be collected

		if (m_playerPtr->isVacuumEnabled() && m_playerPtr->canCollectSoul())
		{
			// calculate it the collect radius of the players hits the soul
			float rad = m_playerPtr->getCollectRadius() + it->getCollectRadius();
			if ((m_playerPtr->getPosition() - it->getPosition()).sqrLentgh() < rad * rad)
			{
				m_playerPtr->collectSoul();
				// now need to remove 

				it = m_souls.erase(it);
				continue; // we skip the ++it

			}
		}
		
		++it;

	}

}

void SoulManager::renderSouls(Engine::Camera& cam)
{
	

	for (auto& soul : m_souls)
	{
		soul.draw(cam, true);
	}
}
