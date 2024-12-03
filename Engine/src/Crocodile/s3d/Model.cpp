#include "Model.h"
#include "../utils/TextureReader.h"

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
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 cameraPosition,
            glm::vec3 lightPosition,
            glm::vec3 lightColour,
            float ambientLighting
        )
        {
            for (Mesh& mesh : m_meshes)
                mesh.render(
                    m_shader,
                    model,
                    view,
                    projection,
                    cameraPosition,
                    lightPosition,
                    lightColour,
                    ambientLighting
                );
        }

        void Model::setShader(graphics::Shader* shader)
        {
            m_shader = shader;
        }

        void Model::loadModel(const std::string& filePath)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                LOG(LoggerLevel::WARNING, "Error loading model: " + std::string(importer.GetErrorString()));
                return;
            }

            // Debug: List all textures in all materials
            for (unsigned int matIdx = 0; matIdx < scene->mNumMaterials; matIdx++) {
                aiMaterial* material = scene->mMaterials[matIdx];
                LOG(LoggerLevel::INFO, "Material " + std::to_string(matIdx) + ":");
                
                for (aiTextureType type = aiTextureType_NONE; type <= aiTextureType_UNKNOWN; type = static_cast<aiTextureType>(type + 1))
                {
                    unsigned int textureCount = material->GetTextureCount(type);
                    if (textureCount > 0)
                    {
                        LOG(LoggerLevel::INFO, "  Found " + std::to_string(textureCount) + " textures of type " + std::to_string(type));
                        for (unsigned int i = 0; i < textureCount; i++)
                        {
                            aiString path;
                            material->GetTexture(type, i, &path);
                            LOG(LoggerLevel::INFO, "    Texture " + std::to_string(i) + ": " + path.C_Str());
                        }
                    }
                }
            }

            // retrieve the directory path of the filepath
            m_directory = filePath.substr(0, filePath.find_last_of('/'));

            // process ASSIMP's root node recursively
            processNode(scene->mRootNode, scene);

        }

        void Model::processNode(aiNode* node, const aiScene* scene)
        {
            LOG(LoggerLevel::INFO, "Node name: " + std::string(node->mName.C_Str()));
            // process all the node's meshes (if any)
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                m_meshes.push_back(processMesh(mesh, scene));
            }
            // then do the same for each of its children
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
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
                vertices.push_back(vertex);
                // texture coordinates
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x; 
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.texCoords = vec;
                }
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);        
            }
            
            LOG(LoggerLevel::INFO, std::to_string(mesh->mMaterialIndex));

            // process material
            if (mesh->mMaterialIndex >= 0)
            {
                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                
                // Load diffuse textures
                std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                
                // Load specular textures
                std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
                
                // Load normal maps
                std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
                textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
                
                // Load height maps
                std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
                textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
                
                // Load ambient occlusion maps
                std::vector<Texture> aoMaps = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
                textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
            }

            // LOG(LoggerLevel::INFO, "N textures: " + std::to_string(textures.size()));

            // return a mesh object created from the extracted mesh data
            return Mesh(vertices, indices, textures);
        }

        std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) const
        {
            std::vector<Texture> textures;
            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                Texture texture;
                texture.id = utils::TextureFromFile(str.C_Str(), m_directory, false);
                texture.type = typeName;
                textures.push_back(texture);
            }
            return textures;
        }
    }   
}
