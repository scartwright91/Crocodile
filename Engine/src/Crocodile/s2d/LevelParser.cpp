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
				std::vector<glm::vec2> path = {};
				for (int j = 0; j < entData["movement_path"].size(); j++)
				{
					Json::Value p = entData["movement_path"][j];
					path.push_back(glm::vec2(p[0].asFloat(), p[1].asFloat()));
				}
				sceneEnt.path = path;
				sceneEntitiesData.push_back(sceneEnt);
			}

			// placed text entities data
			const Json::Value placedTextEntitiesData = levelData["placed_text_entities"];
			std::vector<s2d::SceneTextEntityData *> sceneTextEntitiesData = {};
			for (int i = 0; i < placedTextEntitiesData.size(); i++)
			{
				Json::Value entData = placedTextEntitiesData[i];
				s2d::SceneTextEntityData *sceneTextEnt = new s2d::SceneTextEntityData();
				sceneTextEnt->text = entData["text"].asString();
				sceneTextEnt->layer = entData["layer"].asString();
				sceneTextEnt->alpha = entData["alpha"].asFloat();
				sceneTextEnt->pos = glm::vec2(entData["pos"][0].asFloat(), entData["pos"][1].asFloat());
				sceneTextEnt->color = glm::vec3(
					entData["color"][0].asFloat(),
					entData["color"][1].asFloat(),
					entData["color"][2].asFloat());
				sceneTextEnt->textScale = glm::vec2(entData["text_scale"][0].asFloat(), entData["text_scale"][1].asFloat());
				sceneTextEntitiesData.push_back(sceneTextEnt);
			}

			// placed particle entities data
			const Json::Value placedParticleEntitiesData = levelData["placed_particle_entities"];
			std::vector<s2d::SceneParticleEntityData *> sceneParticleEntitiesData = {};
			for (int i = 0; i < placedParticleEntitiesData.size(); i++)
			{
				Json::Value entData = placedParticleEntitiesData[i];
				s2d::SceneParticleEntityData *sceneParticleEnt = new s2d::SceneParticleEntityData();
				sceneParticleEnt->layer = entData["layer"].asString();
				sceneParticleEnt->amount = entData["amount"].asInt();
				sceneParticleEnt->alpha = entData["alpha"].asFloat();
				sceneParticleEnt->direction = entData["direction"].asFloat();
				sceneParticleEnt->dispersion = entData["dispersion"].asFloat();
				sceneParticleEnt->scale = entData["scale"].asFloat();
				sceneParticleEnt->velocity = entData["velocity"].asFloat();
				sceneParticleEnt->pos = glm::vec2(entData["pos"][0].asFloat(), entData["pos"][1].asFloat());
				sceneParticleEnt->color = glm::vec3(
					entData["color"][0].asFloat(),
					entData["color"][1].asFloat(),
					entData["color"][2].asFloat());
				sceneParticleEntitiesData.push_back(sceneParticleEnt);
			}

			ld.name = level;
			ld.size = getCanvasBounds(level);
			ld.color = getCanvasColor(level);
			ld.layerNames = layerNames;
			ld.sceneEntityData = sceneEntitiesData;
			ld.sceneTextEntityData = sceneTextEntitiesData;
			ld.SceneParticleEntityData = sceneParticleEntitiesData;

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
