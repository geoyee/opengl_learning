#pragma once

#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

class VertexBuffer
{
public:
    VertexBuffer(const void *data, unsigned int size);
    VertexBuffer(unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void bind(const void *data, unsigned int size) const;
    void unbind() const;

private:
    unsigned int m_renderID;
};

#endif // !__VERTEX_BUFFER_H__