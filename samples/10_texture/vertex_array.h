
#pragma once

#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#include "vertex_buffer.h"
#include "vertex_buffer_layout.hpp"

class VertexArrary
{
public:
    VertexArrary();
    ~VertexArrary();

    void bind() const;
    void unbind() const;
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

private:
    unsigned int m_renderID;
};

#endif // !__VERTEX_ARRAY_H__