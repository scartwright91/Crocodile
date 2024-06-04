#include "PostProcessing.h"

namespace Crocodile
{
	namespace graphics
	{
		PostProcessing::PostProcessing(graphics::Shader *shader, unsigned int width, unsigned int height)
		{
			fb = new FrameBuffer(width, height);
			this->shader = shader;
			this->shader->use();
			this->shader->setInt("u_Scene", 0);
		}

		PostProcessing::~PostProcessing()
		{
			delete fb;
		}

		void PostProcessing::update(float dt)
		{
			// transitions
			if (isTransitioning() && (transitionCounter > 0.0f))
			{
				transitionCounter -= dt * 1.f;
				if (transitionCounter < 0.0f)
					transitionCounter = 0.0;
			}
		}

		void PostProcessing::beginRender()
		{
			fb->bind();
		}

		void PostProcessing::endRender()
		{
			fb->unbind();
		}

		unsigned int PostProcessing::getTextureBuffer()
		{
			return fb->textureColorbuffer;
		}

		void PostProcessing::setScreenShake(bool v)
		{
			screenShake = v;
		}

		void PostProcessing::setPostProcessingEffect(PostProcessingEffect effect)
		{
			if (effect == None)
			{
				greyscale = false;
				wavey = false;
				blur = false;
			}
			else if (effect == GREYSCALE)
			{
				greyscale = true;
				wavey = false;
				blur = false;
			}
			else if (effect == WAVEY)
			{
				greyscale = false;
				wavey = true;
				blur = false;
			}
			else if (effect == BLUR)
			{
				greyscale = false;
				wavey = false;
				blur = true;
			}
		}

		bool PostProcessing::isTransitioning()
		{
			return (fadeinTransition) || (fadeoutTransition);
		}

		bool PostProcessing::isBeginSceneTransitionFinished()
		{
			return (transitionCounter == 0.0f) && (fadeinTransition);
		}

		bool PostProcessing::isEndSceneTransitionFinished()
		{
			return (transitionCounter == 0.0f) && (fadeoutTransition);
		}

		void PostProcessing::resetTransitions()
		{
			fadeinTransition = false;
			fadeoutTransition = false;
			transitionCounter = 1.0f;
		}

		void PostProcessing::beginSceneTransition()
		{
			fadeinTransition = true;
			fadeoutTransition = false;
			transitionCounter = 1.0f;
		}

		void PostProcessing::endSceneTransition()
		{
			fadeoutTransition = true;
			fadeinTransition = false;
			transitionCounter = 1.0f;
		}

		void PostProcessing::setTransitionType(graphics::TransitionEffect effect)
		{
			transitionEffect = effect;
		}

		void PostProcessing::render(float dt)
		{
			shader->use();
			shader->setFloat("u_DeltaTime", dt);
			shader->setBool("u_Greyscale", greyscale);
			shader->setBool("u_Wavey", wavey);
			shader->setBool("u_Blur", blur);
			shader->setBool("u_ScreenShake", screenShake);
			if (transitionEffect == FADE)
				shader->setBool("u_FadeTransition", true);
			else if (transitionEffect == DIAMOND)
				shader->setBool("u_DiamondTransition", true);
			shader->setBool("u_BeginScene", fadeinTransition);
			shader->setBool("u_EndScene", fadeoutTransition);
			shader->setFloat("u_TransitionCounter", transitionCounter);
			shader->setFloat("u_ScreenWidth", (float)fb->width);
			shader->setFloat("u_ScreenHeight", (float)fb->height);
			glBindVertexArray(fb->VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fb->textureColorbuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		void PostProcessing::resize(unsigned int width, unsigned int height)
		{
			fb->resize(width, height);
		}

	}
}
