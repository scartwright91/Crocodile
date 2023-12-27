#include "Surface.h"

namespace Crocodile
{
    namespace s3d
    {

        Surface::Surface(ResourceManager::TextureData heightMap) : heightMap(heightMap), type("height_map")
        {
            init();
        }

        Surface::Surface(std::vector<glm::vec3> vertices) : vertices(vertices), type("vertices")
        {
            init();
        }

        Surface::~Surface()
        {

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
            // // set up vertex data (and buffer(s)) and configure vertex attributes
            // // ------------------------------------------------------------------
            std::vector<float> vertices;
            // float yScale = 64.0f / 256.0f, yShift = 16.0f;
            // int rez = 1;
            // unsigned bytePerPixel = nrChannels;
            // for(int i = 0; i < height; i++)
            // {
            //     for(int j = 0; j < width; j++)
            //     {
            //         unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            //         unsigned char y = pixelOffset[0];

            //         // vertex
            //         vertices.push_back( -height/2.0f + height*i/(float)height );   // vx
            //         vertices.push_back( (int) y * yScale - yShift);   // vy
            //         vertices.push_back( -width/2.0f + width*j/(float)width );   // vz
            //     }
            // }
            // std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
            // stbi_image_free(data);

            std::vector<unsigned> indices;
            // for(unsigned i = 0; i < height-1; i += rez)
            // {
            //     for(unsigned j = 0; j < width; j += rez)
            //     {
            //         for(unsigned k = 0; k < 2; k++)
            //         {
            //             indices.push_back(j + width * (i + k*rez));
            //         }
            //     }
            // }
            // std::cout << "Loaded " << indices.size() << " indices" << std::endl;

            // const int numStrips = (height-1)/rez;
            // const int numTrisPerStrip = (width/rez)*2-2;
            // std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
            // std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;

            // first, configure the cube's VAO (and terrainVBO + terrainIBO)
            unsigned int terrainVAO, terrainVBO, terrainIBO;
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

    }
}
