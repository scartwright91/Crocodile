#pragma once

#include <vector>
#include <algorithm>
#include <sol/sol.hpp>

#include "../Core.h"
#include "../ResourceManager.h"
#include "Camera.h"
#include "Object.h"
#include "Particles.h"
#include "Text.h"
#include "BatchSprite.h"
#include "shapes/Line.h"
#include "shapes/Circle.h"
#include "LightSystem.h"
#include "LayerStack.h"
#include "Tile.h"

#include "rendering/SpriteRenderer.h"
#include "rendering/ParticleRenderer.h"
#include "rendering/TextRenderer.h"
#include "rendering/BackgroundGrid.h"
#include "rendering/LineRenderer.h"
#include "rendering/CircleRenderer.h"

#include "../graphics/Window.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API Scene
		{

		public:
			Scene(graphics::Window *window, ResourceManager *resourceManager);
			~Scene();

			void update(float dt);
			void updateObjects(float dt);
			void render();
			void clear();
			void scaleScene();

			Object* addSprite(std::string layerName);
			// Text* addText(std::string layerName);
			// ParticleGenerator* addParticles(ParticleSettings settings, std::string layerName);
			// BatchSprite* addBatchSprite(std::string )

			void addObject(Object *object, std::string layerName);
			void removeObject(Object *object, std::string layerName);
			void addLayer(const std::string &layerName, bool applyCamera);

			unsigned int inline numberOfObjectsInScene() { return m_layerStack->getNumberOfObjects(); };

			graphics::Window* m_window;
			Camera* m_camera;

			int m_windowWidth;
			int m_windowHeight;

			// tilemap (for grid based games)
			unsigned int m_tilemapLayer = 0;
			float m_tileSize = 0;
			glm::vec2 m_tilemapSize = {0, 0};

			// options
			bool m_enableScaling = true;
			bool m_enableLighting = true;

			// lights
			float m_ambientLighting = 1.0f;
			LightSystem* m_lightSystem;

			// distortion
			float time = 0.f;
			unsigned int m_distortionTexture;

			// background grid
			BackgroundGrid* m_grid;

			// render layers
			LayerStack *m_layerStack = nullptr;

			// rendering
			ResourceManager *m_resourceManager;
			virtual void renderObject(Object *obj, Layer *layer);

			// text
			void addTextRenderer(const std::string name, const std::string fontPath, unsigned int fontSize);

			// render pipelines
			SpriteRenderer *m_spriteRenderer;
			ParticleRenderer *m_particleRenderer;
			LineRenderer *m_lineRenderer;
			CircleRenderer *m_circleRenderer;
			std::map<std::string, TextRenderer*> m_textRenderers = {};

			// collisions
			bool isTileCollideable(int x, int y);
			col::BoundingBox getTileBoundingBox(int x, int y);
			std::vector<std::vector<Tile>> m_collisionTilemap = {};
			void addObjectToCollisionLayer(Object* obj, unsigned int collisionLayer);
			void removeObjectFromCollisionLayer(Object* obj, unsigned int collisionLayer);

			// entity groups
			std::vector<std::string> getEntityGroupNames();
			void addEntityToGroup(Object* obj, std::string group);
			void removeEntityFromGroup(Object* obj, std::string group);
			std::vector<Object*> getEntityGroup(std::string group);

			// particles
			std::vector<ParticleGenerator*> m_particles = {};
			void addParticleEffect(glm::vec2 position, ParticleSettings settings, std::string layer);
			void updateParticles(float dt);

			// lua
			void addLuaBindings(sol::state &lua);

		private:

			// scaling
			glm::vec2 m_scale = glm::vec2(1.f);

			// collisions
			std::map<unsigned int, std::vector<Object*>> m_collisionLayers = {
				{0, {}},
				{1, {}},
				{2, {}}
			};
			glm::vec2 resolveTilemapCollisions(Object* obj, glm::vec2 velocity);
			glm::vec2 resolveCollisions(Object* obj, unsigned int collisionLayer, glm::vec2 velocity);

			// entity groups
			std::map<std::string, std::vector<Object*>> m_entityGroups = {};

			void init();
		};

	}
}
