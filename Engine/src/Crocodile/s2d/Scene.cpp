#include "Scene.h"

namespace Crocodile
{
	namespace s2d
	{

		Scene::Scene(graphics::Window *window, ResourceManager *resourceManager)
		{
			m_window = window;
			m_windowWidth = window->getWidth();
			m_windowHeight = window->getHeight();
			m_resourceManager = resourceManager;
			init();
#ifdef CROCODILE_EMSCRIPTEN
			enablePostprocessing = false;
#endif
		}

		Scene::~Scene()
		{
			delete m_spriteRenderer;
			delete m_particleRenderer;
			delete m_grid;
			delete m_lightSystem;
			delete m_camera;
			delete m_layerStack;
		}

		void Scene::addObject(s2d::Object *object, std::string layerName)
		{
			Layer *layer = m_layerStack->getLayer(layerName);
			if (layer != NULL)
			{
				layer->addObject(object);
				object->m_layer = layerName;
			}
			else
				LOG(ERROR, "Layer " + layerName + " does not exist in layerstack");
		}

		void Scene::removeObject(s2d::Object *object, std::string layerName)
		{
			Layer *layer = m_layerStack->getLayer(layerName);
			if (layer != NULL)
			{
				if (object != NULL)
				{
					layer->removeObject(object);
					object->m_layer = "";
				}
			}
			else
				LOG(ERROR, "Layer " + layerName + " does not exist in layerstack");
		}

		void Scene::update(float dt)
		{
			// update timer (used for shaders)
			time += 0.05f * dt;
			if (time >= 1.0)
				time = 0.0;
			if (m_enableScaling)
				m_scale = m_window->getScale();
			m_camera->update(m_scale);
			// objects
			updateObjects(dt);
			// particles
			updateParticles(dt);
		}

		void Scene::updateObjects(float dt)
		{
			for (Layer *layer : m_layerStack->getLayers())
			{
				if (layer->m_ySort)
					layer->sort();
				for (Object *obj : layer->getObjects())
				{
					if (obj == NULL)
						removeObject(obj, layer->m_name);
					else
					{
						obj->updateAnimation(dt);
						obj->updateSqueezeEffect(dt);
						obj->m_modelScale = glm::vec3(m_scale, 1.f);
						glm::vec2 velocity = obj->m_velocity * glm::vec2(dt);
						if (obj->m_collisionLayers.size() > 0)
						{
							// static tile-based collisions
							velocity = resolveTilemapCollisions(obj, velocity);
							// dynamic entitiy collisions
							for (unsigned int collisionLayer = 1; collisionLayer <= 3; collisionLayer++)
								velocity = resolveCollisions(obj, collisionLayer, velocity);
						}
						obj->move(velocity.x, velocity.y);
					}
				}
			}
		}

		void Scene::render()
		{
			// render grid
			if (m_grid->active)
				m_grid->render(
					(float)m_windowWidth,
					(float)m_windowHeight,
					m_camera->m_cameraScaledPosition.x,
					m_camera->m_cameraScaledPosition.y,
					m_camera->m_zoom);
			// render scene objects
			for (Layer *layer : m_layerStack->getLayers())
				if (!layer->m_hide)
					for (Object *obj : layer->getObjects())
					{
						renderObject(obj, layer);
						for (Object* childObject : obj->m_children)
							renderObject(childObject, layer);
					}
		}

