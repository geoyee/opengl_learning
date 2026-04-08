#include "vertex_buffer.h"
#include "error.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    GLCALL(glGenBuffers(1, &m_renderID));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(unsigned int size)
{
    GLCALL(glGenBuffers(1, &m_renderID));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW)); // Dynamic
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_renderID));
}

void VertexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
}

void VertexBuffer::bind(const void *data, unsigned int size) const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
    GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data)); // Dynamic
}

void VertexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}