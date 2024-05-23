#include "Surface.h"

namespace Crocodile
{
    namespace s3d
    {

        HeightMap::HeightMap()
        {

        };

        HeightMap::HeightMap(std::string heightMapPath, float scale, bool inverse)
        {
            unsigned char* image;
            int nChannels;
            std::cout << std::endl << "Creating surface from height map..." << std::endl;
            std::cout << "Reading data from: " << heightMapPath << std::endl;
            image = stbi_load(heightMapPath.c_str(), &nCols, &nRows, &nChannels, 0);
            std::cout << nCols << ", " << nRows << std::endl;
            for(int i = 0; i < nRows; i++)
            {
                for(int j = 0; j < nCols; j++)
                {
                    int pixelIndex = (i * nCols + j) * nChannels;
                    unsigned char pixelValue = image[pixelIndex];
                    float height = pixelValue / 255.f;
                    if (inverse)
                        height = std::fabs(height - 1.);
                    heights.push_back((float)height * scale);
                }
            }
            std::cout << "Finished creating height map..." << std::endl;
            init();
        }

        HeightMap::HeightMap(int nCols, int nRows, std::vector<float> heights) : nCols(nCols), nRows(nRows), heights(heights)
        {
            init();
        }

        void HeightMap::init()
        {
            // find min and max heights
            auto min = std::min_element(heights.begin(), heights.end());
            if (min != heights.end())
                minHeight = *min;
            auto max = std::max_element(heights.begin(), heights.end());
            if (max != heights.end())
                maxHeight = *max;
        }

        float HeightMap::getHeight(int i, int j)
        {
            return heights[i * nCols + j];
        }

        Surface::~Surface()
        {

        }

        void Surface::render(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 cameraPosition,
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
            shader->setVec3("u_CameraPosition", cameraPosition);
            shader->setVec3("u_SurfaceColour", colour);
            shader->setFloat("u_Alpha", alpha);
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
            
            // vertices calculation
            std::vector<float> vertices;
            for(int i = 0; i < heightMap.nRows; i++)
            {
                for(int j = 0; j < heightMap.nCols; j++)
                {
                    float height = heightMap.heights[i * heightMap.nCols + j];

                    // positions
                    vertices.push_back( pos.x + j * gridSize.x );   // vz
                    vertices.push_back( (float)height );   // vy
                    vertices.push_back( pos.y + i *gridSize.y );   // vx

                    // normals
                    glm::vec3 normal = calculateNormal(i, j);
                    vertices.push_back(normal.x);
                    vertices.push_back(normal.y);
                    vertices.push_back(normal.z);
                }
            }

            // indices
            std::vector<unsigned> indices;
            for(int i = 0; i < heightMap.nRows - 1; i++)
                for(int j = 0; j < heightMap.nCols; j++)
                    for(unsigned k = 0; k < 2; k++)
                        indices.push_back(j + heightMap.nCols * (i + k));

            numStrips = (heightMap.nRows - 1);
            numTrisPerStrip = (heightMap.nCols)*2-2;

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

        glm::vec3 Surface::calculateNormal(int i, int j)
        {
            // extract adjacent vertices
            glm::vec3 l, r, u, d;
            if (i - adjacentVertexDistance < 0)
                l = {i, heightMap.getHeight(i, j), j};
            else
                l = {i - adjacentVertexDistance, heightMap.getHeight(i - adjacentVertexDistance, j), j};
            if (i + adjacentVertexDistance > heightMap.nRows - 1)
                r = {i, heightMap.getHeight(i, j), j};
            else
                r = {i + adjacentVertexDistance, heightMap.getHeight(i + adjacentVertexDistance, j), j};
            if (j - 1 < 0)
                u = {i, heightMap.getHeight(i, j), j};
            else
                u = {i, heightMap.getHeight(i, j - adjacentVertexDistance), j - adjacentVertexDistance};
            if (j + adjacentVertexDistance > heightMap.nCols - 1)
                d = {i, heightMap.getHeight(i, j), j};
            else
                d = {i, heightMap.getHeight(i, j + adjacentVertexDistance), j + adjacentVertexDistance};

            // calculate directional vectors across adjacent vertices
            glm::vec3 u_d = {u.x - d.x, u.y - d.y, u.z - d.z};
            glm::vec3 l_r = {l.x - r.x, l.y - r.y, l.z - r.z};

            // cross-product and normalise to find normal vector
            glm::vec3 normal = glm::normalize(glm::cross(u_d, l_r));

            return normal;
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
