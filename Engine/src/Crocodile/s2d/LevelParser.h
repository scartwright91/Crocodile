#pragma once

#include <iostream>
#include <fstream>

#include "utils/jsoncpp/json.h"

#include "../Core.h"
#include "../ResourceManager.h"
#include "Scene.h"


namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API LevelParser
		{
		public:

			LevelParser(std::string path, ResourceManager* resourceManager);
			~LevelParser();

			Json::Value root;
			Json::Value scene_data;
			unsigned int levelSizeX;
			unsigned int levelSizeY;

			glm::vec2 inline getLevelBounds() { return glm::vec2(levelSizeX, levelSizeY); }

			const Json::Value getLayerData(std::string layer);

		private:
			std::string path;
			std::vector<s2d::Object*> objects = {};
			ResourceManager* resourceManager;

			void parseJson();

		};
	}
}