		void Scene::renderObject(Object *obj, Layer *layer)
		{
			// calculating view matrix
			glm::mat4 view;
			if (!layer->m_applyCamera)
				view = glm::mat4(1.0f);
			else
				view = m_camera->getViewMatrix(layer->m_depth);

			// calculating projection matrix
			glm::mat4 projection = m_camera->getProjectionMatrix(layer->m_applyCamera);
			// get object position
			glm::vec2 pos = obj->getScaledPosition();

			std::vector<Light *> lights;
			if (m_enableLighting)
				lights = m_lightSystem->getScaledLights(obj->m_modelScale);
			else
				lights = {};
				
			if (obj->m_renderMethod == "sprite")
			{
				m_spriteRenderer->render(
					time,
					obj->calculateModelMatrix(pos, 1.f),
					view,
					projection,
					obj->m_texture,
					obj->m_useTexture,
					obj->m_useColorTexture,
					obj->m_numberOfRows,
					obj->m_numberOfCols,
					obj->m_textureOffset,
					obj->m_color,
					obj->m_alpha,
					m_ambientLighting,
					lights,
					obj->m_outline,
					obj->m_size.x / obj->m_size.y,
					m_distortionTexture,
					obj->m_useDistortion,
					obj->m_scrollDistortionX,
					obj->m_scrollDistortionY,
					obj->m_distortionSpeed,
					obj->m_flipX,
					obj->m_flipY,
					layer->m_alpha,
					obj->m_useSqueeze,
					obj->m_deformationMagnitude
					);
			}
			else if (obj->m_renderMethod == "particles")
			{
				ParticleGenerator *pg = (ParticleGenerator *)obj;
				m_particleRenderer->render(
					pg->particles,
					pg->calculateModelMatrix(pos, 1.f),
					view,
					projection,
					pg->m_texture.textureID,
					pg->m_useTexture,
					pg->m_color,
					pg->m_alpha,
					layer->m_alpha,
					m_ambientLighting,
					lights
					);
			}
			else if (obj->m_renderMethod == "text")
			{
				Text *text = (Text *)obj;
				m_textRenderers[text->fontName]->render(
					text->text,
					text->calculateModelMatrix(pos, 1.f),
					view,
					projection,
					text->m_color,
					text->m_alpha,
					text->getScale() * glm::vec2(text->m_modelScale.x, text->m_modelScale.y),
					layer->m_alpha);
			}
			else if (obj->m_renderMethod == "line")
			{
				shapes::Line *line = (shapes::Line *)obj;
				m_lineRenderer->render(
					line->p1,
					line->p2,
					view,
					projection,
					line->m_color,
					line->m_alpha,
					layer->m_alpha);
			}
			else if (obj->m_renderMethod == "circle")
			{
				shapes::Circle *circle = (shapes::Circle *)obj;
				m_circleRenderer->render(
					circle->thickness,
					circle->fade,
					obj->calculateModelMatrix(pos, 1.f),
					view,
					projection,
					circle->m_color,
					circle->m_alpha,
					layer->m_alpha);
			}
			else if (obj->m_renderMethod == "batch_sprite")
			{
				BatchSprite *batchSprite = (BatchSprite *)obj;
				batchSprite->render(
					m_scale,
					view,
					projection,
					batchSprite->m_texture,
					batchSprite->m_alpha,
					m_ambientLighting,
					lights
					);
			}
			else
			{
				LOG(WARNING, "object has undefined render type");
			}

			col::BoundingBox bbox = obj->getBoundingBox();
			// render axis lines
			if (obj->m_showAxes)
			{
				// axes lines
				for (glm::vec2 v : bbox.getDebugAxes())
				{
					m_lineRenderer->render(
						bbox.center * m_scale,
						v * m_scale,
						view,
						projection,
						glm::vec3(0.f, 0.f, 1.f),
						0.5f,
						layer->m_alpha);
				}
			}
			// render lines to display object's bounding box
			if (obj->m_showBoundingBox)
			{
				// connect bbox vertices
				std::vector<glm::vec2> vertices = bbox.vertices;
				for (unsigned int i = 1; i < 4; i++)
				{
					m_lineRenderer->render(
						vertices[i - 1] * m_scale,
						vertices[i] * m_scale,
						view,
						projection,
						obj->m_color,
						obj->m_alpha,
						layer->m_alpha);
				}
				m_lineRenderer->render(
					vertices[3] * m_scale,
					vertices[0] * m_scale,
					view,
					projection,
					obj->m_color,
					obj->m_alpha,
					layer->m_alpha);
			}
		}

