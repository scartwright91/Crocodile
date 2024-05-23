#pragma once

#include "Crocodile/s3d/Surface.h"

using namespace Crocodile;

class WaterSurface : public s3d::Surface
{
    public:
        WaterSurface(s3d::HeightMap heightMap, graphics::Shader* shader);
        ~WaterSurface();

        void customRender(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection
        );
};
