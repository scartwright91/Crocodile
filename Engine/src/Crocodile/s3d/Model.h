#pragma once

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

// #include "Object.h"
// #include "Mesh.h"
// #include "../Core.h"
// #include "../Graphics/Shader.h"

// namespace Crocodile
// {
//     namespace s3d
//     {
//         class CROCODILE_API Model : public Object
//         {
//             public:
//                 Model(const std::string& filePath);
//                 ~Model();
//                 void render(
//                     graphics::Shader* shader,
//                     glm::mat4 model,
//                     glm::mat4 view,
//                     glm::mat4 projection,
//                     glm::vec3 cameraPosition,
//                     float ambientLighting
//                 );

//             private:
//                 std::vector<Mesh> m_meshes;
//                 std::string m_directory;

//                 void loadModel(const std::string& filePath);
//                 void processNode(aiNode* node, const aiScene* scene);
//                 Mesh processMesh(aiMesh* mesh, const aiScene* scene);
//         };
//     }
// }