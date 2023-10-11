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

		void Scene::addObject(s2d::Object *object, std::string layerName)
		{
			Layer *layer = layerStack->getLayer(layerName);
			if (layer != NULL)
				layer->addObject(object);
			else
				std::cout << "Layer " << layerName << " does not exist in layerstack" << std::endl;
		}

		void Scene::removeObject(s2d::Object *object, std::string layerName)
		{
			Layer *layer = layerStack->getLayer(layerName);
			if (layer != NULL)
			{
				if (object != NULL)
					layer->removeObject(object);
			}
			else
				std::cout << "Layer " << layerName << " does not exist in layerstack" << std::endl;
		}

		void Scene::update(float dt)
		{
			// update timer (used for shaders)
			time += 0.05 * dt;
			if (time >= 1.0)
				time = 0.0;
			if (enableScaling)
				viewportScale = window->getViewportScale();
			else
				viewportScale = glm::vec2(1.f);
			camera->update(viewportScale);
			// screen resizing
			if ((window->getViewportWidth() != windowWidth) || (window->getViewportHeight() != windowHeight))
				scaleScene(window->getViewportScale());
			// objects
			updateObjects(dt);
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
						obj->modelScale = glm::vec3(viewportScale, 1.f);
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
					blur,
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
				view = camera->getViewMatrix(layer->depth);

			// calculating projection matrix
			glm::mat4 projection = camera->getProjectionMatrix(layer->applyZoom);
			// get object position
			glm::vec2 pos = obj->getScaledPosition();

			if (obj->renderMethod == "sprite")
			{
				std::vector<Light *> lights;
				if (enableLighting)
					lights = lightSystem->getScaledLights(obj->modelScale);
				else
					lights = {};

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
					layer->alpha);
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
					layer->alpha);
			}
			else if (obj->renderMethod == "text")
			{
				Text *text = (Text *)obj;
				textRenderer->render(
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
					view,
					projection,
					batchSprite->texture,
					batchSprite->alpha);
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
				for (unsigned int i = 0; i < 4; i++)
				{
					shapes::Circle c(4);
					circleRenderer->render(
						c.thickness,
						c.fade,
						c.calculateModelMatrix((vertices[i]) * viewportScale, 1.f),
						view,
						projection,
						glm::vec3(1., 0., 0.),
						1.,
						layer->alpha);
				}
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

		void Scene::scaleScene(glm::vec2 s)
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

		void Scene::init()
		{
			camera = new s2d::Camera(window);
			lightSystem = new s2d::LightSystem();
			distortionTexture = resourceManager->getTexture("waterDUDV").textureID;
			layerStack = new LayerStack();
			// renderers
			spriteRenderer = new s2d::SpriteRenderer(resourceManager->getShader("sprite"));
			particleRenderer = new s2d::ParticleRenderer(resourceManager->getShader("particle"));
			lineRenderer = new s2d::LineRenderer(resourceManager->getShader("line"));
			circleRenderer = new s2d::CircleRenderer(resourceManager->getShader("circle"));
			postProcessing = new s2d::PostProcessing(resourceManager->getShader("postprocessing"), windowWidth, windowHeight);
			textRenderer = new s2d::TextRenderer("assets/fonts/OpenSans-Regular.ttf", resourceManager->getShader("text"));
			grid = new s2d::BackgroundGrid(resourceManager->getShader("grid"));
		}

	}
}
