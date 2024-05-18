#include "Scene.h"

namespace Crocodile
{
	namespace s2d
	{

		Scene::Scene(graphics::Window *window, ResourceManager *resourceManager)
		{
			this->window = window;
			windowWidth = window->getWidth();
			windowHeight = window->getHeight();
			this->resourceManager = resourceManager;
			init();
#ifdef CROCODILE_EMSCRIPTEN
			enablePostprocessing = false;
#endif
		}

		Scene::~Scene()
		{
			delete spriteRenderer;
			delete particleRenderer;
			// delete textRenderer;
			delete postProcessing;
			delete grid;
			delete lightSystem;
			delete camera;
			delete layerStack;
		}

		void Scene::addObject(s2d::Object *object, std::string layerName)
		{
			Layer *layer = layerStack->getLayer(layerName);
			if (layer != NULL)
			{
				layer->addObject(object);
				object->layer = layerName;
			}
			else
				std::cout << "Layer " << layerName << " does not exist in layerstack" << std::endl;
		}

		void Scene::removeObject(s2d::Object *object, std::string layerName)
		{
			Layer *layer = layerStack->getLayer(layerName);
			if (layer != NULL)
			{
				if (object != NULL)
				{
					layer->removeObject(object);
					object->layer = "";
				}
			}
			else
				std::cout << "Layer " << layerName << " does not exist in layerstack" << std::endl;
		}

		void Scene::update(float dt)
		{
			// update timer (used for shaders)
			time += 0.05f * dt;
			if (time >= 1.0)
				time = 0.0;
			if (enableScaling)
				viewportScale = window->getViewportScale();
			else
				viewportScale = glm::vec2(1.f);
			camera->update(viewportScale);
			// screen resizing
			if ((window->getViewportWidth() != windowWidth) || (window->getViewportHeight() != windowHeight))
				scaleScene();
			// objects
			updateObjects(dt);
			// particles
			updateParticles(dt);
			// transitions
			if (isTransitioning() && (transitionCounter > 0.0f))
			{
				transitionCounter -= dt * 1.f;
				if (transitionCounter < 0.0f)
					transitionCounter = 0.0;
			}
		}

