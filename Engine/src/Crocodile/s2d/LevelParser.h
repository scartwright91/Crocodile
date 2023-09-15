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
			// level name
			std::string name;
			// world size of canvas
			glm::vec2 size;
			// background colour of canvas
			glm::vec3 color;
			// layer information
			std::vector<std::string> layerNames;
			// Unique entity types to place in the scene
			std::vector<s2d::EntityData *> entitiesData;
			// Entities that have been placed in the scene (containing scene-specific info such as position etc)
			std::vector<s2d::SceneEntityData> sceneEntityData;
			// text entities placed in the scene
			std::vector<s2d::SceneTextEntityData *> sceneTextEntityData;
			// particle Entities placed in the scene
			std::vector<s2d::SceneParticleEntityData *> SceneParticleEntityData;
			// Textures that have been used by entities
			std::vector<ResourceManager::TextureData> textures;
		};

		struct ConnectionData
		{
			std::string currentLevel;
			std::string destinationLevel;
			s2d::Object *collision;
			glm::vec2 collisionPos;
			glm::vec2 collisionSize;
			glm::vec2 spawn;
		};

		class CROCODILE_API LevelParser
		{
		public:
			LevelParser(std::string path, ResourceManager *resourceManager);
			~LevelParser();

			Json::Value project;
			std::vector<std::string> levelNames = {};

			void parseLevel(std::string level);
			void parseConnections(std::string level);

			LevelData ld;

			std::vector<std::string> connectionKeys = {};
			std::map<std::string, ConnectionData> connectionsDataMap = {};

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
