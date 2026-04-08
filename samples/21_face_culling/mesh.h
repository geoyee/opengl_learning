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
         const std::vector<std::pair<std::string, std::string>>& textures,
         unsigned int start_tex_id = 0);
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;
    ~Mesh();

    void draw(Renderer& renderer, Shader& shader, bool use_tex = true);

private:
    void setupMesh(const std::vector<std::pair<std::string, std::string>>& textures, unsigned int start_id = 0);

    VertexArrary *m_vao;
    VertexBuffer *m_vbo;
    IndexBuffer *m_ebo;
    std::map<std::string, std::shared_ptr<Texture>> m_texs;
};

#endif // !__MESH_H__