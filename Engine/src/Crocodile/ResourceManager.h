#pragma once

#include <map>
#include <string>
#include <filesystem>

#include "Core.h"
#include "graphics/ShaderManager.h"
#include "../utils/stb_image.h"

namespace fs = std::filesystem;

namespace Crocodile
{
    struct TextureData
    {
        std::string name;
        std::string path;
        unsigned int textureID;
        float width;
        float height;
        TextureData() : name(""), path(""), textureID(0), width(0.f), height(0.f) {}
    };


    class CROCODILE_API ResourceManager
    {
    public:

        ResourceManager();

        // shaders
        graphics::ShaderManager shaderManager;
        // textures
        std::map<std::string, TextureData> textureIDs = {};
        // animations
        std::map<std::string, std::vector<std::string>> animations = {};

        // true will use nearest pixel when loading textures; false will use linear
        bool pixelArt = false;

        // textures
        std::vector<std::string> getTextureNames();
        void loadTexture(const char* path, std::string name, bool repeat);
        void addTexture(unsigned int texture, unsigned int width, unsigned int height, std::string name);
        TextureData getTexture(std::string name);
        bool textureExists(std::string name) const;

        // animations
        void loadAnimation(const char* dir, std::string name);
        std::vector<TextureData> getAnimationData(std::string name);

        // hot reloading
        void listenToFileChanges();

    private:
        TextureData loadTextureFromFile(char const* path, std::string name, bool repeat) const;

    };
}