#include <common/callbacks.h>
#include <stdio.h>

void
volt_logging_callback(const char *msg)
{
  printf("VOLT: %s", msg);
}