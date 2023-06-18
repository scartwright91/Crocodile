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

		LevelData LevelParser::parseLevel(std::string level)
		{

			LevelData ld;

			Json::Value levelData = project["levels"][level];
			// parse texture data
			const Json::Value levelTexturesData = levelData["texture_keys"];
			for (int i = 0; i < levelTexturesData.size(); i++)
			{
				std::string textureName = levelTexturesData[i].asString();
				std::string texturePath = levelData["textures"][textureName].asString();
				resourceManager->loadTexture(texturePath.c_str(), textureName, false);
			}

			// get level layers
			const Json::Value levelLayersData = levelData["layers"];
			std::vector<std::string> layerNames = {};
			for (int i = 0; i < levelLayersData.size(); i++)
				layerNames.push_back(std::string(levelLayersData[i].asString()));

			// get scene entities
			const Json::Value placedEntitiesData = levelData["placed_entities"];
			std::vector<s2d::SceneEntityData> sceneEntitiesData = {};
			for (int i = 0; i < placedEntitiesData.size(); i++)
			{
				Json::Value entData = placedEntitiesData[i];
				SceneEntityData sceneEnt;
				sceneEnt.label = entData["label"].asString();
				sceneEnt.layer = entData["layer"].asString();
				sceneEnt.rotation = entData["rotation"].asFloat();
				sceneEnt.alpha = entData["alpha"].asFloat();
				sceneEnt.pos = glm::vec2(entData["pos"][0].asFloat(), entData["pos"][1].asFloat());
				sceneEnt.size = glm::vec2(entData["size"][0].asFloat(), entData["size"][1].asFloat());
				sceneEnt.texture = entData["texture"].asString();
				sceneEntitiesData.push_back(sceneEnt);
			}

			ld.name = level;
			ld.size = getCanvasBounds(level);
			ld.color = getCanvasColor(level);
			ld.layerNames = layerNames;
			ld.sceneEntityData = sceneEntitiesData;

			return ld;
		}

		glm::vec2 LevelParser::getCanvasBounds(std::string level)
		{
			Json::Value levelData = project["levels"][level];
			Json::Value canvasSize = levelData["canvas_size"];
			return glm::vec2(canvasSize[0].asFloat(), canvasSize[1].asFloat());
		}

		glm::vec3 LevelParser::getCanvasColor(std::string level)
		{
			Json::Value levelData = project["levels"][level];
			Json::Value canvasColor = levelData["canvas_color"];
			return glm::vec3(canvasColor[0].asFloat(), canvasColor[1].asFloat(), canvasColor[2].asFloat());
		}

	}
}
