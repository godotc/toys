#pragma once

#ifdef _WIN32
#ifdef BUILD_SHARED_GLTK
#define GLTK_API __declspec(dllexport)
#else
#define GLTK_API __declspec(dllimport)
#endif
#endif
