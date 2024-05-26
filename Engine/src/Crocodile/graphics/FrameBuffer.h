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

                unsigned int textureColorbuffer;
                unsigned int width, height;
                unsigned int framebuffer, RBO;
                unsigned int VAO;

                void bind();
                void unbind();
                void resize(unsigned int width, unsigned int height);

            private:
                void init();

        };

    }
}

