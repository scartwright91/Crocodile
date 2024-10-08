#include "LevelParser.h"

namespace Crocodile
{
	namespace s2d
	{
		LevelParser::LevelParser(
			const std::string& path,
			unsigned int tileSize,
			s2d::Scene* scene
		) : m_path(path), m_tileSize(tileSize), m_scene(scene)
		{
		}

		void LevelParser::loadLevel(const std::string &levelName)
		{
			ldtk::Project project;
			project.loadFromFile(m_path);

			// get the world
			const auto& world = project.getWorld();
			
			const auto& level = world.getLevel(levelName);

			m_levelSizePixels = { level.size.x, level.size.y };
			m_levelSizeGrid = {
				(int)m_levelSizePixels.x / m_tileSize,
				(int)m_levelSizePixels.y / m_tileSize
			};

			m_scene->m_tileSize = (float)m_tileSize;
			m_scene->m_tilemapSize = m_levelSizeGrid;

			// collisions
			if (m_collisionTilemapLayer != "")
			{
				std::vector<std::vector<s2d::Tile>> collisionTileMap(
					(int)m_levelSizeGrid.y,
					std::vector<s2d::Tile>((int)m_levelSizeGrid.x, s2d::Tile()));

				for (int y = 0; y < m_levelSizeGrid.y; y++)
					for (int x = 0; x < m_levelSizeGrid.x; x++)
					{
						// Rectangle static collisions
						auto c = level.getLayer("Collisions").getIntGridVal(x, y);
						s2d::Tile t{(float)x, (float)y, m_scene->m_tileSize, c.value == 1};
						collisionTileMap[y][x] = t;
					}
				m_scene->m_collisionTilemap = collisionTileMap;
			}

			// batch sprites

		}

	}
}