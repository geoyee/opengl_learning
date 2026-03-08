#pragma once

#ifndef __ERROR_H__
#define __ERROR_H__

#include <GL/glew.h>

#define ASSERT(x)                                                                                                      \
    if (!(x))                                                                                                          \
    {                                                                                                                  \
        __debugbreak();                                                                                                \
    }

#define GLCALL(func)                                                                                                   \
    GLClearError();                                                                                                    \
    func;                                                                                                              \
    ASSERT(GLLogError(#func, __FILE__, __LINE__))

void GLClearError();
bool GLLogError(const char *func_name, const char *file, int line);

#endif // !__ERROR_H__