#pragma once

#ifndef __VERTEX_BUFFER_LAYOUT_HPP__
#define __VERTEX_BUFFER_LAYOUT_HPP__

#include <GL/glew.h>

#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
};

using VertexBufferElements = std::vector<VertexBufferElement>;

class VertexBufferLayout
{
public:
    VertexBufferLayout() : m_stride(0) { }

    template<typename T>
    void push(unsigned int count)
    {
        static_assert(false);
    }

    const VertexBufferElements& getElements() const
    {
        return m_elements;
    }

    unsigned int getStride() const
    {
        return m_stride;
    }

private:
    VertexBufferElements m_elements;
    unsigned int m_stride;
};

template<>
inline void VertexBufferLayout::push<float>(unsigned int count)
{
    m_elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_stride += count * sizeof(GLfloat);
}

template<>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += count * sizeof(GLuint);
}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_stride += count * sizeof(GLubyte);
}

#endif // !__VERTEX_BUFFER_LAYOUT_HPP__