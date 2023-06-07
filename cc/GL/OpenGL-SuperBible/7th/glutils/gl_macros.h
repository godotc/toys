#pragma once

#include <any>
#include <glad/glad.h>
#include <iostream>
#include <type_traits>

namespace unknow {

inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

inline bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << ")"
                  << ": " << function << " " << file << ":" << line
                  << std::endl;
        return false;
    }
    return true;
}

} // namespace unknow

#if defined(_WIN32)
#define PLATFORM_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
#define PLATFORM_BREAK() __builtin_trap()
#else
#define PLATFORM_BREAK()
#endif

#ifndef NDEBUG
#define __ASSERT(x) \
    if (!(x))       \
    PLATFORM_BREAK()
#else
#define __ASSERT(x) (x)
#endif

#ifdef NDEBUG
#define GL_CALL(x) x
#else
#define GL_CALL(x)                                           \
    do {                                                     \
        unknow::GLClearError();                              \
        x;                                                   \
        __ASSERT(unknow::GLLogCall(#x, __FILE__, __LINE__)); \
    } while (0)
#endif
