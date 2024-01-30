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
            glm::mat4 projection
        )
        {
            shader->use();
            shader->setMat4("u_Model", model);
            shader->setMat4("u_View", view);
            shader->setMat4("u_Projection", projection);
            // render the cube
            glBindVertexArray(terrainVAO);
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
            if (type == "data")
                createSurfaceFromData();
            else if (type == "height_map")
                createSurfaceFromHeightMap();
        }

        void Surface::createSurfaceFromHeightMap()
        {

            std::cout << std::endl << "---Creating surface from height map---" << std::endl;

            int width, height, channels;
            unsigned char* image = stbi_load(heightMapPath.c_str(), &width, &height, &channels, 0);

            std::vector<float> vertices;
            float yScale = 64.0f / 256.0f, yShift = 16.0f;
            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    int pixelIndex = (i * width + j) * channels;
                    unsigned char pixelValue = image[pixelIndex];

                    float hh = pixelValue * yScale - yShift;
                    // vertex
                    vertices.push_back( -height/2.0f + height*i/(float)height );   // vx
                    vertices.push_back( (float)hh );   // vy
                    vertices.push_back( -width/2.0f + width*j/(float)width );   // vz
                }
            }

            stbi_image_free(image);

            std::vector<unsigned> indices;
            for(int i = 0; i < height - 1; i++)
                for(int j = 0; j < width; j++)
                    for(unsigned k = 0; k < 2; k++)
                        indices.push_back(j + width * (i + k));
            std::cout << "Loaded " << indices.size() << " indices" << std::endl;

            numStrips = (height - 1);
            numTrisPerStrip = (width)*2-2;

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

        void Surface::createSurfaceFromData()
        {

            std::vector<float> vertices;
            int idx = 0;
            for(int i = 0; i < nCols; i++)
            {
                for(int j = 0; j < nRows; j++)
                {
                    float h = heights[idx];
                    // position
                    vertices.push_back( -nCols/2.0f + nCols*i/(float)nCols );   // vx
                    vertices.push_back( h );   // vy
                    vertices.push_back( -nRows/2.0f + nRows*j/(float)nRows );   // vz
                    // colour
                    glm::vec3 col = interpolateRGB(glm::vec3(1.f), glm::vec3(0.f), h);
                    vertices.push_back(col.r);
                    vertices.push_back(col.g);
                    vertices.push_back(col.b);
                    idx++;
                }
            }

            std::vector<unsigned> indices;
            for(int i = 0; i < nCols-1; i++)
                for(int j = 0; j < nRows; j++)
                    for(int k = 0; k < 2; k++)
                        indices.push_back(j + (int)nRows * (i + k));

            numStrips = nCols - 1;
            numTrisPerStrip = nRows * 2 - 2;

            // first, configure the cube's VAO (and terrainVBO + terrainIBO)
            unsigned int terrainVBO, terrainIBO;
            glGenVertexArrays(1, &terrainVAO);
            glBindVertexArray(terrainVAO);

            glGenBuffers(1, &terrainVBO);
            glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
            glEnableVertexAttribArray(1);

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
