#ifndef SHARP_LIBSHARP_GLOBAL_H_
#define SHARP_LIBSHARP_GLOBAL_H_

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif // HAVE_CONFIG_H

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define SHARP_HELPER_IMPORT __declspec(dllimport)
  #define SHARP_HELPER_EXPORT __declspec(dllexport)
  #define SHARP_HELPER_LOCAL
#else
  #if __GNUC__ >= 4
    #define SHARP_HELPER_IMPORT __attribute__ ((visibility ("default")))
    #define SHARP_HELPER_EXPORT __attribute__ ((visibility ("default")))
    #define SHARP_HELPER_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define SHARP_HELPER_IMPORT
    #define SHARP_HELPER_EXPORT
    #define SHARP_HELPER_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define SHARP_API.
// SHARP_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
#ifdef SHARP_EXPORTS // defined if we are building the SHARP DLL (instead of using it)
  #define SHARP_API SHARP_HELPER_EXPORT
#else
  #define SHARP_API SHARP_HELPER_IMPORT
#endif // SHARP_EXPORTS
#define SHARP_LOCAL SHARP_HELPER_LOCAL

#endif // SHARP_LIBSHARP_GLOBAL_H_

