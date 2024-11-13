#include "Model.h"

namespace Crocodile
{
    namespace s3d
    {
        Model::Model(const std::string& filePath)
        {
            loadModel(filePath);
        };

        Model::~Model()
        {

        }

        void Model::render(
            graphics::Shader* shader,
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 cameraPosition,
            float ambientLighting
        )
        {
            for (Mesh& mesh : m_meshes)
                mesh.render(shader, model, view, projection, cameraPosition, ambientLighting);
        }

        void Model::loadModel(const std::string& filePath)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
                return;
            }

            // retrieve the directory path of the filepath
            m_directory = filePath.substr(0, filePath.find_last_of('/'));

            // process ASSIMP's root node recursively
            processNode(scene->mRootNode, scene);
        }

        void Model::processNode(aiNode* node, const aiScene* scene)
        {
            // process all the node's meshes (if any)
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                m_meshes.push_back(processMesh(mesh, scene));
            }
        }

        Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            // walk through each of the mesh's vertices
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;
                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.normal = vector;
                }
                // texture coordinates
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x; 
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.texCoords = vec;
                    // tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.tangent = vector;
                    // bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.bitangent = vector;
                }
                else
                    vertex.texCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);        
            }
            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            // // 1. diffuse maps
            // vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // // 2. specular maps
            // vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // // 3. normal maps
            // std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // // 4. height maps
            // std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            // textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
            
            // return a mesh object created from the extracted mesh data
            return Mesh(vertices, indices, {});
        }
    }   
}
