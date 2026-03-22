#include "error.h"

#include <iostream>
#include <string>

namespace
{
inline std::string GLErrorCodeParse(GLenum code)
{
#define GL_CODE_CASE(error)                                                                                            \
    case error :                                                                                                       \
        return #error

    switch (code)
    {
        GL_CODE_CASE(GL_INVALID_ENUM);
        GL_CODE_CASE(GL_INVALID_VALUE);
        GL_CODE_CASE(GL_INVALID_OPERATION);
        GL_CODE_CASE(GL_STACK_OVERFLOW);
        GL_CODE_CASE(GL_STACK_UNDERFLOW);
        GL_CODE_CASE(GL_OUT_OF_MEMORY);
        GL_CODE_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
        default :
            return "GL_UNKNOWN_ERROR";
    }

#undef GL_CODE_CASE
}
} // namespace

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogError(const char *func_name, const char *file, int line)
{
    while (GLenum code = glGetError())
    {
        std::cerr << "[" << GLErrorCodeParse(code) << "] " << file << " " << func_name << " -> (" << line
                  << "): " << code << std::endl;
        return false;
    }
    return true;
}
