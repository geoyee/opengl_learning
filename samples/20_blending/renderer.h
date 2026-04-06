#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"

class Renderer
{
public:
    Renderer(bool depth = true, bool stencil = true, bool blend = true);

    bool depth() const;
    void depth(bool enable);
    bool stencil() const;
    void stencil(bool enable);
    bool blend() const;
    void blend(bool enable);
    void clear() const;
    void draw(const VertexArrary& va, unsigned int size, const Shader& shader);
    void draw(const VertexArrary& va, const IndexBuffer& ib, const Shader& shader);

private:
    bool m_enable_depth;
    bool m_enable_stencil;
    bool m_enable_blend;
};

#endif // !__RENDERER_H__