#pragma once

#include <vector>
#include <algorithm>

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

#include "rendering/SpriteRenderer.h"
#include "rendering/ParticleRenderer.h"
#include "rendering/TextRenderer.h"
#include "rendering/PostProcessing.h"
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

			graphics::Window *window;
			s2d::Camera *camera;

			int windowWidth;
			int windowHeight;

			void addObject(s2d::Object *object, std::string layerName);
			void removeObject(s2d::Object *object, std::string layerName);

			void update(float dt);
			void updateObjects(float dt);
			void render();
			void clear();
			void scaleScene();

			// postprocessing
			unsigned int getTextureBuffer();
			void setScreenShake(bool v);
			void setPostProcessingEffect(PostProcessing::PostProcessingEffect effect);

			// transitions
			float transitionSpeed = 1.f;
			bool isTransitioning();
			bool isBeginSceneTransitionFinished();
			bool isEndSceneTransitionFinished();
			void resetTransitions();
			void beginSceneTransition();
			void endSceneTransition();
			void setTransitionType(PostProcessing::TransitionEffect effect);

			// options
			bool enableScaling = true;
			bool enableLighting = true;
			bool enablePostprocessing = true;

			// lights
			float ambientLighting = 1.0f;
			LightSystem *lightSystem;

			// distortion
			float time = 0.f;
			unsigned int distortionTexture;

			// background grid
			BackgroundGrid *grid;

			// render layers
			LayerStack *layerStack = nullptr;

			// rendering
			ResourceManager *resourceManager;
			virtual void renderObject(Object *obj, Layer *layer);

			// render pipelines
			SpriteRenderer *spriteRenderer;
			ParticleRenderer *particleRenderer;
			TextRenderer *textRenderer;
			LineRenderer *lineRenderer;
			CircleRenderer *circleRenderer;
			PostProcessing *postProcessing;

			// collisions
			void addObjectToCollisionLayer(Object* obj, unsigned int collisionLayer);
			void removeObjectFromCollisionLayer(Object* obj, unsigned int collisionLayer);

		private:

			// postprocessing
			bool greyscale = false;
			bool wavey = false;
			bool blur = false;
			bool screenShake = false;

			// transitions
			PostProcessing::TransitionEffect transitionEffect = PostProcessing::NO_TRANSITION;
			float transitionCounter = 1.0;
			bool fadeinTransition = false;
			bool fadeoutTransition = false;

			// scaling
			glm::vec2 viewportScale = glm::vec2(1.f);

			// collisions
			std::map<unsigned int, std::vector<Object*>> collisionLayers = {
				{0, {}},
				{1, {}},
				{2, {}}
			};
			void resolveCollisions(Object* obj, unsigned int collisionLayer);

			void init();
		};

	}
}
