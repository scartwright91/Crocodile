#pragma once

#include <map>
#include <thread>

#include "Shader.h"
#include "Crocodile/utils/Logger.h"

namespace Crocodile
{
	namespace graphics
	{

		class ShaderManager
		{

		public:
			ShaderManager()
			{
			}

			void addShader(std::string name, const char* vertexPath, const char* fragmentPath)
			{
				Shader* shader = new Shader(vertexPath, fragmentPath);
				LOG(INFO, "Compiled " + name + " shader");
				m_shaders[name] = shader;
				m_pathsToShaders[vertexPath] = name;
				m_pathsToShaders[fragmentPath] = name;
			}

			Shader* getShader(std::string name)
			{
				if (m_shaders.find(name) == m_shaders.end())
				{
					LOG(WARNING, "Shader not found in shader manager: " + name);
					return nullptr;
				}
				return m_shaders[name];
			}

			std::vector<std::string> getShaderNames() const
			{
				std::vector<std::string> names;
				for (const auto& pair : m_shaders)
					names.push_back(pair.first);
				return names;
			}

			void reloadShader(std::string path)
			{
				std::string name = m_pathsToShaders[path];
				m_shaders[name]->reload();
			}

		private:
			std::map<std::string, Shader*> m_shaders = {};
			std::map<std::string, std::string> m_pathsToShaders = {};

		};

	}
}

