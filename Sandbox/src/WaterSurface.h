#pragma once

#include "Crocodile/s3d/Surface.h"
#include "Crocodile/graphics/FrameBuffer.h"
#include "Crocodile/graphics/Window.h"


using namespace Crocodile;

class WaterSurface : public s3d::Surface
{
    public:
        WaterSurface(
            graphics::Window* window,
            s3d::HeightMap heightMap,
            graphics::Shader* shader
        );
        ~WaterSurface();

        void customRender(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            unsigned int reflectionTexture,
            unsigned int refractionTexture,
            unsigned int dudvTexture
        );
};
