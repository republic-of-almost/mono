#include <roa_lib/log.h>
#include <roa_lib/fundamental.h>
#include <stdio.h>

void
roa_log(Log_level level, const char *msg)
{
  ROA_UNUSED(level);

  printf("%s\n", msg);
}