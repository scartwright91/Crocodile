#include "Mesh.h"

namespace Crocodile
{
    namespace s3d
    {
        Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : m_vertices(vertices), m_indices(indices)
        {
            createMesh();
        }

        Mesh::~Mesh()
        {

        }

        void Mesh::render(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 cameraPosition,
            float ambientLighting
        )
        {

        }

        void Mesh::createMesh()
        {
            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);
            glGenBuffers(1, &m_EBO);

            glBindVertexArray(m_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

            // Position attribute
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // Normal attribute
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            // Texture Coord attribute
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

            glBindVertexArray(0);
        }
    }
}