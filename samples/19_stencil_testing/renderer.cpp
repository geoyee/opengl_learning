#include "renderer.h"
#include "error.h"

Renderer::Renderer(bool depth, bool stencil) : m_enable_depth(depth), m_enable_stencil(stencil)
{
    if (depth)
    {
        GLCALL(glEnable(GL_DEPTH_TEST));
    }
    if (stencil)
    {
        GLCALL(glEnable(GL_STENCIL_TEST));
    }
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

bool Renderer::depth() const
{
    return m_enable_depth;
}

void Renderer::depth(bool enable)
{
    if (enable)
    {
        GLCALL(glEnable(GL_DEPTH_TEST));
    }
    else
    {
        GLCALL(glDisable(GL_DEPTH_TEST));
    }
    m_enable_depth = enable;
}

bool Renderer::stencil() const
{
    return m_enable_stencil;
}

void Renderer::stencil(bool enable)
{
    if (enable)
    {
        GLCALL(glEnable(GL_STENCIL_TEST));
    }
    else
    {
        GLCALL(glDisable(GL_STENCIL_TEST));
    }
    m_enable_stencil = enable;
}

void Renderer::clear() const
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    auto clear_type = GL_COLOR_BUFFER_BIT;
    if (m_enable_depth)
    {
        clear_type |= GL_DEPTH_BUFFER_BIT;
    }
    if (m_enable_stencil)
    {
        clear_type |= GL_STENCIL_BUFFER_BIT;
    }
    GLCALL(glClear(clear_type));
}

void Renderer::draw(const VertexArrary& va, unsigned int size, const Shader& shader)
{
    va.bind();
    shader.bind();
    GLCALL(glDrawArrays(GL_TRIANGLES, 0, size));
}

void Renderer::draw(const VertexArrary& va, const IndexBuffer& ib, const Shader& shader)
{
    va.bind();
    ib.bind();
    shader.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}