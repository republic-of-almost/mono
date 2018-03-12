/*
  Log

  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef ROA_LOG_INCLUDED_55713C67_54CB_4844_B789_EC11EFA4F9E6
#define ROA_LOG_INCLUDED_55713C67_54CB_4844_B789_EC11EFA4F9E6


#include <stdint.h>
#include <string.h>
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


// ------------------------------------------------------- [ Logging Config ] --



// ------------------------------------------------------- [ Logging Macros ] --


#define ROA_LOGGING_FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if defined __APPLE__ || __linux__
#define ROA_LOGGING_FUNC_STR __PRETTY_FUNCTION__
#else
#define ROA_LOGGING_FUNC_STR __FUNCTION__
#endif


#define ROA_LOG_NONE(prefix)
#define ROA_LOG_ONE(prefix, msg) roa_internal_log(prefix, ROA_LOGGING_FILE_NAME, ROA_LOGGING_FUNC_STR, __LINE__, msg)
#define ROA_LOG_V(prefix, msg, ...) roa_internal_log(prefix, ROA_LOGGING_FILE_NAME, ROA_LOGGING_FUNC_STR, __LINE__, msg, __VA_ARGS__)

#define ROA_LOG_GET(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg12, arg13, arg14, arg15, arg16, ...) arg16

#define ROA_LOG_ARGS(...) ROA_LOG_GET(__VA_ARGS__, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_V, ROA_LOG_ONE, ROA_LOG_NONE, )
#define ROA_LOGGER(...) ROA_LOG_ARGS(__VA_ARGS__)(__VA_ARGS__)


#ifndef ROA_LOG_NO_TODO
#define ROA_LOG_TODO(...)      ROA_LOGGER("[todo]", __VA_ARGS__);
#define ROA_LOG_TODO_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; ROA_LOG_TODO(__VA_ARGS__); } };
#else
#define ROA_LOG_TODO(...)
#define ROA_LOG_TODO_ONCE(...)
#endif

#ifndef ROA_LOG_NO_INFO
#define ROA_LOG_INFO(...)      ROA_LOGGER("[info]", __VA_ARGS__);
#define ROA_LOG_INFO_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; ROA_LOG_INFO(__VA_ARGS__); } };
#else
#define ROA_LOG_INFO(...)
#define ROA_LOG_INFO_ONCE(...)
#endif

#ifndef ROA_LOG_NO_WARNING
#define ROA_LOG_WARNING(...)      ROA_LOGGER("[warn]", __VA_ARGS__);
#define ROA_LOG_WARNING_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; ROA_LOG_WARNING(__VA_ARGS__); } };
#else
#define ROA_LOG_WARNING(...)
#define ROA_LOG_WARNING_ONCE(...)
#endif

#ifndef ROA_LOG_NO_ERROR
#define ROA_LOG_ERROR(...)      ROA_LOGGER("[err]", __VA_ARGS__);
#define ROA_LOG_ERROR_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; ROA_LOG_ERROR(__VA_ARGS__); } };
#else
#define ROA_LOG_ERROR(...)
#define ROA_LOG_ERROR_ONCE(...)
#endif

#ifndef ROA_LOG_NO_FATAL
#define ROA_LOG_FATAL(...)      ROA_LOGGER("[fatal]", __VA_ARGS__);
#define ROA_LOG_FATAL_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; ROA_LOG_FATAL(__VA_ARGS__); } };
#else
#define ROA_LOG_FATAL(...)
#define ROA_LOG_FATAL_ONCE(...)
#endif

#ifndef ROA_LOG_NO_DEPRECATED
#define ROA_LOG_DEPRECATED(...)      ROA_LOGGER("[depr]", __VA_ARGS__);
#define ROA_LOG_DEPRECATED_ONCE(...) { static int err_once = 0; if(!err_once) { err_once = 1; ROA_LOG_DEPRECATED(__VA_ARGS__); } };
#else
#define ROA_LOG_DEPRECATED(...)
#define ROA_LOG_DEPRECATED_ONCE(...)
#endif


typedef enum _roa_log_output {

  ROA_LOG_OUTPUT_CONSOLE = 1 << 0,
  ROA_LOG_OUTPUT_FILE = 1 << 1,

} roa_log_output;


void
roa_logging_set_output(const uint32_t out);


void
roa_internal_log(const char *prefix,
  const char *file,
  const char *func,
  const uint32_t line,
  const char *msg,
  ...);


void
roa_internal_log_v(const char *prefix,
  const char *file,
  const char *func,
  const uint32_t line,
  const char *msg,
  va_list args);



#ifdef __cplusplus
} /* extern */
#endif


#endif // inc guard
