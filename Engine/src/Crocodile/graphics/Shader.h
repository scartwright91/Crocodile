#pragma once

#ifdef CROCODILE_EMSCRIPTEN
#define GLFW_INCLUDE_ES3
#endif
#include <GL/glew.h>

#include <glm/glm.hpp>																										
#include <glm/gtc/type_ptr.hpp>																																	
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "../Core.h"

namespace Crocodile
{
	namespace graphics
	{
		class CROCODILE_API Shader
		{
		public:
			Shader();
			Shader(const char *vertexPath, const char *fragmentPath);
			~Shader();

			void use() const;
			void reload();

			// set uniforms
			void setTexture(const std::string& name);
			void setBool(const std::string &name, bool value) const;
			void setInt(const std::string &name, int value) const;
			void setFloat(const std::string &name, float value) const;
			void setMat4(const std::string &name, glm::mat4 value) const;
			void setVec2(const std::string &name, glm::vec2 value) const;
			void setVec3(const std::string &name, glm::vec3 value) const;
			void setVec4(const std::string &name, glm::vec4 value) const;
			// arrays
			void setArrayFloat(const std::string &name, std::vector<float> value) const;
			void setArrayInt(const std::string &name, std::vector<int> value) const;
			void setArrayVec2(const std::string &name, std::vector<glm::vec2> value) const;

		private:
			void compile();
			GLuint compileShader(const std::string& source, GLenum shaderType);
			std::string readShaderSource(const std::string& path);

			GLuint m_id;

		public:
			std::string m_vertexPath, m_fragmentPath;
			std::vector<std::string> m_textures = {};
			GLuint m_vertexShader;
			GLuint m_fragmentShader;

		};
	}
}
