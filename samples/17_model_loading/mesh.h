#pragma once

#ifndef __MESH_H__
#define __MESH_H__

#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const std::vector<std::pair<std::string, std::string>>& textures);
    ~Mesh() = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    void draw(Renderer& renderer, Shader& shader);

private:
    void setupMesh(const std::vector<std::pair<std::string, std::string>>& textures);

    VertexArrary m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ebo;
    std::map<std::string, std::unique_ptr<Texture>> m_texs;
};

#endif // !__MESH_H__