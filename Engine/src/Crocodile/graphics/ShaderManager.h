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
			ShaderManager() {};

			void addShader(std::string name, const char* vertexPath, const char* fragmentPath)
			{
				Shader* shader = new Shader(vertexPath, fragmentPath);
				LOG(INFO, "Compiled " + name + " shader");
				m_shaders[name] = shader;
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

			void reloadShaders() const
			{

			}

			void listenToFileChanges()
			{

			}

		private:
			std::map<std::string, Shader*> m_shaders = {};

		};

	}
}

