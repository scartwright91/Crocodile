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

                void createSurface();

                void render(
                    glm::mat4 model,
                    glm::mat4 view,
                    glm::mat4 projection,
                    glm::vec3 cameraPosition,
                    float ambientLighting,
                    glm::vec3 lightPosition,
                    glm::vec3 lightColour
                );

                float maxHeight = 0.0f;
                // This is for calculating the normal vector. A larger value will create a smoother effect.
                int adjacentVertexPositions = 1;
                glm::vec3 colour = glm::vec3(1.f);

            private:
                std::string heightMapPath;
                std::string type;
                int nRows, nCols, nChannels;
                std::vector<float> heights = {};

                graphics::Shader* shader = nullptr;
                unsigned int terrainVAO;
                int numTrisPerStrip, numStrips;

                void init();

                glm::vec3 calculateNormal(int i, int j);
                glm::vec3 interpolateRGB(const glm::vec3 colour1, const glm::vec3 colour2, float t);

        };
    }
}
