
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Object.h"
#include "../Core.h"

namespace Crocodile
{
    namespace s3d
    {

        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
        };

        class CROCODILE_API Mesh : public Object
        {
            public:
                Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
                ~Mesh();

                void render(
                    glm::mat4 model,
                    glm::mat4 view,
                    glm::mat4 projection,
                    glm::vec3 cameraPosition,
                    float ambientLighting
                );

            private:
                std::vector<Vertex> m_vertices;
                std::vector<unsigned int> m_indices;
                
                void createMesh();

                GLuint m_VAO, m_VBO, m_EBO;
        };
    }
}

