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

		struct LevelData
		{
			std::string name;
			glm::vec2 size;
			glm::vec3 color;
			std::vector<std::string> layers;
			std::vector<s2d::SceneEntityData> sceneEntityData;
		};

		class CROCODILE_API LevelParser
		{
		public:
			LevelParser(std::string path, ResourceManager *resourceManager);
			~LevelParser();

			Json::Value project;
			std::vector<std::string> levelNames = {};

			LevelData parseLevel(std::string level);

		private:
			std::string path;
			std::vector<s2d::Object *> objects = {};
			ResourceManager *resourceManager;

			void parseJson();
			glm::vec2 getCanvasBounds(std::string level);
			glm::vec3 getCanvasColor(std::string level);
		};
	}
}
