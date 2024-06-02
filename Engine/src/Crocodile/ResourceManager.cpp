#include "ResourceManager.h"

#include "Crocodile/utils/Logger.h"

namespace Crocodile
{

	ResourceManager::ResourceManager()
	{

	}

	std::vector<std::string> ResourceManager::getTextureNames()
	{
		std::vector<std::string> textureNames = {};
		for (std::map<std::string, TextureData>::iterator it = textureIDs.begin(); it != textureIDs.end(); ++it)
			textureNames.push_back(it->first);
		return textureNames;
	}

	void ResourceManager::loadTexture(const char* path, std::string name, bool repeat)
	{
		TextureData textureID = loadTextureFromFile(path, name, repeat);
		textureIDs[name] = textureID;
	}

	void ResourceManager::addTexture(unsigned int texture, unsigned int width, unsigned int height, std::string name)
	{
		TextureData td;
		td.textureID = texture;
		td.path = "";
		td.name = name;
		td.width = (float)width;
		td.height = (float)height;
		textureIDs[name] = td;
	}

	TextureData ResourceManager::getTexture(std::string name)
	{
		return textureIDs[name];
	}

	bool ResourceManager::textureExists(std::string name) const
	{
		return textureIDs.count(name);
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
		animations[name] = animImageNames;
	}

	std::vector<TextureData> ResourceManager::getAnimationData(std::string name)
	{
		std::vector<TextureData> animation = {};
		for (std::string animImageName : animations[name])
			animation.push_back(getTexture(animImageName));
		return animation;
	}

    void ResourceManager::listenToFileChanges()
    {

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
            if (pixelArt)
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

}
