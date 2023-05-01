#include "LevelParser.h"


namespace Crocodile
{
	namespace s2d
	{
		LevelParser::LevelParser(std::string path, ResourceManager* resourceManager)
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
			Json::Reader reader;
			std::ifstream file(this->path);
			bool parsingSuccessful = reader.parse(file, root);
			if (!parsingSuccessful)
			{
				// report to the user the failure and their locations in the document.
				std::cout << path << ": failed to parse configuration\n"
					<< reader.getFormattedErrorMessages() << std::endl;
				return;
			}

			const Json::Value levelSize = root["level_size"];
			levelSizeX = levelSize["size_x"].asInt();
			levelSizeY = levelSize["size_y"].asInt();
			scene_data = root["scene"];

		}

		const Json::Value LevelParser::getLayerData(std::string layer)
		{
			return scene_data[layer];
		}

	}
}