		void Scene::clear()
		{
			m_lightSystem->clear();
			for (Layer *layer : m_layerStack->getLayers())
				layer->clear();
			m_collisionLayers.clear();
		}

		void Scene::scaleScene()
		{
			m_windowWidth = m_window->getWidth();
			m_windowHeight = m_window->getHeight();
		}

		bool Scene::isTileCollideable(int x, int y)
		{
			if ((x >= 0) && (x < m_tilemapSize.x) && (y >= 0) && (y < m_tilemapSize.y))
			{
				return m_collisionTilemap[y][x].collideable;
			}
			return 0;
		}

		col::BoundingBox Scene::getTileBoundingBox(int x, int y)
		{
			return col::BoundingBox(x * m_tileSize, y * m_tileSize, m_tileSize, m_tileSize, 0.0f);
		}

		void Scene::addObjectToCollisionLayer(Object* obj, unsigned int collisionLayer)
		{
			if (collisionLayer > 2)
			{
				LOG(WARNING, "Collision layer must be 0, 1, 2");
				return;
			}
			m_collisionLayers[collisionLayer].push_back(obj);
		}

		void Scene::removeObjectFromCollisionLayer(Object* obj, unsigned int collisionLayer)
		{
			if (collisionLayer > 2)
			{
				LOG(WARNING, "Collision layer must be 0, 1, 2");
				return;
			}
			m_collisionLayers[collisionLayer].erase(
				std::remove(
					m_collisionLayers[collisionLayer].begin(), 
					m_collisionLayers[collisionLayer].end(),
					obj
				),
				m_collisionLayers[collisionLayer].end()
			);
		}

		glm::vec2 Scene::resolveTilemapCollisions(Object* obj, glm::vec2 velocity)
		{
			obj->resetCollisionData(m_tilemapLayer);

			// get sprite's current position and tile
			glm::vec2 pos = obj->getPosition();
			int tileX = (int)(pos.x / m_tileSize);
			int tileY = (int)(pos.y / m_tileSize);
			float d = 0.f;

			for (int x = std::max(tileX - 3, 0); x < std::min(tileX + 3, (int)m_tilemapSize.x); x++)
			{
				for (int y = std::max(tileY - 3, 0); y < std::min(tileY + 3, (int)m_tilemapSize.y); y++)
				{
					s2d::Tile tile = m_collisionTilemap[y][x];
					if (tile.collideable)
					{
						col::BoundingBox tb = tile.getBoundingBox();
						// y-axis collision
						bool yCollision = obj->getShiftedBoundingBox(0.0f, velocity.y).intersectsBounds(tb);
						if (yCollision)
						{
							d = obj->getBoundingBox().getMinDistanceFromBounds(tb, "y");
							if (velocity.y >= 0)
							{
								velocity.y = d;
								obj->m_velocity.y = 0.f;
								obj->m_collisionData[m_tilemapLayer].on_floor = true;
							}
							else
							{
								velocity.y = -d;
								obj->m_collisionData[m_tilemapLayer].on_ceiling = true;
							}
						}
						// x-axis collision
						bool xCollision = obj->getShiftedBoundingBox(velocity.x, 0.0f).intersectsBounds(tb);
						if (xCollision)
						{
							d = obj->getBoundingBox().getMinDistanceFromBounds(tb, "x");
							if (velocity.x >= 0)
							{
								velocity.x = d;
								obj->m_collisionData[m_tilemapLayer].on_wall_right = true;
							}
							else
							{
								velocity.x = -d;
								obj->m_collisionData[m_tilemapLayer].on_wall_left = true;
							}
						}
					}
				}
			}

			return velocity;
		}