		void Scene::updateObjects(float dt)
		{
			for (Layer *layer : layerStack->layers)
			{
				if (layer->ySort)
					layer->sort();
				for (Object *obj : layer->objects)
				{
					if (obj == NULL)
						removeObject(obj, layer->name);
					else
					{
						obj->updateAnimation(dt);
						obj->updateSqueezeEffect(dt);
						obj->modelScale = glm::vec3(viewportScale, 1.f);
						glm::vec2 velocity = obj->velocity * glm::vec2(dt);
						if (obj->collisionLayers.size() > 0)
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
			// start postprocessing scene capture
			if (enablePostprocessing)
				postProcessing->beginRender();
			// render grid
			if (grid->active)
				grid->render(
					(float)windowWidth,
					(float)windowHeight,
					camera->cameraScaledPosition.x,
					camera->cameraScaledPosition.y,
					camera->zoom);
			// render scene objects
			for (Layer *layer : layerStack->layers)
				if (!layer->hide)
					for (Object *obj : layer->objects)
					{
						renderObject(obj, layer);
						for (Object* childObject : obj->children)
							renderObject(childObject, layer);
					}
			// finish and render postprocessing
			if (enablePostprocessing)
			{
				postProcessing->endRender();
				postProcessing->render(
					(float)glfwGetTime(),
					greyscale,
					wavey,
					blur,
					screenShake,
					transitionEffect,
					fadeinTransition,
					fadeoutTransition,
					transitionCounter,
					(float)windowWidth,
					(float)windowHeight);
			}
		}

		void Scene::renderObject(Object *obj, Layer *layer)
		{
			// calculating view matrix
			glm::mat4 view;
			if (!layer->applyCamera)
				view = glm::mat4(1.0f);
			else
				view = camera->getViewMatrix(layer->depth);

			// calculating projection matrix
			glm::mat4 projection = camera->getProjectionMatrix(layer->applyCamera);
			// get object position
			glm::vec2 pos = obj->getScaledPosition();

			std::vector<Light *> lights;
			if (enableLighting)
				lights = lightSystem->getScaledLights(obj->modelScale);
			else
				lights = {};
				
			if (obj->renderMethod == "sprite")
			{
				spriteRenderer->render(
					time,
					obj->calculateModelMatrix(pos, 1.f),
					view,
					projection,
					obj->texture,
					obj->useTexture,
					obj->useColorTexture,
					obj->numberOfRows,
					obj->numberOfCols,
					obj->textureOffset,
					obj->color,
					obj->alpha,
					ambientLighting,
					lights,
					obj->outline,
					obj->size.x / obj->size.y,
					distortionTexture,
					obj->useDistortion,
					obj->scrollDistortionX,
					obj->scrollDistortionY,
					obj->distortionSpeed,
					obj->flipX,
					obj->flipY,
					layer->alpha,
					obj->useSqueeze,
					obj->deformationMagnitude
					);
			}
			else if (obj->renderMethod == "particles")
			{
				ParticleGenerator *pg = (ParticleGenerator *)obj;
				particleRenderer->render(
					pg->particles,
					pg->calculateModelMatrix(pos, 1.f),
					view,
					projection,
					pg->texture.textureID,
					pg->useTexture,
					pg->color,
					pg->alpha,
					layer->alpha,
					ambientLighting,
					lights
					);
			}
			else if (obj->renderMethod == "text")
			{
				Text *text = (Text *)obj;
				textRenderers[text->fontName]->render(
					text->text,
					text->calculateModelMatrix(pos, 1.f),
					view,
					projection,
					text->color,
					text->alpha,
					text->getScale() * glm::vec2(text->modelScale.x, text->modelScale.y),
					layer->alpha);
			}
			else if (obj->renderMethod == "line")
			{
				shapes::Line *line = (shapes::Line *)obj;
				lineRenderer->render(
					line->p1,
					line->p2,
					view,
					projection,
					line->color,
					line->alpha,
					layer->alpha);
			}
			else if (obj->renderMethod == "circle")
			{
				shapes::Circle *circle = (shapes::Circle *)obj;
				circleRenderer->render(
					circle->thickness,
					circle->fade,
					obj->calculateModelMatrix(pos, 1.f),
					view,
					projection,
					circle->color,
					circle->alpha,
					layer->alpha);
			}
			else if (obj->renderMethod == "batch_sprite")
			{
				BatchSprite *batchSprite = (BatchSprite *)obj;
				batchSprite->render(
					viewportScale,
					view,
					projection,
					batchSprite->texture,
					batchSprite->alpha,
					ambientLighting,
					lights
					);
			}
			else
			{
				std::cout << "Undefined render method" << std::endl;
			}

			col::BoundingBox bbox = obj->getBoundingBox();
			// render axis lines
			if (obj->showAxes)
			{
				// axes lines
				for (glm::vec2 v : bbox.getDebugAxes())
				{
					lineRenderer->render(
						bbox.center * viewportScale,
						v * viewportScale,
						view,
						projection,
						glm::vec3(0.f, 0.f, 1.f),
						0.5f,
						layer->alpha);
				}
			}
			// render lines to display object's bounding box
			if (obj->showBoundingBox)
			{
				// connect bbox vertices
				std::vector<glm::vec2> vertices = bbox.vertices;
				for (unsigned int i = 1; i < 4; i++)
				{
					lineRenderer->render(
						vertices[i - 1] * viewportScale,
						vertices[i] * viewportScale,
						view,
						projection,
						obj->color,
						obj->alpha,
						layer->alpha);
				}
				lineRenderer->render(
					vertices[3] * viewportScale,
					vertices[0] * viewportScale,
					view,
					projection,
					obj->color,
					obj->alpha,
					layer->alpha);
			}
		}

		void Scene::clear()
		{
			lightSystem->clear();
			for (Layer *layer : layerStack->layers)
				layer->objects.clear();
			collisionLayers.clear();
		}

		unsigned int Scene::getTextureBuffer()
		{
			return postProcessing->textureColorbuffer;
		}

		void Scene::setScreenShake(bool v)
		{
			screenShake = v;
		}

		void Scene::setPostProcessingEffect(PostProcessing::PostProcessingEffect effect)
		{
			if (effect == PostProcessing::None)
			{
				greyscale = false;
				wavey = false;
				blur = false;
			}
			else if (effect == PostProcessing::GREYSCALE)
			{
				greyscale = true;
				wavey = false;
				blur = false;
			}
			else if (effect == PostProcessing::WAVEY)
			{
				greyscale = false;
				wavey = true;
				blur = false;
			}
			else if (effect == PostProcessing::BLUR)
			{
				greyscale = false;
				wavey = false;
				blur = true;
			}
		}

		bool Scene::isTransitioning()
		{
			return (fadeinTransition) || (fadeoutTransition);
		}

		bool Scene::isBeginSceneTransitionFinished()
		{
			return (transitionCounter == 0.0f) && (fadeinTransition);
		}

		bool Scene::isEndSceneTransitionFinished()
		{
			return (transitionCounter == 0.0f) && (fadeoutTransition);
		}

		void Scene::resetTransitions()
		{
			fadeinTransition = false;
			fadeoutTransition = false;
			transitionCounter = 1.0f;
		}

		void Scene::beginSceneTransition()
		{
			fadeinTransition = true;
			fadeoutTransition = false;
			transitionCounter = 1.0f;
		}

		void Scene::endSceneTransition()
		{
			fadeoutTransition = true;
			fadeinTransition = false;
			transitionCounter = 1.0f;
		}

		void Scene::setTransitionType(PostProcessing::TransitionEffect effect)
		{
			transitionEffect = effect;
		}

		void Scene::scaleScene()
		{
			windowWidth = window->getViewportWidth();
			windowHeight = window->getViewportHeight();
			// scale postprocessing texture
			if (enablePostprocessing)
			{
				delete postProcessing;
				postProcessing = new s2d::PostProcessing(resourceManager->getShader("postprocessing"), windowWidth, windowHeight);
			}
		}

		bool Scene::isTileCollideable(int x, int y)
		{
			if ((x >= 0) && (x < tilemapSize.x) && (y >= 0) && (y < tilemapSize.y))
			{
				return collisionTilemap[y][x].collideable;
			}
			return 0;
		}

		col::BoundingBox Scene::getTileBoundingBox(int x, int y)
		{
			return col::BoundingBox(x * tileSize, y * tileSize, tileSize, tileSize, 0.0f);
		}

		void Scene::addObjectToCollisionLayer(Object* obj, unsigned int collisionLayer)
		{
			if (collisionLayer > 2)
			{
				std::cout << "Collision layer must be 0, 1, 2" << std::endl;
				return;
			}
			collisionLayers[collisionLayer].push_back(obj);
		}

		void Scene::removeObjectFromCollisionLayer(Object* obj, unsigned int collisionLayer)
		{
			if (collisionLayer > 2)
			{
				std::cout << "Collision layer must be 0, 1, 2" << std::endl;
				return;
			}
			collisionLayers[collisionLayer].erase(
				std::remove(
					collisionLayers[collisionLayer].begin(), 
					collisionLayers[collisionLayer].end(),
					obj
				),
				collisionLayers[collisionLayer].end()
			);
		}

		glm::vec2 Scene::resolveTilemapCollisions(Object* obj, glm::vec2 velocity)
		{
			obj->resetCollisionData(tilemapLayer);

			// get sprite's current position and tile
			glm::vec2 pos = obj->getPosition();
			int tileX = (int)(pos.x / tileSize);
			int tileY = (int)(pos.y / tileSize);
			float d = 0.f;

			for (int x = std::max(tileX - 3, 0); x < std::min(tileX + 3, (int)tilemapSize.x); x++)
			{
				for (int y = std::max(tileY - 3, 0); y < std::min(tileY + 3, (int)tilemapSize.y); y++)
				{
					s2d::Tile tile = collisionTilemap[y][x];
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
								obj->velocity.y = 0.f;
								obj->collisionData[tilemapLayer].on_floor = true;
							}
							else
							{
								velocity.y = -d;
								obj->collisionData[tilemapLayer].on_ceiling = true;
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
								obj->collisionData[tilemapLayer].on_wall_right = true;
							}
							else
							{
								velocity.x = -d;
								obj->collisionData[tilemapLayer].on_wall_left = true;
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
			for (s2d::Object *e : collisionLayers[collisionLayer])
			{
				// y-axis collision
				bool yCollision = obj->getShiftedBoundingBox(0.0f, velocity.y).intersectsBounds(e->getBoundingBox());
				if (yCollision)
				{
					d = obj->getBoundingBox().getMinDistanceFromBounds(e->getBoundingBox(), "y");
					if (velocity.y >= 0)
					{
						velocity.y = d;
						obj->velocity.y = 0.f;
						obj->collisionData[collisionLayer].on_floor = true;
					}
					else
					{
						velocity.y = -d;
						obj->collisionData[collisionLayer].on_ceiling = true;
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
						obj->collisionData[collisionLayer].on_wall_right = true;
					}
					else
					{
						velocity.x = -d;
						obj->collisionData[collisionLayer].on_wall_left = true;
					}
				}
			}
			return velocity;
		}

		std::vector<std::string> Scene::getEntityGroupNames()
		{
			std::vector<std::string> keys;
			for (auto it = entityGroups.begin(); it != entityGroups.end(); it++) {
				keys.push_back(it->first);
			}
			return keys;
		}

		void Scene::addEntityToGroup(Object* obj, std::string group)
		{
			if (entityGroups.find(group) == entityGroups.end())
				entityGroups[group] = {obj};
			else
				entityGroups[group].push_back(obj);
		}

		void Scene::removeEntityFromGroup(Object* obj, std::string group)
		{
			entityGroups[group].erase(std::remove(entityGroups[group].begin(), entityGroups[group].end(), obj), entityGroups[group].end());
		}

		std::vector<Object*> Scene::getEntityGroup(std::string group)
		{
			return entityGroups[group];
		}

		void Scene::addTextRenderer(const std::string name, const std::string fontPath, unsigned int fontSize)
		{
			textRenderers[name] = new s2d::TextRenderer(fontPath, fontSize, resourceManager->getShader("text")); 
		}

		void Scene::addParticleEffect(glm::vec2 position, ParticleSettings settings, std::string layer)
		{
			ParticleGenerator* pg = new ParticleGenerator(settings);
			pg->setPosition(position);
			addObject(pg, layer);
			particles.push_back(pg);
		}

		void Scene::updateParticles(float dt)
		{
			for (ParticleGenerator* pg : particles)
			{
				pg->update(dt);
				if (pg->finished)
				{
					removeObject(pg, pg->layer);
					particles.erase(std::remove(particles.begin(), particles.end(), pg), particles.end());
					delete pg;
				}
			}
		}

		void Scene::init()
		{
			camera = new s2d::Camera(window);
			lightSystem = new s2d::LightSystem();
			distortionTexture = resourceManager->getTexture("distortion_texture").textureID;
			layerStack = new LayerStack();
			// renderers
			spriteRenderer = new s2d::SpriteRenderer(resourceManager->getShader("sprite"));
			particleRenderer = new s2d::ParticleRenderer(resourceManager->getShader("particle"));
			lineRenderer = new s2d::LineRenderer(resourceManager->getShader("line"));
			circleRenderer = new s2d::CircleRenderer(resourceManager->getShader("circle"));
			postProcessing = new s2d::PostProcessing(resourceManager->getShader("postprocessing"), windowWidth, windowHeight);
			grid = new s2d::BackgroundGrid(resourceManager->getShader("grid"));
			textRenderers["default"] = new s2d::TextRenderer("assets/fonts/OpenSans-Regular.ttf", 48, resourceManager->getShader("text"));
		}

	}
}
