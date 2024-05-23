#pragma once

#include "Crocodile/s3d/Surface.h"

using namespace Crocodile;

class EarthSurface : public s3d::Surface
{
    public:
        EarthSurface(s3d::HeightMap heightMap, graphics::Shader* shader);
        ~EarthSurface();

        void customRender(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 cameraPosition,
            float ambientLighting,
            glm::vec3 lightPosition,
            glm::vec3 lightColour
        );
};
