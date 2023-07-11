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
		}

		Scene::~Scene()
		{
			delete spriteRenderer;
			delete particleRenderer;
			delete textRenderer;
			delete postProcessing;
			delete grid;
			delete lightSystem;
			delete camera;
			delete layerStack;
		}

		void Scene::addChild(s2d::Object *object, std::string layerName)
		{
			Layer *layer = layerStack->getLayer(layerName);
			if (layer != NULL)
				layer->addObject(object);
		}

		void Scene::removeChild(s2d::Object *object, std::string layerName)
		{
			Layer *layer = layerStack->getLayer(layerName);
			layer->removeObject(object);
		}

		void Scene::update(float dt)
		{
			// update timer (used for shaders)
			time += 0.05 * dt;
			if (time >= 1.0)
				time = 0.0;
			camera->update();
			// screen resizing
			if ((window->getViewportWidth() != windowWidth) || (window->getViewportHeight() != windowHeight))
				scaleScene(window->getviewportScale());
			// objects
			updateObjects(dt);
			// transitions
			if (isTransitioning() && (transitionCounter > 0.0f))
			{
				transitionCounter -= dt * 0.5;
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
					std::vector<Object *> objectStack = {obj};
					for (Object *child : obj->children)
						objectStack.push_back(child);
					for (Object *o : objectStack)
					{
						// updating animations
						o->updateAnimation(dt);
						// move objects relative to camera based on render depth
						if (layer->depth != 0.f)
						{
							glm::vec2 startingPos = o->getStartingPosition();
							glm::vec2 newPos = glm::vec2(
								startingPos.x - camera->cameraPosition.x * layer->depth,
								startingPos.y + (windowHeight - camera->cameraPosition.y) * layer->depth);
							o->setPosition(newPos);
						}
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
					windowWidth,
					windowHeight,
					camera->cameraPosition.x,
					camera->cameraPosition.y,
					camera->zoom);
			// render scene objects
			for (Layer *layer : layerStack->layers)
				if (!layer->hide)
					for (Object *obj : layer->objects)
					{
						std::vector<Object *> objectStack = {obj};
						for (Object *child : obj->children)
							objectStack.push_back(child);
						for (Object *o : objectStack)
							renderObject(o, layer);
					}
			// finish and render postprocessing
			if (enablePostprocessing)
			{
				postProcessing->endRender();
				postProcessing->render(
					(float)glfwGetTime(),
					greyscale,
					wavey,
					screenShake,
					transitionEffect,
					fadeinTransition,
					fadeoutTransition,
					transitionCounter,
					windowWidth,
					windowHeight);
			}
		}

		void Scene::renderObject(Object *obj, Layer *layer)
		{
			// calculating view matrix
			glm::mat4 view;
			if (!layer->cameraScroll)
				view = glm::mat4(1.0f);
			else
				view = camera->getViewMatrix();

			// calculating projection matrix
			glm::mat4 projection = camera->getProjectionMatrix(layer->applyZoom);
			// get object position
			glm::vec2 pos = obj->getScaledPosition();

			if (obj->renderMethod == "sprite")
			{
				std::vector<Light *> lights;
				if (enableLighting)
				{
					if (layer->depth == 0.f)
						lights = lightSystem->getScaledLights(obj->modelScale);
					else
						lights = {};
				}
				else
					lights = {};

				spriteRenderer->render(
					time,
					obj->calculateModelMatrix(pos),
					view,
					projection,
					obj->texture.textureID,
					obj->useTexture,
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
					layer->alpha);
			}
			else if (obj->renderMethod == "particles")
			{
				ParticleGenerator *pg = (ParticleGenerator *)obj;
				particleRenderer->render(
					pg->particles,
					pg->calculateModelMatrix(pos),
					view,
					projection,
					pg->texture.textureID,
					pg->useTexture,
					pg->color,
					pg->alpha,
					layer->alpha);
			}
			else if (obj->renderMethod == "text")
			{
				Text *text = (Text *)obj;
				textRenderer->render(
					text->text,
					text->calculateModelMatrix(pos),
					view,
					projection,
					text->color,
					text->alpha,
					text->textScale,
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
			else
			{
				std::cout << "Undefined render method" << std::endl;
			}
		}

		void Scene::clear()
		{
			lightSystem->clear();
			for (Layer *layer : layerStack->layers)
				layer->objects.clear();
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
			}
			else if (effect == PostProcessing::GREYSCALE)
			{
				greyscale = true;
				wavey = false;
			}
			else if (effect == PostProcessing::WAVEY)
			{
				greyscale = false;
				wavey = true;
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

		void Scene::scaleScene(glm::vec2 s)
		{
			windowWidth = window->getViewportWidth();
			windowHeight = window->getViewportHeight();
			if (!enableScaling)
				return;
			std::cout << "Scale scene: (" << s.x << ", " << s.y << ")" << std::endl;
			for (Layer *layer : layerStack->layers)
				for (Object *obj : layer->objects)
				{
					std::vector<Object *> objectStack = {obj};
					for (Object *child : obj->children)
						objectStack.push_back(child);
					for (Object *o : objectStack)
						o->scale(s);
				}
			// scale postprocessing texture
			if (enablePostprocessing)
			{
				delete postProcessing;
				postProcessing = new s2d::PostProcessing(resourceManager->getShader("postprocessing"), windowWidth, windowHeight);
			}
		}

		void Scene::init()
		{
			camera = new s2d::Camera(window);
			spriteRenderer = new s2d::SpriteRenderer(resourceManager->getShader("sprite"));
			particleRenderer = new s2d::ParticleRenderer(resourceManager->getShader("particle"));
			lineRenderer = new s2d::LineRenderer(resourceManager->getShader("line"));
			postProcessing = new s2d::PostProcessing(resourceManager->getShader("postprocessing"), windowWidth, windowHeight);
			textRenderer = new s2d::TextRenderer("assets/fonts/FFF_Tusj.ttf", resourceManager->getShader("text"));
			grid = new s2d::BackgroundGrid(resourceManager->getShader("grid"));
			lightSystem = new s2d::LightSystem();
			distortionTexture = resourceManager->getTexture("waterDUDV").textureID;
			layerStack = new LayerStack();
		}

	}
}
