#pragma once

#include <string>
#include "LDtkLoader/Project.hpp"

#include "Crocodile/s2d/Scene.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API LevelParser
		{
		public:
			LevelParser(
				const std::string& path,
				unsigned int tileSize,
				s2d::Scene* scene
			);

			void loadLevel(const std::string &levelName);
			glm::vec2 inline getLevelSizePixels() const { return m_levelSizePixels; };
			glm::vec2 inline getLevelSizeGrid() const { return m_levelSizeGrid; };

			void inline addCollisionTileMap(std::string layer) { m_collisionTilemapLayer = layer; };
			void inline addBatchSprite(std::string layer) { m_batchspriteLayers.push_back(layer); };
			std::vector<s2d::Object*> inline getEntities() { return m_entities; };

		private:
			
			std::string m_path;
			s2d::Scene* m_scene;
			unsigned int m_tileSize;
			glm::vec2 m_levelSizePixels = glm::vec2(0.f);
			glm::vec2 m_levelSizeGrid = glm::vec2(0);
			std::vector<std::string> m_batchspriteLayers = {};
			std::string m_collisionTilemapLayer = "";
			std::vector<s2d::Object*> m_entities = {};

		};
	}
}
