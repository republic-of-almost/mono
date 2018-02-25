#include <roa_ctx/roa_ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/log.h>
#include <roa_lib/fundamental.h>
#include <GLFW/glfw3.h>


struct roa_ctx
{
  GLFWwindow *window;
};


void
roa_ctx_err_callback(int error, const char *desc)
{
  ROA_UNUSED(error);

  roa_log(ROA_LOG_ERR, desc);
}


/* --------------------------------------------------------- [ Lifetime ] -- */


void
roa_ctx_create(roa_ctx_t *ctx)
{
  ROA_ASSERT(ctx);

  if (glfwInit())
  {
    glfwSetErrorCallback(roa_ctx_err_callback);

    GLFWwindow *win = glfwCreateWindow(800, 600, "ROA Application", NULL, NULL);

    if (win)
    {
      struct roa_ctx *new_ctx = NULL;
      new_ctx = roa_zalloc(sizeof(*new_ctx));
      new_ctx->window = win;

      glfwMakeContextCurrent(win);

      *ctx = new_ctx;
    }
  }
}


void
roa_ctx_destroy(roa_ctx_t *ctx)
{
  ROA_ASSERT(ctx);
  ROA_ASSERT(*ctx);

  if ((*ctx)->window)
  {
    glfwDestroyWindow((*ctx)->window);

    roa_free(*ctx);
  }

  glfwTerminate();
}


ROA_BOOL
roa_ctx_new_frame(roa_ctx_t ctx)
{
  if (ctx && ctx->window)
  {
    glfwPollEvents();
    glfwSwapBuffers(ctx->window);

    return glfwWindowShouldClose(ctx->window) ? ROA_FALSE : ROA_TRUE;
  }

  return ROA_FALSE;
}


/* ----------------------------------------------------------- [ Window ] -- */


void
roa_ctx_get_window_desc(
  roa_ctx_t ctx,
  struct roa_ctx_window_desc *out_desc)
{
  ROA_UNUSED(ctx);
  ROA_UNUSED(out_desc);
}


void
roa_ctx_set_window_desc(
  roa_ctx_t ctx,
  const struct roa_ctx_window_desc *desc)
{
  ROA_UNUSED(ctx);
  ROA_UNUSED(desc);
}