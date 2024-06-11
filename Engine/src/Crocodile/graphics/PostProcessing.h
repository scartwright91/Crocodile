#pragma once

#include <glm/glm.hpp>
#include "../Core.h"

#include "Shader.h"
#include "FrameBuffer.h"

namespace Crocodile
{
	namespace graphics
	{

		enum PostProcessingEffect
		{
			None,
			GREYSCALE,
			WAVEY,
			BLUR
		};

		enum TransitionEffect
		{
			NO_TRANSITION,
			FADE,
			DIAMOND
		};

		class CROCODILE_API PostProcessing
		{

		public:
			PostProcessing(graphics::Shader *shader, unsigned int width, unsigned int height);
			~PostProcessing();

			FrameBuffer* m_fb = nullptr;
			
			void beginRender();
			void endRender();
			void render(float dt);

			void update(float dt);
			void resize(unsigned int width, unsigned int height);
			unsigned int getTextureBuffer();
			void setScreenShake(bool v);
			void setPostProcessingEffect(graphics::PostProcessingEffect effect);

			// transitions
			float transitionSpeed = 1.f;
			bool isTransitioning();
			bool isBeginSceneTransitionFinished();
			bool isEndSceneTransitionFinished();
			void resetTransitions();
			void beginSceneTransition();
			void endSceneTransition();
			void setTransitionType(graphics::TransitionEffect effect);

		private:
			Shader *m_shader = nullptr;

			// postprocessing
			bool m_greyscale = false;
			bool m_wavey = false;
			bool m_blur = false;
			bool m_screenShake = false;

			// transitions
			TransitionEffect m_transitionEffect = NO_TRANSITION;
			float m_transitionCounter = 1.0;
			bool m_fadeinTransition = false;
			bool m_fadeoutTransition = false;

		};
	}
}
