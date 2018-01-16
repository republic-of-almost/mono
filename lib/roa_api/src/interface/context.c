#include <interface/context.h>

struct roa_context*
roa_ctx()
{
  static struct roa_context ctx = {
    0,  /* optio ctx */
    0,  /* glfw window */
  };

  return &ctx;
}