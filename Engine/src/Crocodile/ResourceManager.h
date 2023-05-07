#pragma once

#include <map>
#include <string>

#include "Core.h"
#include "graphics/Shader.h"
#include "../utils/stb_image.h"

namespace Crocodile
{
    class CROCODILE_API ResourceManager
    {

    public:
        struct TextureData
        {
            unsigned int textureID;
            float width;
            float height;
        };

        // shaders
        std::map<std::string, graphics::Shader> shaders = {};
        // textures
        std::map<std::string, TextureData> textureIDs = {};

        ResourceManager()
        {
        }
        ~ResourceManager()
        {
        }

        void loadShaderFromString(std::string vertexCode, std::string fragmentCode, std::string name)
        {
            graphics::Shader shader(vertexCode, fragmentCode, true);
            std::cout << "Compiled " << name << " shader\n";
            shaders[name] = shader;
        }

        void loadShader(const char *vert_path, const char *frag_path, std::string name)
        {
            graphics::Shader shader(vert_path, frag_path);
            std::cout << "Compiled " << name << " shader\n";
            shaders[name] = shader;
        }

        graphics::Shader *getShader(std::string name)
        {
            if (shaders.find(name) == shaders.end())
            {
                std::cout << "SHADER NOT FOUND IN RESOURCE MANAGER: " << name << std::endl;
                return nullptr;
            }
            return &shaders[name];
        }

        void loadTexture(const char *path, std::string name, bool repeat)
        {
            TextureData textureID = loadTextureFromFile(path, name, repeat);
            textureIDs[name] = textureID;
        }

        TextureData getTexture(std::string name)
        {
            return textureIDs[name];
        }

        bool textureExists(std::string name)
        {
            return textureIDs.count(name);
        }

    private:
        TextureData loadTextureFromFile(char const *path, std::string name, bool repeat)
        {
            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

            if (data)
            {

                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                if (repeat)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                }
                else
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            TextureData td;
            td.textureID = textureID;
            td.width = (float)width;
            td.height = (float)height;

            return td;
        }
    };

}