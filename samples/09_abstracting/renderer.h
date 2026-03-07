#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArrary& va, const IndexBuffer& ib, const Shader& shader);
};

#endif // !__RENDERER_H__