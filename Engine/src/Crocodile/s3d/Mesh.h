#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "../Core.h"
#include "../graphics/Shader.h"

#define MAX_BONE_INFLUENCE 4

namespace Crocodile
{
    namespace s3d
    {

        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
            glm::vec3 tangent;
            glm::vec3 bitangent;
            int boneIDs[MAX_BONE_INFLUENCE];
            float weights[MAX_BONE_INFLUENCE];
        };

        struct Texture {
            unsigned int id;
            std::string type;
        };  

        class CROCODILE_API Mesh
        {
            public:
                Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
                ~Mesh();

                void render(
                    graphics::Shader* shader,
                    glm::mat4 model,
                    glm::mat4 view,
                    glm::mat4 projection,
                    glm::vec3 cameraPosition,
                    float ambientLighting
                );

            private:
                void createMesh();

            public:
                std::vector<Vertex> m_vertices;
                std::vector<unsigned int> m_indices;
                std::vector<Texture> m_textures;

            private:
                GLuint m_VAO, m_VBO, m_EBO;
        };
    }
}

