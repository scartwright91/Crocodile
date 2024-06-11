#pragma once

#include <GL/glew.h>

#include "iostream"

#include "../Core.h"


namespace Crocodile
{
    namespace graphics
    {

        class CROCODILE_API FrameBuffer
        {

            public:
                FrameBuffer(unsigned int width, unsigned int height);
                ~FrameBuffer();

                unsigned int m_textureColorbuffer;
                unsigned int m_width, m_height;
                unsigned int m_framebuffer, m_RBO;
                unsigned int m_VAO;

                void bind();
                void unbind();
                void resize(unsigned int width, unsigned int height);

            private:
                void init();

        };

    }
}

