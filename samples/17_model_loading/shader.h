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
    void setUniform1f(const std::string& name, float v0);
    void setUniform1i(const std::string& name, int v0);
    void setUniform1ui(const std::string& name, unsigned int v0);
    void setUniform2f(const std::string& name, float v0, float v1);
    void setUniform2f(const std::string& name, const glm::vec2& v);
    void setUniform3f(const std::string& name, float v0, float v1, float v2);
    void setUniform3f(const std::string& name, const glm::vec3& v);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform4f(const std::string& name, const glm::vec4& v);
    void setUniform2i(const std::string& name, int v0, int v1);
    void setUniform2i(const std::string& name, const glm::ivec2& v);
    void setUniform3i(const std::string& name, int v0, int v1, int v2);
    void setUniform3i(const std::string& name, const glm::ivec3& v);
    void setUniform4i(const std::string& name, int v0, int v1, int v2, int v3);
    void setUniform4i(const std::string& name, const glm::ivec4& v);
    void setUniform2ui(const std::string& name, unsigned int v0, unsigned int v1);
    void setUniform2ui(const std::string& name, const glm::uvec2& v);
    void setUniform3ui(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2);
    void setUniform3ui(const std::string& name, const glm::uvec3& v);
    void setUniform4ui(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
    void setUniform4ui(const std::string& name, const glm::uvec4& v);
    void setUniformMat2f(const std::string& name, const glm::mat2& mat);
    void setUniformMat3f(const std::string& name, const glm::mat3& mat);
    void setUniformMat4f(const std::string& name, const glm::mat4& mat);
    void setUniform1fv(const std::string& name, unsigned int count, const float *values);
    void setUniform2fv(const std::string& name, unsigned int count, const float *values);
    void setUniform3fv(const std::string& name, unsigned int count, const float *values);
    void setUniform4fv(const std::string& name, unsigned int count, const float *values);
    void setUniform1iv(const std::string& name, unsigned int count, const int *values);
    void setUniform2iv(const std::string& name, unsigned int count, const int *values);
    void setUniform3iv(const std::string& name, unsigned int count, const int *values);
    void setUniform4iv(const std::string& name, unsigned int count, const int *values);
    void setUniform1uiv(const std::string& name, unsigned int count, const unsigned int *values);
    void setUniform2uiv(const std::string& name, unsigned int count, const unsigned int *values);
    void setUniform3uiv(const std::string& name, unsigned int count, const unsigned int *values);
    void setUniform4uiv(const std::string& name, unsigned int count, const unsigned int *values);

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