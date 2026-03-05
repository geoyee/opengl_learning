#include "render.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogError(const char *func_name, const char *file, int line)
{
    while (GLenum code = glGetError())
    {
        std::cerr << "[OpenGL ERROR] " << file << " " << func_name << " -> (" << line << "): " << code << std::endl;
        return false;
    }
    return true;
}
