#include "mesh.h"

#include <iostream>

static std::unordered_map<std::string, std::weak_ptr<Texture>> g_texMap{};

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<std::pair<std::string, std::string>>& textures)
    : m_vao(new VertexArrary())
    , m_vbo(new VertexBuffer(vertices.data(), static_cast<unsigned int>(vertices.size() * sizeof(Vertex))))
    , m_ebo(new IndexBuffer(indices.data(), static_cast<unsigned int>(indices.size())))
    , m_texs({})
{
    std::cout << "Creating mesh: " << vertices.size() << " vertices, " << indices.size() << " indices, "
              << textures.size() << " textures" << std::endl;
    setupMesh(textures);
}

Mesh::~Mesh()
{
    delete m_vao;
    m_vao = nullptr;
    delete m_vbo;
    m_vbo = nullptr;
    delete m_ebo;
    m_ebo = nullptr;
}

void Mesh::setupMesh(const std::vector<std::pair<std::string, std::string>>& textures)
{
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(3);
    layout.push<float>(2);
    m_vao->addBuffer(*m_vbo, layout);
    m_vao->unbind();

    size_t textureNum = textures.size();
    for (unsigned int i = 0; i < textureNum; ++i)
    {
        if (g_texMap.find(textures[i].second) != g_texMap.end())
        {
            auto tex = g_texMap[textures[i].second].lock();
            m_texs.emplace(textures[i].first, tex);
        }
        else
        {
            auto tex = std::make_shared<Texture>(textures[i].second);
            if (tex->isValid())
            {
                tex->bind(i);
                m_texs.emplace(textures[i].first, tex);
                g_texMap[textures[i].second] = tex;
            }
            else
            {
                std::cerr << "ERROR::MESH::Failed to load texture: " << textures[i].second << std::endl;
            }
        }
    }
}

void Mesh::draw(Renderer& renderer, Shader& shader, bool use_tex)
{
    if (use_tex)
    {
        unsigned int i = 0;
        for (auto& [name, texture] : m_texs)
        {
            if (texture)
            {
                shader.setUniform1i(name, i);
                texture->bind(i);
                ++i;
            }
        }
    }

    renderer.draw(*m_vao, *m_ebo, shader);
}