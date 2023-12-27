#pragma once

#include <vector>
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core.h"
#include "../ResourceManager.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API Surface
        {
            public:
                Surface(ResourceManager::TextureData heightMap); // from heightmap
                Surface(std::vector<glm::vec3> vertices); // from data
                ~Surface();

            private:
                std::string type;
                std::vector<glm::vec3> vertices = {};
                ResourceManager::TextureData heightMap;

                void init();
                void createSurfaceFromVertices();
                void createSurfaceFromHeightMap();
        };
    }
}
