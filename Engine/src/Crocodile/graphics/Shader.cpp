#include "Shader.h"

#include "Crocodile/utils/Logger.h"

namespace Crocodile
{
    namespace graphics
    {

        Shader::Shader() : m_id(0) {}

        Shader::Shader(const char* vertexPath, const char* fragmentPath) :
            m_vertexPath(vertexPath), m_fragmentPath(fragmentPath)
        {
            load();
        }

        Shader::~Shader()
        {
            glDeleteProgram(m_id);
        }

        void Shader::use() const
        {
            glUseProgram(m_id);
        }

        void Shader::load()
        {
            //glDeleteProgram(m_id);
            // 1. retrieve the vertex/fragment source code from filePath
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                // open files
                vShaderFile.open(m_vertexPath);
                fShaderFile.open(m_fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                // close file handlers
                vShaderFile.close();
                fShaderFile.close();
                // convert stream into string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure& e)
            {
                LOG(ERROR, "Shader not successfully read");
            }

            init(vertexCode, fragmentCode);

        }

        // setting functions
        void Shader::setBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
        }

        void Shader::setInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
        }

        void Shader::setFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
        }

        void Shader::setMat4(const std::string &name, glm::mat4 value) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
        }

        void Shader::setVec2(const std::string &name, glm::vec2 value) const
        {
            glUniform2f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y);
        }

        void Shader::setVec3(const std::string &name, glm::vec3 value) const
        {
            glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
        }

        void Shader::setVec4(const std::string &name, glm::vec4 value) const
        {
            glUniform4f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z, value.w);
        }

        void Shader::setArrayFloat(const std::string &name, std::vector<float> value) const
        {
            glUniform1fv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), (float *)value.data());
        }

        void Shader::setArrayInt(const std::string &name, std::vector<int> value) const
        {
            glUniform1iv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), value.data());
        }

        void Shader::setArrayVec2(const std::string &name, std::vector<glm::vec2> value) const
        {
            glUniform2fv(glGetUniformLocation(m_id, name.c_str()), (GLsizei)value.size(), (float *)value.data());
        }

        void Shader::init(std::string vertexCode, std::string fragmentCode)
        {
            const char *vShaderCode = vertexCode.c_str();
            const char *fShaderCode = fragmentCode.c_str();
            // 2. compile shaders
            unsigned int vertex, fragment;
            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");
            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");
            // shader Program
            m_id = glCreateProgram();
            glAttachShader(m_id, vertex);
            glAttachShader(m_id, fragment);
            glLinkProgram(m_id);
            checkCompileErrors(m_id, "PROGRAM");
            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void Shader::checkCompileErrors(unsigned int shader, std::string type)
        {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    LOG(ERROR, "Shader did not compile: " + type);
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    LOG(ERROR, "Shader did not link: " + type);
                }
            }
        }
    }
}
