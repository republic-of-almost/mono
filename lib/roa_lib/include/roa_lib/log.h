#ifndef LOG_INCLUDED_3F0264FB_5ABF_4318_B5F8_4CA706C3B919
#define LOG_INCLUDED_3F0264FB_5ABF_4318_B5F8_4CA706C3B919


typedef enum {
  ROA_LOG_INFO,
  ROA_LOG_WARN,
  ROA_LOG_ERR,
} Log_level;


void
roa_log(Log_level level, const char *msg);


#endif /* inc guard */