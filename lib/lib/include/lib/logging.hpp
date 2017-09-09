/*
  Logging
  --
  Logging things.

  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef LOGGING_INCLUDED_55713C67_54CB_4844_B789_EC11EFA4F9E6
#define LOGGING_INCLUDED_55713C67_54CB_4844_B789_EC11EFA4F9E6


#include <stdint.h>
#include <cstring> // needed for 'LOGGING_FILE_NAME' macro
#include <stdarg.h>


// ------------------------------------------------------- [ Logging Config ] --


// Not impl yet.
#ifndef LIB_NS_NAME
#define LIB_NS_NAME lib
#endif


// ------------------------------------------------------- [ Logging Macros ] --



//#define LOG_NO_TODO
//#define LOG_NO_WARNING
//#define LOG_NO_ERROR
//#define LOG_NO_FATAL
//#define LOG_NO_DEPRECATED

#define LOGGING_FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if defined __APPLE__ || __linux__
  #define LOGGING_FUNC_STR __PRETTY_FUNCTION__
#else
  #define LOGGING_FUNC_STR __FUNCTION__
#endif


#define LOG_NONE(prefix)
#define LOG_ONE(prefix, msg) LIB_NS_NAME::logging::log(prefix, LOGGING_FILE_NAME, LOGGING_FUNC_STR, __LINE__, msg)
#define LOG_V(prefix, msg, ...) LIB_NS_NAME::logging::log(prefix, LOGGING_FILE_NAME, LOGGING_FUNC_STR, __LINE__, msg, __VA_ARGS__)

#define LOG_GET(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg12, arg13, arg14, arg15, arg16, ...) arg16

#define LOG_ARGS(...) LOG_GET(__VA_ARGS__, LOG_V, LOG_V, LOG_V, LOG_V, LOG_V, LOG_V, LOG_V, LOG_V, LOG_V, LOG_V, LOG_V, LOG_V, LOG_ONE, LOG_NONE, )
#define LOGGER(...) LOG_ARGS(__VA_ARGS__)(__VA_ARGS__)


#ifndef LOG_NO_TODO
#define LOG_TODO(...)      LOGGER("[todo]", __VA_ARGS__);
#define LOG_TODO_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; LOG_TODO(__VA_ARGS__); } };
#else
#define LOG_TODO(...)
#define LOG_TODO_ONCE(...)
#endif

#ifndef LOG_NO_INFO
#define LOG_INFO(...)      LOGGER("[info]", __VA_ARGS__);
#define LOG_INFO_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; LOG_INFO(__VA_ARGS__); } };
#else
#define LOG_INFO(...)
#define LOG_INFO_ONCE(...)
#endif

#ifndef LOG_NO_WARNING
#define LOG_WARNING(...)      LOGGER("[warn]", __VA_ARGS__);
#define LOG_WARNING_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; LOG_WARNING(__VA_ARGS__); } };
#else
#define LOG_WARNING(...)
#define LOG_WARNING_ONCE(...)
#endif

#ifndef LOG_NO_ERROR
#define LOG_ERROR(...)      LOGGER("[err]", __VA_ARGS__);
#define LOG_ERROR_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; LOG_ERROR(__VA_ARGS__); } };
#else
#define LOG_ERROR(...)
#define LOG_ERROR_ONCE(...)
#endif

#ifndef LOG_NO_FATAL
#define LOG_FATAL(...)      LOGGER("[fatal]", __VA_ARGS__);
#define LOG_FATAL_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; LOG_FATAL(__VA_ARGS__); } };
#else
#define LOG_FATAL(...)
#define LOG_FATAL_ONCE(...)
#endif

#ifndef LOG_NO_DEPRECATED
#define LOG_DEPRECATED(...)      LOGGER("[depr]", __VA_ARGS__);
#define LOG_DEPRECATED_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; LOG_DEPRECATED(__VA_ARGS__); } };
#else
#define LOG_DEPRECATED(...)
#define LOG_DEPRECATED_ONCE(...)
#endif


namespace LIB_NS_NAME {
namespace logging {


namespace out {
enum ENUM {

  console = 1 << 0,
  file    = 1 << 1,

};
}


void
set_output(const uint32_t out);


void
get_output();


void
log(const char *prefix,
    const char *file,
    const char *func,
    const uint32_t line,
    const char *msg,
    ...);



void
log_v(const char *prefix,
    const char *file,
    const char *func,
    const uint32_t line,
    const char *msg,
    va_list args);


} // ns
} // ns


#endif // inc guard


#ifdef LIB_LOGGING_IMPL


#ifndef LIB_IMPL_INCLUDED_2B1B9F18_0082_4CA6_8411_01B99B7A6959
#define LIB_IMPL_INCLUDED_2B1B9F18_0082_4CA6_8411_01B99B7A6959


#include "directory.hpp"
#include "platform.hpp"
#include "file.hpp"
#include <cstdio>
#include <cstring>


namespace
{
  uint32_t logging_outputs = LIB_NS_NAME::logging::out::console;
  char filepath[LIB_MAX_FILE_PATH_SIZE];
}


namespace LIB_NS_NAME {
namespace logging {


void
set_output(const uint32_t output)
{
  logging_outputs = output;

  if(logging_outputs & LIB_NS_NAME::logging::out::file)
  {
    memset(filepath, 0, sizeof(filepath));

    strcat(filepath, dir::exe_path());
    strcat(filepath, "logging");

    #ifndef NDEBUG
    strcat(filepath, "-debug");
    #else
    strcat(filepath, "-release");
    #endif

    strcat(filepath, ".txt");

    FILE *log_file = fopen(filepath, "wb");

    if(log_file)
    {
      char log[] = "Logging\n--\n\n";

      fwrite(log, sizeof(char), sizeof(log), log_file);
      fclose(log_file);
    }
  }
}


void
log(const char *prefix,
    const char *file,
    const char *func,
    const uint32_t line,
    const char *msg,
    ...)
{
  va_list args;
  va_start(args, msg);
  log_v(prefix, file, func, line, msg, args);
  va_end(args);
}


void
log_v(const char *prefix,
    const char *file,
    const char *func,
    const uint32_t line,
    const char *msg,
    va_list args)
{
  if(logging_outputs & LIB_NS_NAME::logging::out::console)
  {
    printf("\n-\n");

    if(prefix)        { printf("%s ", prefix); }
    if(file && line)  { printf("%s : %s : %d ", file, func, line); }

    if((prefix || (file || func)) && msg)
    {
      printf("\n");
    }

    if(msg) { vprintf(msg, args); }

    if(prefix || (file && line) || msg)
    {
      printf("\n-\n");
    }
  }

  if(logging_outputs & LIB_NS_NAME::logging::out::file)
  {
    constexpr size_t sizeof_buffer = 2048;
    size_t buffer_used = 5; // start at 5 to account for trailing characters.

    char buffer[sizeof_buffer];
    memset(buffer, 0, sizeof(buffer));

    if(prefix)
    {
      buffer_used += strlen(prefix) + 1;

      if(buffer_used < sizeof_buffer)
      {
        strcat(buffer, prefix);
        strcat(buffer, "\n");
      }
    }

    if(msg)
    {
      buffer_used += strlen(msg) + 1;

      if(buffer_used < sizeof_buffer)
      {
        strcat(buffer, msg);
        strcat(buffer, "\n");
      }
    }

    strcat(buffer, "-\n");

    FILE *log_file = fopen(filepath, "a+");


    if(log_file)
    {
      fwrite((void*)buffer, sizeof(char), sizeof(char) * strlen(buffer), log_file);
      fclose(log_file);
    }
  }
}


} // ns
} // ns


#endif // impl guard
#endif // impl inc
