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
			unsigned int ID;
			Shader();
			Shader(std::string vertexCode, std::string fragmentCode, bool source);
			Shader(const char *vertexPath, const char *fragmentPath);
			~Shader();

			void use();
			void setBool(const std::string &name, bool value) const;
			void setInt(const std::string &name, int value) const;
			void setFloat(const std::string &name, float value) const;
			void setMat4(const std::string &name, glm::mat4 value);
			void setVec2(const std::string &name, glm::vec2 value);
			void setVec3(const std::string &name, glm::vec3 value);
			void setVec4(const std::string &name, glm::vec4 value);
			// arrays
			void setArrayFloat(const std::string &name, std::vector<float> value);
			void setArrayInt(const std::string &name, std::vector<int> value);
			void setArrayVec2(const std::string &name, std::vector<glm::vec2> value);

		private:
			void init(std::string vertexCode, std::string fragmentCode);
			void checkCompileErrors(unsigned int shader, std::string type);
		};
	}
}
