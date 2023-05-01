#pragma once

#include <vector>
#include <map>
#include <algorithm>

#include "../Core.h"
#include "../ResourceManager.h"
#include "Camera.h"
#include "Object.h"
#include "Particles.h"
#include "Text.h"
#include "LightSystem.h"
#include "LayerStack.h"

#include "rendering/SpriteRenderer.h"
#include "rendering/ParticleRenderer.h"
#include "rendering/TextRenderer.h"
#include "rendering/PostProcessing.h"
#include "rendering/BackgroundGrid.h"

#include "../graphics/Window.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API Scene
		{

		public:

			Scene(graphics::Window* window, ResourceManager* resourceManager);
			~Scene();

			graphics::Window* window;
			s2d::Camera* camera;

			int windowWidth;
			int windowHeight;

			void addChild(s2d::Object* object, std::string layerName);
			void removeChild(s2d::Object* object, std::string layerName);

			void update(float dt);
			void updateObjects(float dt);
			void render();
			void clear();

			// postprocessing
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

			// lights
			bool enableLighting = true;
			float ambientLighting = 1.0f;
			LightSystem* lightSystem;

			// distortion
			float time = 0.f;
			unsigned int distortionTexture;

			// background grid
			BackgroundGrid* grid;

			// render layers
			LayerStack* layerStack = nullptr;

		private:

			// rendering
			ResourceManager* resourceManager;
			void renderObject(Object* obj, Layer* layer);

			// render pipelines
			SpriteRenderer* spriteRenderer;
			ParticleRenderer* particleRenderer;
			TextRenderer* textRenderer;

			// postprocessing
			bool greyscale = false;
			bool wavey = false;
			bool screenShake = false;
			PostProcessing* postProcessing;

			// transitions
			PostProcessing::TransitionEffect transitionEffect = PostProcessing::NO_TRANSITION;
			float transitionCounter = 1.0;
			bool fadeinTransition = false;
			bool fadeoutTransition = false;

			void scaleScene(glm::vec2 s);
			void init();

		};

	}
}
