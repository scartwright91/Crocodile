#include "Surface.h"

namespace Crocodile
{
    namespace s3d
    {

        Surface::Surface(ResourceManager::TextureData heightMap, graphics::Shader* shader) : heightMap(heightMap), type("height_map"), shader(shader)
        {
            init();
        }

        Surface::Surface(std::vector<glm::vec3> vertices, graphics::Shader* shader) : vertices(vertices), type("vertices"), shader(shader)
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
            if (type == "vertices")
                createSurfaceFromVertices();
            else if (type == "height_map")
                createSurfaceFromHeightMap();
        }

        void Surface::createSurfaceFromHeightMap()
        {
            // // ------------------------------------------------------------------
            std::vector<float> vertices;
            float yScale = 64.0f / 256.0f, yShift = 16.0f;
            int rez = 1;
            unsigned bytePerPixel = 1;
            for(int i = 0; i < heightMap.height; i++)
            {
                for(int j = 0; j < heightMap.width; j++)
                {
                    // vertex
                    vertices.push_back( -heightMap.height/2.0f + heightMap.height*i/(float)heightMap.height );   // vx
                    vertices.push_back( (int) 0 * yScale - yShift);   // vy
                    vertices.push_back( -heightMap.width/2.0f + heightMap.width*j/(float)heightMap.width );   // vz
                }
            }
            std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;

            std::vector<unsigned> indices;
            for(unsigned i = 0; i < heightMap.height-1; i += rez)
            {
                for(unsigned j = 0; j < heightMap.width; j += rez)
                {
                    for(unsigned k = 0; k < 2; k++)
                    {
                        indices.push_back(j + (int)heightMap.width * (i + k*rez));
                    }
                }
            }
            std::cout << "Loaded " << indices.size() << " indices" << std::endl;

            numStrips = (int)(heightMap.height - 1) / rez;
            numTrisPerStrip = (int)(heightMap.width/rez) * 2 - 2;
            std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
            std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;

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

        void Surface::createSurfaceFromVertices()
        {
            
        }

    }
}
