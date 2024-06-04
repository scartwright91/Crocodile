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
            compile();
        }

        Shader::~Shader()
        {
            glDeleteProgram(m_id);
        }

        void Shader::use() const
        {
            glUseProgram(m_id);
        }

        void Shader::reload()
        {
            glDeleteProgram(m_id);
            compile();
            // we need to reset the texture uniforms
            use();
            for (int i = 0; i < m_textures.size(); i++)
                setInt(m_textures[i], i);
        }

        void Shader::compile()
        {
            std::string vertexCode = readShaderSource(m_vertexPath);
            std::string fragmentCode = readShaderSource(m_fragmentPath);

            vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
            fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

            m_id = glCreateProgram();
            glAttachShader(m_id, vertexShader);
            glAttachShader(m_id, fragmentShader);
            glLinkProgram(m_id);

            GLint isLinked = 0;
            glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);
            if (isLinked == GL_FALSE) {
                GLint maxLength = 0;
                glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

                std::string errorLog(maxLength, ' ');
                glGetProgramInfoLog(m_id, maxLength, &maxLength, &errorLog[0]);

                glDeleteProgram(m_id);
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                LOG(ERROR, "Program linking error: " + errorLog);
            }

            glDetachShader(m_id, vertexShader);
            glDetachShader(m_id, fragmentShader);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        GLuint Shader::compileShader(const std::string& source, GLenum shaderType) {
            GLuint shader = glCreateShader(shaderType);
            const char* src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);
            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                std::string errorLog(maxLength, ' ');
                glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
                glDeleteShader(shader);
                LOG(ERROR, "Shader compilation error: " + errorLog);
                return 0;
            }
            return shader;
        }

        std::string Shader::readShaderSource(const std::string& path) {
            std::ifstream file(path);
            if (!file.is_open()) {
                LOG(ERROR, "Failed to open shader file: " + path);
                return "";
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        // setting functions

        // we need to store texture names incase we hot reload the shader
        void Shader::setTexture(const std::string& name)
        {
            setInt(name, (int)m_textures.size());
            m_textures.push_back(std::string(name));
        }

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

    }
}
