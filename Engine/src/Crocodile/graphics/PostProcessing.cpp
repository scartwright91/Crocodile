#include "PostProcessing.h"

namespace Crocodile
{
	namespace graphics
	{
		PostProcessing::PostProcessing(graphics::Shader *shader, unsigned int width, unsigned int height)
		{
			m_fb = new FrameBuffer(width, height);
			m_shader = shader;
			m_shader->use();
			m_shader->setInt("u_Scene", 0);
		}

		PostProcessing::~PostProcessing()
		{
			delete m_fb;
		}

		void PostProcessing::update(float dt)
		{
			// transitions
			if (isTransitioning() && (m_transitionCounter > 0.0f))
			{
				m_transitionCounter -= dt * 1.f;
				if (m_transitionCounter < 0.0f)
					m_transitionCounter = 0.0;
			}
		}

		void PostProcessing::beginRender()
		{
			m_fb->bind();
		}

		void PostProcessing::endRender()
		{
			m_fb->unbind();
		}

		unsigned int PostProcessing::getTextureBuffer()
		{
			return m_fb->m_textureColorbuffer;
		}

		void PostProcessing::setScreenShake(bool v)
		{
			m_screenShake = v;
		}

		void PostProcessing::setPostProcessingEffect(PostProcessingEffect effect)
		{
			if (effect == None)
			{
				m_greyscale = false;
				m_wavey = false;
				m_blur = false;
			}
			else if (effect == GREYSCALE)
			{
				m_greyscale = true;
				m_wavey = false;
				m_blur = false;
			}
			else if (effect == WAVEY)
			{
				m_greyscale = false;
				m_wavey = true;
				m_blur = false;
			}
			else if (effect == BLUR)
			{
				m_greyscale = false;
				m_wavey = false;
				m_blur = true;
			}
		}

		bool PostProcessing::isTransitioning()
		{
			return (m_fadeinTransition) || (m_fadeoutTransition);
		}

		bool PostProcessing::isBeginSceneTransitionFinished()
		{
			return (m_transitionCounter == 0.0f) && (m_fadeinTransition);
		}

		bool PostProcessing::isEndSceneTransitionFinished()
		{
			return (m_transitionCounter == 0.0f) && (m_fadeoutTransition);
		}

		void PostProcessing::resetTransitions()
		{
			m_fadeinTransition = false;
			m_fadeoutTransition = false;
			m_transitionCounter = 1.0f;
		}

		void PostProcessing::beginSceneTransition()
		{
			m_fadeinTransition = true;
			m_fadeoutTransition = false;
			m_transitionCounter = 1.0f;
		}

		void PostProcessing::endSceneTransition()
		{
			m_fadeoutTransition = true;
			m_fadeinTransition = false;
			m_transitionCounter = 1.0f;
		}

		void PostProcessing::setTransitionType(graphics::TransitionEffect effect)
		{
			m_transitionEffect = effect;
		}

		void PostProcessing::render(float dt)
		{
			m_shader->use();
			m_shader->setFloat("u_DeltaTime", dt);
			m_shader->setBool("u_Greyscale", m_greyscale);
			m_shader->setBool("u_Wavey", m_wavey);
			m_shader->setBool("u_Blur", m_blur);
			m_shader->setBool("u_ScreenShake", m_screenShake);
			if (m_transitionEffect == FADE)
				m_shader->setBool("u_FadeTransition", true);
			else if (m_transitionEffect == DIAMOND)
				m_shader->setBool("u_DiamondTransition", true);
			m_shader->setBool("u_BeginScene", m_fadeinTransition);
			m_shader->setBool("u_EndScene", m_fadeoutTransition);
			m_shader->setFloat("u_TransitionCounter", m_transitionCounter);
			m_shader->setFloat("u_ScreenWidth", (float)m_fb->m_width);
			m_shader->setFloat("u_ScreenHeight", (float)m_fb->m_height);
			glBindVertexArray(m_fb->m_VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_fb->m_textureColorbuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		void PostProcessing::resize(unsigned int width, unsigned int height)
		{
			m_fb->resize(width, height);
		}

	}
}
