#include "PostProcessing.h"

namespace Crocodile
{
	namespace s2d
	{
		PostProcessing::PostProcessing(graphics::Shader *shader, unsigned int width, unsigned int height) : width(width), height(height)
		{
			this->shader = shader;
			this->shader->use();
			this->shader->setInt("u_Scene", 0);
			init();
		}

		PostProcessing::~PostProcessing()
		{
		}

		void PostProcessing::beginRender()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void PostProcessing::endRender()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void PostProcessing::render(
			float dt,
			bool greyscale,
			bool wavey,
			bool screenShake,
			TransitionEffect effect,
			bool fadeinTransition,
			bool fadeoutTransition,
			float transitionCounter,
			float width,
			float height)
		{
			shader->use();
			shader->setFloat("u_DeltaTime", dt);
			shader->setBool("u_Greyscale", greyscale);
			shader->setBool("u_Wavey", wavey);
			shader->setBool("u_ScreenShake", screenShake);
			if (effect == FADE)
				shader->setBool("u_FadeTransition", true);
			else if (effect == DIAMOND)
				shader->setBool("u_DiamondTransition", true);
			shader->setBool("u_BeginScene", fadeinTransition);
			shader->setBool("u_EndScene", fadeoutTransition);
			shader->setFloat("u_TransitionCounter", transitionCounter);
			shader->setFloat("u_ScreenWidth", width);
			shader->setFloat("u_ScreenHeight", height);
			glBindVertexArray(VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		void PostProcessing::resize(unsigned int width, unsigned int height)
		{
			this->width = width;
			this->height = height;
			init();
		}

		void PostProcessing::init()
		{
			// init framebuffer
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

			// init color buffer
			glGenTextures(1, &textureColorbuffer);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

			// render buffer
			glGenRenderbuffers(1, &RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);					  // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // now actually attach it
			// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::POSTPROCESSING::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// create render quad
			float quadVertices[] = {
				// positions   // texCoords
				-1.0f, 1.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f,
				1.0f, -1.0f, 1.0f, 0.0f,

				-1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, -1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f};

			unsigned int VBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
		}
	}
}
