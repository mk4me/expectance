
#if defined(ENGINE_TO_DLL) && defined(_WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)

#ifndef ENGINE_API
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#endif

#else

#define ENGINE_API

#endif
