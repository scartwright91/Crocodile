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

        struct HeightMap
        {
            int nCols;
            int nRows;
            std::vector<float> heights;
            float minHeight;
            float maxHeight;
            HeightMap();
            HeightMap(std::string heightMapPath, float scale, bool inverse);
            HeightMap(int nCols, int nRows, std::vector<float> heights);
            void init();
            float getHeight(int i, int j);
        };

        class CROCODILE_API Surface
        {
            public:
                Surface(HeightMap heightMap, graphics::Shader* shader) : heightMap(heightMap), shader(shader) {};
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

                HeightMap heightMap;

                // This is for calculating the normal vector. A larger value will create a smoother effect.
                int adjacentVertexDistance = 1;
                float alpha = 1.0f;
                glm::vec3 colour = glm::vec3(1.f);
                glm::vec2 pos = glm::vec2(0.f);
                glm::vec2 gridSize = glm::vec2(1.f);

            private:

                graphics::Shader* shader = nullptr;
                unsigned int terrainVAO;
                int numTrisPerStrip, numStrips;

                void init();

                glm::vec3 calculateNormal(int i, int j);
                glm::vec3 interpolateRGB(const glm::vec3 colour1, const glm::vec3 colour2, float t);

        };
    }
}
