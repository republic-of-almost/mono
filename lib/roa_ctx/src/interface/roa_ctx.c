#include <roa_ctx/roa_ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/log.h>
#include <roa_lib/fundamental.h>
#include <GLFW/glfw3.h>


struct roa_ctx
{
  GLFWwindow *window;
  int width;
  int height;
  ROA_BOOL is_fullscreen;
};


static void
roa_ctx_err_callback(int error, const char *desc)
{
  ROA_UNUSED(error);

  ROA_LOG_ERROR("[glfw] %s", desc);
}


static void
roa_ctx_win_size_callback(GLFWwindow *win, int width, int height)
{
  struct roa_ctx *ctx = (struct roa_ctx*)glfwGetWindowUserPointer(win);

  ctx->width = width;
  ctx->height = height;
}


/* --------------------------------------------------------- [ Lifetime ] -- */


void
roa_ctx_create(roa_ctx_t *ctx)
{
  ROA_ASSERT(ctx);

  if (glfwInit())
  {
    glfwSetErrorCallback(roa_ctx_err_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *win = glfwCreateWindow(800, 480, "ROA Application", NULL, NULL);

    if (win)
    {
      struct roa_ctx *new_ctx = NULL;
      new_ctx = roa_zalloc(sizeof(*new_ctx));
      new_ctx->window = win;

      glfwMakeContextCurrent(win);
      glfwSwapInterval(GLFW_TRUE);

      glfwSetWindowUserPointer(win, new_ctx);

      glfwGetWindowSize(win, &new_ctx->width, &new_ctx->height);
      glfwSetWindowSizeCallback(win, roa_ctx_win_size_callback);

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
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT_PEDANTIC(ctx->window);
  ROA_ASSERT(out_desc);

  static const char* title = "Not supported yet";

  out_desc->width = ctx->width;
  out_desc->height = ctx->height;
  out_desc->title = title;
}


void
roa_ctx_set_window_desc(
  roa_ctx_t ctx,
  const struct roa_ctx_window_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT_PEDANTIC(ctx->window);
  ROA_ASSERT(desc);

  glfwSetWindowSize(ctx->window, desc->width, desc->height);
  glfwSetWindowTitle(ctx->window, desc->title);
}