#include "Surface.h"

namespace Crocodile
{
    namespace s3d
    {

        Surface::Surface(
            std::string heightMapPath,
            graphics::Shader* shader) : heightMapPath(heightMapPath), type("height_map"), shader(shader)
        {
            init();
        }

        Surface::Surface(
            std::vector<float> heights,
            unsigned int rows,
            unsigned int cols,
            graphics::Shader* shader) : heights(heights), nRows(rows), nCols(cols), type("data"), shader(shader)
        {
            init();
        }

        Surface::~Surface()
        {

        }

        void Surface::render(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            float ambientLighting,
            glm::vec3 lightPosition,
            glm::vec3 lightColour
        )
        {
            shader->use();
            shader->setMat4("u_Model", model);
            shader->setMat4("u_View", view);
            shader->setMat4("u_Projection", projection);
            shader->setFloat("u_AmbientLighting", ambientLighting);
            shader->setVec3("u_LightPosition", lightPosition);
            shader->setVec3("u_LightColour", lightColour);
            // render the cube
            glBindVertexArray(terrainVAO);
            for(int strip = 0; strip < numStrips; strip++)
            {
                glDrawElements(
                    GL_TRIANGLE_STRIP,   // primitive type
                    numTrisPerStrip+2,   // number of indices to render
                    GL_UNSIGNED_INT,     // index data type
                    (void*)(sizeof(unsigned) * (numTrisPerStrip+2) * strip)
                ); // offset to starting index
            }
            glBindVertexArray(0);
        }

        void Surface::init()
        {
            createSurface();
        }

        void Surface::createSurface()
        {
            
            unsigned char* image;
            if (type == "height_map")
            {
                std::cout << std::endl << "---Creating surface from height map---" << std::endl;
                image = stbi_load(heightMapPath.c_str(), &nCols, &nRows, &nChannels, 0);
            }

            // vertices calculation
            std::vector<float> vertices;
            float yScale = 64.0f / 256.0f, yShift = 16.0f;
            int idx = 0;
            for(int i = 0; i < nRows; i++)
            {
                for(int j = 0; j < nCols; j++)
                {
                    float height;
                    // we don't have the height vector but we extract it from the pixels
                    if (type == "height_map")
                    {
                        int pixelIndex = (i * nCols + j) * nChannels;
                        unsigned char pixelValue = image[pixelIndex];
                        height = pixelValue * yScale - yShift;
                        heights.push_back((float)height);
                    }
                    else
                    {
                        height = heights[idx];
                        idx++;
                    }

                    if (height > maxHeight)
                        maxHeight = height;

                    // vertex
                    vertices.push_back( -nRows/2.0f + i );   // vx
                    vertices.push_back( (float)height );   // vy
                    vertices.push_back( -nCols/2.0f + j );   // vz
                }
            }

            // normals

            // indices
            std::vector<unsigned> indices;
            for(int i = 0; i < nRows - 1; i++)
                for(int j = 0; j < nCols; j++)
                    for(unsigned k = 0; k < 2; k++)
                        indices.push_back(j + nCols * (i + k));

            numStrips = (nRows - 1);
            numTrisPerStrip = (nCols)*2-2;

            // first, configure the cube's VAO (and terrainVBO + terrainIBO)
            unsigned int terrainVBO, terrainIBO;
            glGenVertexArrays(1, &terrainVAO);
            glBindVertexArray(terrainVAO);

            glGenBuffers(1, &terrainVBO);
            glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glGenBuffers(1, &terrainIBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

        }

        glm::vec3 Surface::interpolateRGB(const glm::vec3 color1, const glm::vec3 color2, float t)
        {
            // Ensure t is within the range [0, 1]
            t = std::fmin(1.0f, std::fmax(0.0f, t));

            glm::vec3 result;
            result.r = (color1.r + t * (color2.r - color1.r));
            result.g = (color1.g + t * (color2.g - color1.g));
            result.b = (color1.b + t * (color2.b - color1.b));

            return result;
        }

    }
}
