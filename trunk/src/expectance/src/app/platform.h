
#if defined(EXPECTANCE_TO_DLL) && defined(_WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)

#ifndef EXPECTANCE_API
#ifdef EXPECTANCE_EXPORTS
#define EXPECTANCE_API __declspec(dllexport)
#else
#define EXPECTANCE_API __declspec(dllimport)
#endif
#endif

#else

#define EXPECTANCE_API

#endif

