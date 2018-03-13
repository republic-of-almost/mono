#include <roa_ctx/roa_ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/log.h>
#include <roa_lib/fundamental.h>
#include <GLFW/glfw3.h>


struct roa_ctx
{
  GLFWwindow *window;

	/*
	 * we manually track width / height because querying after set will not give
	 * correct results until events are processed.
	 */
	int width;
	int height;
	char title[256];
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

		/* some defaults */
		int width = 800;
		int height = 450;
		const char *title = "ROA Application";

    GLFWwindow *win = glfwCreateWindow(
			width,
			height,
			title,
			ROA_NULL,
			ROA_NULL);

    if (win)
    {
      struct roa_ctx *new_ctx = ROA_NULL;
      new_ctx = roa_zalloc(sizeof(*new_ctx));
      new_ctx->window = win;

      glfwMakeContextCurrent(win);
      glfwSwapInterval(GLFW_TRUE);

      glfwSetWindowUserPointer(win, new_ctx);

      glfwSetWindowSizeCallback(win, roa_ctx_win_size_callback);
			
			new_ctx->width = width;
			new_ctx->height = height;
			
			int bytes = strlen(title) + 1 > sizeof(new_ctx->title) - 1 ? sizeof(new_ctx->title) - 1 : strlen(title);
			memcpy(new_ctx->title, title, bytes);
			
      *ctx = new_ctx;

			ROA_LOG_INFO("Created Context");
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
  ROA_ASSERT(ctx->window);
  ROA_ASSERT(out_desc);

	out_desc->height = ctx->height;
  out_desc->width = ctx->width;
	out_desc->title = ctx->title;
}


void
roa_ctx_set_window_desc(
  roa_ctx_t ctx,
  const struct roa_ctx_window_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(ctx->window);
  ROA_ASSERT(desc);
	
	ctx->width = desc->width;
	ctx->height = desc->height;

	unsigned bytes = strlen(desc->title) + 1 > sizeof(ctx->title) - 1 ? sizeof(ctx->title) - 1 : strlen(desc->title) + 1;
	memcpy(ctx->title, desc->title, bytes);

  glfwSetWindowSize(ctx->window, desc->width, desc->height);
  glfwSetWindowTitle(ctx->window, desc->title);
}
