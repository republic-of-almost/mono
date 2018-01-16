/*
  platform.h
  --

  Determins various platform properties that might be desired.
*/
#ifndef PLATFORM_INCLUDED_0DB1DB2D_0560_4F8D_9690_3918C0686CC7
#define PLATFORM_INCLUDED_0DB1DB2D_0560_4F8D_9690_3918C0686CC7


/* ---------------------------------------------------- [ Possible flags ] -- */
/*
  Below are the possible flags that might get defined.
*/

/* compiler flags */
/*
  ROA_COMPILER_IS_MSVS
  ROA_COMPILER_IS_CLANG
  ROA_COMPILER_IS_GCC
  ROA_COMPILER_IS_KNOWN
*/

/* os flags */
/*
  ROA_OS_IS_WINDOWS
  ROA_OS_IS_MACOS
  ROA_OS_IS_APPLE
  ROA_OS_IS_LINUX
  ROA_OS_IS_NIX
  ROA_OS_IS_KNOWN
*/

/* arch flags */
/*
  ROA_ARCH_IS_X86_32
  ROA_ARCH_IS_X86_64
  ROA_ARCH_IS_ARM
  ROA_ARCH_IS_KNOWN
*/

/* compiler flags */
#ifdef __clang__
#define ROA_COMPILER_IS_CLANG
#define ROA_COMPILER_IS_KNOWN
#elif defined _MSC_VER
#define ROA_COMPILER_IS_MSVS
#define ROA_COMPILER_IS_KNOWN
#elif defined GNU
#define ROA_COMPILER_IS_GCC
#define ROA_COMPILER_IS_KNOWN
#endif

/* os flags */
#ifdef _APPLE_
#define ROA_OS_IS_MACOS
#define ROA_OS_IS_APPLE
#define ROA_OS_IS_NIX
#define ROA_OS_IS_KNOWN
#elif defined _WIN32
#define ROA_OS_IS_WINDOWS
#define ROA_OS_IS_KNOWN
#elif defined __linux__
#define ROA_OS_IS_LINUX
#define ROA_OS_IS_NIX
#define ROA_OS_IS_KNOWN
#endif

/* arch flags */
#ifdef ROA_COMPILER_IS_CLANG

#elif defined ROA_COMPILER_IS_MSVS

#elif defined ROA_COMPILER_IS_GCC

#endif


#endif /* inc guard */
