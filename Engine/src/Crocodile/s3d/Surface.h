#pragma once

#include <vector>
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core.h"
#include "../ResourceManager.h"
#include "../graphics/Shader.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API Surface
        {
            public:
                Surface(std::string heightMapPath, graphics::Shader* shader); // from heightmap
                Surface(std::vector<float> heights, unsigned int rows, unsigned int cols, graphics::Shader* shader); // from data
                ~Surface();

                void render(
                    glm::mat4 model,
                    glm::mat4 view,
                    glm::mat4 projection,
                    float ambientLighting,
                    glm::vec3 lightPosition,
                    glm::vec3 lightColour
                );

                float maxHeight = 0.0f;

            private:
                std::string heightMapPath;
                std::string type;
                int nRows, nCols, nChannels;
                std::vector<float> heights = {};
                std::vector<float> normals = {};

                graphics::Shader* shader = nullptr;
                unsigned int terrainVAO;
                int numTrisPerStrip, numStrips;

                void init();
                void createSurface();

                glm::vec3 interpolateRGB(const glm::vec3 colour1, const glm::vec3 colour2, float t);

        };
    }
}
