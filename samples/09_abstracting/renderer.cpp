#include "renderer.h"
#include "error.h"

void Renderer::clear() const
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArrary& va, const IndexBuffer& ib, const Shader& shader)
{
    va.bind();
    ib.bind();
    shader.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}