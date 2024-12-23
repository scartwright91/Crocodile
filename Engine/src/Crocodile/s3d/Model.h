#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Core.h"
#include "../Graphics/Shader.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API Model
        {
            public:
                Model(const std::string& filePath);
                ~Model();
                void render(
                    glm::mat4 model,
                    glm::mat4 view,
                    glm::mat4 projection,
                    glm::vec3 cameraPosition,
                    glm::vec3 lightPosition,
                    glm::vec3 lightColour,
                    float ambientLighting
                );

                void setShader(graphics::Shader* shader);

            private:
                graphics::Shader* m_shader;
                std::vector<Mesh> m_meshes;
                std::string m_directory;

                void loadModel(const std::string& filePath);
                void processNode(aiNode* node, const aiScene* scene);
                Mesh processMesh(aiMesh* mesh, const aiScene* scene);

                std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) const;

        };
    }
}