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
                    glm::mat4 projection
                );

            private:
                std::string type;
                int nRows, nCols;
                std::vector<float> heights = {};
                std::string heightMapPath;

                graphics::Shader* shader = nullptr;
                unsigned int terrainVAO;
                int numTrisPerStrip, numStrips;

                void init();
                void createSurfaceFromData();
                void createSurfaceFromHeightMap();

                glm::vec3 interpolateRGB(const glm::vec3 colour1, const glm::vec3 colour2, float t);

        };
    }
}
