#pragma once

#include <any>
#include <glad/glad.h>
#include <iostream>
#include <type_traits>

namespace gltk {

inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

#define GL_GETERROR_STRING(error) ""


inline bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << ":" << GL_GETERROR_STRING(error) << ")"
                  << ": " << function << " " << file << ":" << line
                  << std::endl;
        return false;
    }
    return true;
}


// inline void warnning()
//{
// #pragma message("Warning: Using GL_CALL with glEnd()")
// }

} // namespace gltk

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
#define GL_CALL(x)                                         \
    do {                                                   \
        gltk::GLClearError();                              \
        x;                                                 \
        __ASSERT(gltk::GLLogCall(#x, __FILE__, __LINE__)); \
    } while (0)
#endif

#ifdef NDEBUG
#define GL_CHECK_HEALTH()
#else
#define GL_CHECK_HEALTH()                                        \
    do {                                                         \
        __ASSERT(gltk::GLLogCall(__func__, __FILE__, __LINE__)); \
    } while (0)
#endif
