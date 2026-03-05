#include "index_buffer.h"
#include "render.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : m_nCount(count)
{
    GLCALL(glGenBuffers(1, &m_renderID));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_renderID));
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
}

void IndexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::getCount() const
{
    return m_nCount;
}