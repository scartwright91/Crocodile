#include "LevelParser.h"

namespace Crocodile
{
	namespace s2d
	{
		LevelParser::LevelParser(std::string path, ResourceManager *resourceManager)
		{
			this->path = path;
			this->resourceManager = resourceManager;
			parseJson();
		}

		LevelParser::~LevelParser()
		{
		}

		void LevelParser::parseJson()
		{
			// read project from file
			std::ifstream f(path);
			f >> project;
		}

		glm::vec2 LevelParser::getLevelBounds(std::string level)
		{
			Json::Value levelData = project[level];
			Json::Value canvasSize = levelData["canvas_size"];
			return glm::vec2(canvasSize[0], canvasSize[1]);
		}

	}
}
