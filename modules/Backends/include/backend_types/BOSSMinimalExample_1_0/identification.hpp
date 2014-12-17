// Identify backend and set macro flags

#include "cats.hpp"

#ifdef BACKENDRENAME
  #define BACKENDNAME BACKENDRENAME
#else
  #define BACKENDNAME BOSSMinimalExample
#endif

#define VERSION 1.0
#define SAFE_VERSION 1_0

#undef DO_CLASSLOADING
#define DO_CLASSLOADING 1