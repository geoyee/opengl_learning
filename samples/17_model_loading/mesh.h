#pragma once

#ifndef __MESH_H__
#define __MESH_H__

#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

#include <glm/glm.hpp>

#include <map>
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
    void draw(Renderer& renderer, Shader& shader);

private:
    void setupMesh(const std::vector<std::pair<std::string, std::string>>& textures);

    VertexArrary m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ebo;
    std::map<std::string, Texture> m_texs;
};

#endif // !__MESH_H__