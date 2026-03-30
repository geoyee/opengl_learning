#pragma once

#ifndef __ERROR_H__
#define __ERROR_H__

#include <GL/glew.h>

#ifdef _MSC_VER
#define DEBUG_BREAK() __debugbreak()
#else // !_MSC_VER
#if defined(__GNUC__) || defined(__clang__)
#if defined(__i386__) || defined(__x86_64__)
#define DEBUG_BREAK() __asm__ volatile("int $3")
#elif defined(__arm__) || defined(__aarch64__)
#define DEBUG_BREAK() __asm__ volatile("brk #0")
#else
#define DEBUG_BREAK() __builtin_trap()
#endif // __arm__ || __aarch64__
#else  // !(__GNUC__ || __clang__)
#include <cstdlib>
#define DEBUG_BREAK() std::abort()
#endif // defined(__GNUC__) || defined(__clang__)
#endif // _MSC_VER

#define ASSERT(x)                                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            DEBUG_BREAK();                                                                                             \
        }                                                                                                              \
    } while (0)

#define GLCALL(func)                                                                                                   \
    GLClearError();                                                                                                    \
    func;                                                                                                              \
    ASSERT(GLLogError(#func, __FILE__, __LINE__))

void GLClearError();
bool GLLogError(const char *func_name, const char *file, int line);

#endif // !__ERROR_H__