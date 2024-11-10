#include "MeshLoader.h"

namespace Crocodile
{
    namespace s3d
    {

        Mesh MeshLoader::loadMesh(const std::string& filePath)
        {
            const aiScene* scene = m_importer.ReadFile(
                filePath, 
                aiProcess_Triangulate | 
                aiProcess_FlipUVs | 
                aiProcess_GenNormals);
            
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cerr << "Error loading model: " << m_importer.GetErrorString() << std::endl;
                return Mesh({}, {}); // Return an empty mesh if loading fails
            }

            // Only take the first mesh found in the file
            aiMesh* mesh = scene->mMeshes[0]; 

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            // Process vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                Vertex vertex;
                vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

                // Check if the mesh has texture coordinates
                if (mesh->mTextureCoords[0]) {
                    vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                } else {
                    vertex.texCoords = glm::vec2(0.0f, 0.0f);
                }

                vertices.push_back(vertex);
            }

            // Process indices
            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j]);
                }
            }

            return Mesh(vertices, indices);

        }

    }
}
