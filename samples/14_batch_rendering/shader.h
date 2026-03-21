#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <vector>

using ShaderFiles = std::vector<std::pair<unsigned int, std::string>>;

class Shader
{
public:
    Shader(const ShaderFiles& file_path_and_types);
    ~Shader();

    void bind() const;
    void unbind() const;

    /* Further abstraction can be extended */
    void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    void setUniform1i(const std::string& name, int f0);
    void setUniformMat4f(const std::string& name, const glm::mat4& f0);

private:
    static unsigned int CompilerShader(unsigned int type, const std::string& source);
    static unsigned int CreateShader(const ShaderFiles& file_path_and_types);

    int getUniformLocation(const std::string& name);
    std::string getShaderFilePath(unsigned int type);

    unsigned int m_readerID;
    ShaderFiles m_files;
    std::unordered_map<std::string, int> m_locationCache;
};

#endif // !__SHADER_H__