		glm::vec2 Scene::resolveCollisions(Object* obj, unsigned int collisionLayer, glm::vec2 velocity)
		{
			obj->resetCollisionData(collisionLayer);
			float d = 0.f;
			for (s2d::Object *e : m_collisionLayers[collisionLayer])
			{
				// y-axis collision
				bool yCollision = obj->getShiftedBoundingBox(0.0f, velocity.y).intersectsBounds(e->getBoundingBox());
				if (yCollision)
				{
					d = obj->getBoundingBox().getMinDistanceFromBounds(e->getBoundingBox(), "y");
					if (velocity.y >= 0)
					{
						velocity.y = d;
						obj->m_velocity.y = 0.f;
						obj->m_collisionData[collisionLayer].on_floor = true;
					}
					else
					{
						velocity.y = -d;
						obj->m_collisionData[collisionLayer].on_ceiling = true;
					}
				}
				// x-axis collision
				bool xCollision = obj->getShiftedBoundingBox(velocity.x, 0.0f).intersectsBounds(e->getBoundingBox());
				if (xCollision)
				{
					d = obj->getBoundingBox().getMinDistanceFromBounds(e->getBoundingBox(), "x");
					if (velocity.x >= 0)
					{
						velocity.x = d;
						obj->m_collisionData[collisionLayer].on_wall_right = true;
					}
					else
					{
						velocity.x = -d;
						obj->m_collisionData[collisionLayer].on_wall_left = true;
					}
				}
			}
			return velocity;
		}

		std::vector<std::string> Scene::getEntityGroupNames()
		{
			std::vector<std::string> keys;
			for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); it++) {
				keys.push_back(it->first);
			}
			return keys;
		}

		void Scene::addEntityToGroup(Object* obj, std::string group)
		{
			if (m_entityGroups.find(group) == m_entityGroups.end())
				m_entityGroups[group] = {obj};
			else
				m_entityGroups[group].push_back(obj);
		}

		void Scene::removeEntityFromGroup(Object* obj, std::string group)
		{
			m_entityGroups[group].erase(std::remove(m_entityGroups[group].begin(), m_entityGroups[group].end(), obj), m_entityGroups[group].end());
		}

		std::vector<Object*> Scene::getEntityGroup(std::string group)
		{
			return m_entityGroups[group];
		}

		void Scene::addTextRenderer(const std::string name, const std::string fontPath, unsigned int fontSize)
		{
			m_textRenderers[name] = new s2d::TextRenderer(fontPath, fontSize, m_resourceManager->m_shaderManager.getShader("text")); 
		}

		void Scene::addParticleEffect(glm::vec2 position, ParticleSettings settings, std::string layer)
		{
			ParticleGenerator* pg = new ParticleGenerator(settings);
			pg->setPosition(position);
			addObject(pg, layer);
			m_particles.push_back(pg);
		}

		void Scene::updateParticles(float dt)
		{
			for (ParticleGenerator* pg : m_particles)
			{
				pg->update(dt);
				if (pg->finished)
				{
					removeObject(pg, pg->m_layer);
					m_particles.erase(std::remove(m_particles.begin(), m_particles.end(), pg), m_particles.end());
					delete pg;
				}
			}
		}

		void Scene::init()
		{
			m_camera = new s2d::Camera(m_window);
			m_lightSystem = new s2d::LightSystem();
			m_distortionTexture = m_resourceManager->getTexture("distortion_texture").textureID;
			m_layerStack = new LayerStack();
			// renderers
			m_spriteRenderer = new s2d::SpriteRenderer(m_resourceManager->m_shaderManager.getShader("sprite"));
			m_particleRenderer = new s2d::ParticleRenderer(m_resourceManager->m_shaderManager.getShader("particle"));
			m_lineRenderer = new s2d::LineRenderer(m_resourceManager->m_shaderManager.getShader("line"));
			m_circleRenderer = new s2d::CircleRenderer(m_resourceManager->m_shaderManager.getShader("circle"));
			m_grid = new s2d::BackgroundGrid(m_resourceManager->m_shaderManager.getShader("grid"));
			m_textRenderers["default"] = new s2d::TextRenderer("assets/fonts/OpenSans-Regular.ttf", 48, m_resourceManager->m_shaderManager.getShader("text"));
		}

	}
}
