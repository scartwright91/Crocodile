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
			LevelParser(std::string path, ResourceManager *resourceManager);
			~LevelParser();

			Json::Value project;
			std::vector<std::string> levelNames = {};

			const Json::Value inline getLevelData(std::string level) { return project[level]; }
			glm::vec2 getLevelBounds(std::string level);

		private:
			std::string path;
			std::vector<s2d::Object *> objects = {};
			ResourceManager *resourceManager;

			void parseJson();
		};
	}
}
