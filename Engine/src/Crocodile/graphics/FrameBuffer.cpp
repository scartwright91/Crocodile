#include "FrameBuffer.h"

#include "Crocodile/utils/Logger.h"

namespace Crocodile
{
    namespace graphics
    {

        FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) : m_width(width), m_height(height)
        {
            init();
        }

        FrameBuffer::~FrameBuffer()
        {
			glDeleteFramebuffers(1, &m_framebuffer);
        }

		void FrameBuffer::bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void FrameBuffer::unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

        void FrameBuffer::resize(unsigned int width, unsigned int height)
        {
            m_width = width;
            m_height = height;
            init();
        }

		void FrameBuffer::init()
		{
			// init framebuffer
			glGenFramebuffers(1, &m_framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

			// init color buffer
			glGenTextures(1, &m_textureColorbuffer);
			glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);

			// render buffer
			glGenRenderbuffers(1, &m_RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);					  // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO); // now actually attach it
			// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				LOG(WARNING, "Framebuffer is not complete");
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
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
		}

    }
}

