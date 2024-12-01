#pragma once

#include <map>
#include <string>
#include <filesystem>
#include <vector>
#include <sol/sol.hpp>

#include "Core.h"
#include "graphics/ShaderManager.h"
#include "../utils/stb_image.h"
#include "utils/DirectoryWatcher.h"

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
        bool repeat;
        TextureData() : name(""), path(""), textureID(0), width(0.f), height(0.f), repeat(false) {}
    };

    enum AssetType
    {
        TEXTURE,
        SHADER,
        AUDIO,
        SCRIPT
    };

    class CROCODILE_API ResourceManager
    {
    public:

        ResourceManager();
        ~ResourceManager();

        void update();

        // textures
        std::vector<std::string> getTextureNames();
        void loadTexture(const char* path, std::string name, bool repeat);
        void addTexture(unsigned int texture, unsigned int width, unsigned int height, std::string name);
        TextureData getTexture(std::string name);
        bool textureExists(std::string name) const;
        TextureData loadTextureFromFile(char const* path, std::string name, bool repeat) const;

        // animations
        void loadAnimation(const char* dir, std::string name);
        std::vector<TextureData> getAnimationData(std::string name);

        void addLuaBindings(sol::state &lua);
        
    private:
        void reloadTexture(std::string path);

    public:
        bool m_addNewShader = false;
        // shaders
        graphics::ShaderManager m_shaderManager;
        // true will use nearest pixel when loading textures; false will use linear
        bool m_pixelArt = false;

    private:
        // hot reloading
        std::vector<std::string> m_shaderReloadQueue = {};
        DirectoryWatcher* m_dirWatcher = nullptr;
        // textures
        std::map<std::string, TextureData> m_textureIDs = {};
        // animations
        std::map<std::string, std::vector<std::string>> m_animations = {};

    };
}