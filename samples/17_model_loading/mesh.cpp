#include "mesh.h"
#include "texture.h"

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<std::pair<std::string, std::string>>& textures)
    : m_vbo(vertices.data(), vertices.size())
    , m_ebo(indices.data(), indices.size())
    , m_texs({})
{
    setupMesh(textures);
}

void Mesh::setupMesh(const std::vector<std::pair<std::string, std::string>>& textures)
{
    VertexBufferLayout layout;
    layout.push<float>(3); // XYZ
    layout.push<float>(3); // Normal
    layout.push<float>(2); // UV
    m_vao.addBuffer(m_vbo, layout);
    m_vao.unbind();

    size_t textureNum = textures.size();
    for (unsigned int i = 0; i < textureNum; ++i)
    {
        m_texs[textures[i].first] = std::move(Texture(textures[i].second));
        m_texs[textures[i].first].bind(i);
    }
}

void Mesh::draw(Renderer& renderer, Shader& shader)
{
    unsigned int i = 0;
    for (auto& [name, texture] : m_texs)
    {
        shader.setUniform1i(name, i);
        m_texs[name].bind(i);
    }

    renderer.draw(m_vao, m_ebo, shader);
}