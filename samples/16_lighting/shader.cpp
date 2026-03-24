#include "shader.h"
#include "error.h"

#include <GL/glew.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
std::string ParseShader(const std::string& file)
{
    std::ifstream fs(file);
    std::ostringstream oss;
    if (!fs.is_open())
    {
        return "";
    }
    oss << fs.rdbuf();
    return oss.str();
}
} // namespace

Shader::Shader(const ShaderFiles& file_path_and_types) : m_files(file_path_and_types), m_readerID(0)
{
    m_readerID = CreateShader(file_path_and_types);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_readerID));
}

void Shader::bind() const
{
    GLCALL(glUseProgram(m_readerID));
}

void Shader::unbind() const
{
    GLCALL(glUseProgram(0));
}

// Shader.cpp
void Shader::setUniform1f(const std::string& name, float v0)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform1f(location, v0));
    }
}

void Shader::setUniform1i(const std::string& name, int v0)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform1i(location, v0));
    }
}

void Shader::setUniform1ui(const std::string& name, unsigned int v0)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform1ui(location, v0));
    }
}

void Shader::setUniform2f(const std::string& name, float v0, float v1)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform2f(location, v0, v1));
    }
}

void Shader::setUniform2f(const std::string& name, const glm::vec2& v)
{
    setUniform2f(name, v.x, v.y);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform3f(location, v0, v1, v2));
    }
}

void Shader::setUniform3f(const std::string& name, const glm::vec3& v)
{
    setUniform3f(name, v.x, v.y, v.z);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform4f(location, v0, v1, v2, v3));
    }
}

void Shader::setUniform4f(const std::string& name, const glm::vec4& v)
{
    setUniform4f(name, v.x, v.y, v.z, v.w);
}

void Shader::setUniform2i(const std::string& name, int v0, int v1)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform2i(location, v0, v1));
    }
}

void Shader::setUniform2i(const std::string& name, const glm::ivec2& v)
{
    setUniform2i(name, v.x, v.y);
}

void Shader::setUniform3i(const std::string& name, int v0, int v1, int v2)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform3i(location, v0, v1, v2));
    }
}

void Shader::setUniform3i(const std::string& name, const glm::ivec3& v)
{
    setUniform3i(name, v.x, v.y, v.z);
}

void Shader::setUniform4i(const std::string& name, int v0, int v1, int v2, int v3)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform4i(location, v0, v1, v2, v3));
    }
}

void Shader::setUniform4i(const std::string& name, const glm::ivec4& v)
{
    setUniform4i(name, v.x, v.y, v.z, v.w);
}

void Shader::setUniform2ui(const std::string& name, unsigned int v0, unsigned int v1)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform2ui(location, v0, v1));
    }
}

void Shader::setUniform2ui(const std::string& name, const glm::uvec2& v)
{
    setUniform2ui(name, v.x, v.y);
}

void Shader::setUniform3ui(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform3ui(location, v0, v1, v2));
    }
}

void Shader::setUniform3ui(const std::string& name, const glm::uvec3& v)
{
    setUniform3ui(name, v.x, v.y, v.z);
}

void Shader::setUniform4ui(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform4ui(location, v0, v1, v2, v3));
    }
}

void Shader::setUniform4ui(const std::string& name, const glm::uvec4& v)
{
    setUniform4ui(name, v.x, v.y, v.z, v.w);
}

void Shader::setUniformMat2f(const std::string& name, const glm::mat2& mat)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]));
    }
}

void Shader::setUniformMat3f(const std::string& name, const glm::mat3& mat)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]));
    }
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& mat)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]));
    }
}

void Shader::setUniform1fv(const std::string& name, unsigned int count, const float *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform1fv(location, count, values));
    }
}

void Shader::setUniform2fv(const std::string& name, unsigned int count, const float *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform2fv(location, count, values));
    }
}

void Shader::setUniform3fv(const std::string& name, unsigned int count, const float *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform3fv(location, count, values));
    }
}

void Shader::setUniform4fv(const std::string& name, unsigned int count, const float *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform4fv(location, count, values));
    }
}

void Shader::setUniform1iv(const std::string& name, unsigned int count, const int *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform1iv(location, count, values));
    }
}

void Shader::setUniform2iv(const std::string& name, unsigned int count, const int *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform2iv(location, count, values));
    }
}

void Shader::setUniform3iv(const std::string& name, unsigned int count, const int *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform3iv(location, count, values));
    }
}

void Shader::setUniform4iv(const std::string& name, unsigned int count, const int *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform4iv(location, count, values));
    }
}

void Shader::setUniform1uiv(const std::string& name, unsigned int count, const unsigned int *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform1uiv(location, count, values));
    }
}

void Shader::setUniform2uiv(const std::string& name, unsigned int count, const unsigned int *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform2uiv(location, count, values));
    }
}

void Shader::setUniform3uiv(const std::string& name, unsigned int count, const unsigned int *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform3uiv(location, count, values));
    }
}

void Shader::setUniform4uiv(const std::string& name, unsigned int count, const unsigned int *values)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform4uiv(location, count, values));
    }
}

unsigned int Shader::CompilerShader(unsigned int type, const std::string& source)
{
    GLCALL(unsigned int id = glCreateShader(type));
    const char *src = source.data();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));
    /* Error handing */
    int res;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &res));
    if (res == GL_FALSE)
    {
        int len;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
        std::vector<char> msg(len);
        GLCALL(glGetShaderInfoLog(id, len, &len, msg.data()));
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader: " << reinterpret_cast<char const *>(msg.data()) << std::endl;
        GLCALL(glDeleteShader(id));
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const ShaderFiles& file_path_and_types)
{
    GLCALL(unsigned int program = glCreateProgram());
    std::vector<unsigned int> shaders;
    shaders.reserve(file_path_and_types.size());
    for (const auto& ft : file_path_and_types)
    {
        unsigned int tmp = CompilerShader(ft.first, ParseShader(ft.second));
        GLCALL(glAttachShader(program, tmp));
        shaders.push_back(tmp);
    }
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));
    /* Generally not deleted, for ease of debugging and suchlike */
    for (auto& shader : shaders)
    {
        GLCALL(glDeleteShader(shader));
    }
    return program;
}

int Shader::getUniformLocation(const std::string& name)
{
    if (m_locationCache.find(name) != m_locationCache.end())
    {
        return m_locationCache[name];
    }
    GLCALL(int location = glGetUniformLocation(m_readerID, name.c_str()));
    if (location == -1)
    {
        std::cerr << "invalid uniform named " << name << std::endl;
        return -1;
    }
    m_locationCache[name] = location;
    return location;
}

std::string Shader::getShaderFilePath(unsigned int type)
{
    const auto& it =
        std::find_if(m_files.cbegin(), m_files.cend(), [type](const auto& item) { return item.first == type; });
    if (it != m_files.cend())
    {
        return it->second;
    }
    return "";
}