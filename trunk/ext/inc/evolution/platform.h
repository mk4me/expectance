
#if !defined(EVOLUTION_STATIC) && defined(_WIN32) && !defined(__MNGW32__) && !defined(__CYGWIN__)

#ifndef EVOLUTION_API
#ifdef EVOLUTION_EXPORTS
#define EVOLUTION_API __declspec(dllexport)
#else
#define EVOLUTION_API __declspec(dllimport)
#   pragma warning (disable : 4251)
#endif
#endif

#else

#define EVOLUTION_API

#endif
