#if _WIN32
#ifdef BUILD_PYVM_SHARED
#define PYVM_API decllspec(dllexport)
#else
#define PYVM_API decllspec(dllimport)
#endif
#else
#define PYVM_API extern
#endif
