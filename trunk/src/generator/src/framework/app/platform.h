
#if defined(GENERATOR_TO_DLL) && defined(_WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)

#ifndef GENERATOR_API
#ifdef GENERATOR_EXPORTS
#define GENERATOR_API __declspec(dllexport)
#else
#define GENERATOR_API __declspec(dllimport)
#endif
#endif

#else

#define GENERATOR_API

#endif
