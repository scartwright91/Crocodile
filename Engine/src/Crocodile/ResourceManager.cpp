#include "ResourceManager.h"

#include "Crocodile/utils/Logger.h"

namespace Crocodile
{

	ResourceManager::ResourceManager()
	{

	}

    ResourceManager::~ResourceManager()
    {
        for (DirectoryWatcher* dirWatcher : m_dirWatchers)
            delete dirWatcher;
        m_dirWatchers.clear();
    }

    void ResourceManager::update()
    {
        if (m_shaderReloadQueue.size() > 0)
        {
            for (std::string shaderPath : m_shaderReloadQueue)
                m_shaderManager.reloadShader(shaderPath);
            m_shaderReloadQueue.clear();
        }
        if (m_textureReloadQueue.size() > 0)
        {
            for (std::string texturePath : m_textureReloadQueue)
                reloadTexture(texturePath);
            m_textureReloadQueue.clear();
        }
    }

	std::vector<std::string> ResourceManager::getTextureNames()
	{
		std::vector<std::string> textureNames = {};
		for (std::map<std::string, TextureData>::iterator it = m_textureIDs.begin(); it != m_textureIDs.end(); ++it)
			textureNames.push_back(it->first);
		return textureNames;
	}

	void ResourceManager::loadTexture(const char* path, std::string name, bool repeat)
	{
		TextureData textureID = loadTextureFromFile(path, name, repeat);
		m_textureIDs[name] = textureID;
	}

	void ResourceManager::addTexture(unsigned int texture, unsigned int width, unsigned int height, std::string name)
	{
		TextureData td;
		td.textureID = texture;
		td.path = "";
		td.name = name;
		td.width = (float)width;
		td.height = (float)height;
		m_textureIDs[name] = td;
	}

	TextureData ResourceManager::getTexture(std::string name)
	{
		return m_textureIDs[name];
	}

	bool ResourceManager::textureExists(std::string name) const
	{
		return m_textureIDs.count(name);
	}

	void ResourceManager::loadAnimation(const char* dir, std::string name)
	{
		std::vector<std::string> animImageNames = {};
		for (const auto& entry : fs::directory_iterator(dir))
		{
			std::string filePath = entry.path().string();
			std::string fileName = entry.path().filename().string();
			loadTexture(filePath.c_str(), fileName, false);
			animImageNames.push_back(fileName);
		}
		m_animations[name] = animImageNames;
	}

	std::vector<TextureData> ResourceManager::getAnimationData(std::string name)
	{
		std::vector<TextureData> animation = {};
		for (std::string animImageName : m_animations[name])
			animation.push_back(getTexture(animImageName));
		return animation;
	}

    void ResourceManager::addDirWatcher(std::string path, AssetType assetType)
    {
        DirectoryWatcher* dirWatcher = new DirectoryWatcher("res/shaders", std::chrono::milliseconds(2000));
        dirWatcher->start([&](const std::string& path)
        {
            std::string normalisedPath = path;
            std::replace(normalisedPath.begin(), normalisedPath.end(), '\\', '/');
            if (assetType == SHADER)
            {
                LOG(INFO, "Shader modified: " + normalisedPath);
                m_shaderReloadQueue.push_back(normalisedPath);
            }
            else if (assetType == TEXTURE)
            {
                LOG(INFO, "Texture modified: " + normalisedPath);
                m_textureReloadQueue.push_back(normalisedPath);
            }
        });
    }

    void ResourceManager::reloadTexture(std::string path)
    {
        for (const auto& pair : m_textureIDs) {
            if (path == pair.second.path)
            {
                m_textureIDs[pair.first] = loadTextureFromFile(
                    path.c_str(),
                    pair.second.name,
                    pair.second.repeat
                );
                break;
            }
        }
    }

    TextureData ResourceManager::loadTextureFromFile(char const* path, std::string name, bool repeat) const
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

        if (data)
        {

            GLenum format = 0;
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
            if (m_pixelArt)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }

            stbi_image_free(data);
        }
        else
        {
            LOG(ERROR, "Texture failed to load from path: " + std::string(path));
            stbi_image_free(data);
        }

        TextureData td;
        td.name = name;
        td.path = path;
        td.textureID = textureID;
        td.width = (float)width;
        td.height = (float)height;

        return td;
    }

    void ResourceManager::addLuaBindings(sol::state &lua)
    {
        lua.new_usertype<TextureData>(
            "texture_data",
            sol::no_constructor,
            "name", &TextureData::name,
            "path", &TextureData::path,
            "texture", &TextureData::textureID,
            "width", &TextureData::width,
            "height", &TextureData::height,
            "repeat", &TextureData::repeat
        );
        lua.new_usertype<ResourceManager>(
            "resource_manager",
            sol::no_constructor,
            "load_texture", &ResourceManager::loadTexture,
            "load_animation", &ResourceManager::loadAnimation,
            "get_texture", &ResourceManager::getTexture
        );
    }

}
