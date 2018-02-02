#include <roa_lib/log.h>
#include <stdio.h>

void
roa_log(Log_level level, const char *msg)
{
  printf("%s\n", msg);
}