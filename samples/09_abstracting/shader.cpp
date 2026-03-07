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

void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    int location = getUniformLocation(name);
    if (location != -1)
    {
        GLCALL(glUniform4f(location, f0, f1, f2, f3));
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
    m_locationCache.insert_or_assign(name, location);
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