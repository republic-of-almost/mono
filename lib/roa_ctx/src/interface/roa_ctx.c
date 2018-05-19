#include <roa_ctx/roa_ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/log.h>
#include <roa_lib/fundamental.h>
#include <GL/gl3w.h>
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

  int kb_keys[ROA_KB_COUNT];
  int ms_keys[ROA_MS_BUTTON_COUNT];

	float mouse_x;
	float mouse_y;

  float mouse_last_x;
	float mouse_last_y;

  float mouse_delta_x;
	float mouse_delta_y;

  float mouse_scroll_x;
  float mouse_scroll_y;

  float mouse_scroll_delta_x;
  float mouse_scroll_delta_y;

	int mouse_locked;
};




static void
roa_ctx_err_callback(int error, const char *desc)
{
  ROA_UNUSED(error);

  ROA_LOG_ERROR("[glfw] %s", desc);
}


static void
roa_ctx_win_size_callback(
  GLFWwindow *win,
  int width,
  int height)
{
  struct roa_ctx *ctx = (struct roa_ctx*)glfwGetWindowUserPointer(win);

  ctx->width = width;
  ctx->height = height;
}


static void
roa_ctx_cursor_pos_callback(
  GLFWwindow *win,
  double xpos,
  double ypos)
{
	struct roa_ctx *ctx = (struct roa_ctx*)glfwGetWindowUserPointer(win);

	ctx->mouse_x = (float)xpos;
	ctx->mouse_y = (float)ypos;
}


static void
roa_ctx_cursor_scroll_callback(
  GLFWwindow *win,
  double xoffset,
  double yoffset)
{
  struct roa_ctx *ctx = (struct roa_ctx*)glfwGetWindowUserPointer(win);

	ctx->mouse_scroll_delta_x += (float)xoffset;
	ctx->mouse_scroll_delta_y += (float)yoffset;
}


static void
roa_ctx_key_callback(
  GLFWwindow *win,
  int key,
  int scancode,
  int action,
  int mods)
{
  ROA_UNUSED(scancode);
  ROA_UNUSED(mods);

  struct roa_ctx *ctx = (struct roa_ctx*)glfwGetWindowUserPointer(win);

  kb_button_id roa_key = ROA_KB_UNKNOWN;
  int state = action == GLFW_RELEASE ? 0 : 1;

  switch (key)
  {
    /* numeric */
    case(GLFW_KEY_0): roa_key = ROA_KB_0; break;
    case(GLFW_KEY_1): roa_key = ROA_KB_1; break;
    case(GLFW_KEY_2): roa_key = ROA_KB_2; break;
    case(GLFW_KEY_3): roa_key = ROA_KB_3; break;
    case(GLFW_KEY_4): roa_key = ROA_KB_4; break;
    case(GLFW_KEY_5): roa_key = ROA_KB_5; break;
    case(GLFW_KEY_6): roa_key = ROA_KB_6; break;
    case(GLFW_KEY_7): roa_key = ROA_KB_7; break;
    case(GLFW_KEY_8): roa_key = ROA_KB_8; break;
    case(GLFW_KEY_9): roa_key = ROA_KB_9; break;

    /* alpha */
    case(GLFW_KEY_A): roa_key = ROA_KB_A; break;
    case(GLFW_KEY_B): roa_key = ROA_KB_B; break;
    case(GLFW_KEY_C): roa_key = ROA_KB_C; break;
    case(GLFW_KEY_D): roa_key = ROA_KB_D; break;
    case(GLFW_KEY_E): roa_key = ROA_KB_E; break;
    case(GLFW_KEY_F): roa_key = ROA_KB_F; break;
    case(GLFW_KEY_G): roa_key = ROA_KB_G; break;
    case(GLFW_KEY_H): roa_key = ROA_KB_H; break;
    case(GLFW_KEY_I): roa_key = ROA_KB_I; break;
    case(GLFW_KEY_J): roa_key = ROA_KB_J; break;
    case(GLFW_KEY_K): roa_key = ROA_KB_K; break;
    case(GLFW_KEY_L): roa_key = ROA_KB_L; break;
    case(GLFW_KEY_M): roa_key = ROA_KB_M; break;
    case(GLFW_KEY_N): roa_key = ROA_KB_N; break;
    case(GLFW_KEY_O): roa_key = ROA_KB_O; break;
    case(GLFW_KEY_P): roa_key = ROA_KB_P; break;
    case(GLFW_KEY_Q): roa_key = ROA_KB_Q; break;
    case(GLFW_KEY_R): roa_key = ROA_KB_R; break;
    case(GLFW_KEY_S): roa_key = ROA_KB_S; break;
    case(GLFW_KEY_T): roa_key = ROA_KB_T; break;
    case(GLFW_KEY_U): roa_key = ROA_KB_U; break;
    case(GLFW_KEY_V): roa_key = ROA_KB_V; break;
    case(GLFW_KEY_W): roa_key = ROA_KB_W; break;
    case(GLFW_KEY_X): roa_key = ROA_KB_X; break;
    case(GLFW_KEY_Y): roa_key = ROA_KB_Y; break;
    case(GLFW_KEY_Z): roa_key = ROA_KB_Z; break;

    /* function keys */
    case(GLFW_KEY_F1): roa_key = ROA_KB_F1; break;
    case(GLFW_KEY_F2): roa_key = ROA_KB_F2; break;
    case(GLFW_KEY_F3): roa_key = ROA_KB_F3; break;
    case(GLFW_KEY_F4): roa_key = ROA_KB_F4; break;
    case(GLFW_KEY_F5): roa_key = ROA_KB_F5; break;
    case(GLFW_KEY_F6): roa_key = ROA_KB_F6; break;
    case(GLFW_KEY_F7): roa_key = ROA_KB_F7; break;
    case(GLFW_KEY_F8): roa_key = ROA_KB_F8; break;
    case(GLFW_KEY_F9): roa_key = ROA_KB_F9; break;
    case(GLFW_KEY_F10): roa_key = ROA_KB_F10; break;
    case(GLFW_KEY_F11): roa_key = ROA_KB_F11; break;
    case(GLFW_KEY_F12): roa_key = ROA_KB_F12; break;
    case(GLFW_KEY_F13): roa_key = ROA_KB_F13; break;
    case(GLFW_KEY_F14): roa_key = ROA_KB_F14; break;
    case(GLFW_KEY_F15): roa_key = ROA_KB_F15; break;

    /* dir keys */
    case(GLFW_KEY_UP): roa_key = ROA_KB_UP; break;
    case(GLFW_KEY_DOWN): roa_key = ROA_KB_DOWN; break;
    case(GLFW_KEY_LEFT): roa_key = ROA_KB_LEFT; break;
    case(GLFW_KEY_RIGHT): roa_key = ROA_KB_RIGHT; break;

    /* other keys */
    case(GLFW_KEY_ESCAPE): roa_key = ROA_KB_ESC; break;
    case(GLFW_KEY_TAB): roa_key = ROA_KB_TAB; break;
  }

  ctx->kb_keys[roa_key] = state;
}


/* ---------------------------------------------------------- [ Lifetime ] -- */


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
    
    #ifdef __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

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
			glfwSetCursorPosCallback(win, roa_ctx_cursor_pos_callback);
      glfwSetScrollCallback(win, roa_ctx_cursor_scroll_callback);
      glfwSetKeyCallback(win, roa_ctx_key_callback);

      glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      gl3wInit();

			new_ctx->width = width;
			new_ctx->height = height;

			int bytes = strlen(title) + 1 > sizeof(new_ctx->title) - 1 ? sizeof(new_ctx->title) - 1 : strlen(title);
			memcpy(new_ctx->title, title, bytes);

      *ctx = new_ctx;

			ROA_LOG_INFO("Created Context");
    }
    else
    {
      ROA_ASSERT(0);
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
    ctx->mouse_scroll_delta_x = 0;
    ctx->mouse_scroll_delta_y = 0;

		/* graphics */
    glfwPollEvents();
    glfwSwapBuffers(ctx->window);

		/* mouse position and delta */
		ctx->mouse_delta_x = ctx->mouse_x - ctx->mouse_last_x;
		ctx->mouse_delta_y = ctx->mouse_y - ctx->mouse_last_y;

		ctx->mouse_last_x = ctx->mouse_x;
		ctx->mouse_last_y = ctx->mouse_y;

    return glfwWindowShouldClose(ctx->window) ? ROA_FALSE : ROA_TRUE;
  }

  return ROA_FALSE;
}


/* ------------------------------------------------------------ [ Window ] -- */


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
  ROA_ASSERT(desc->width);
  ROA_ASSERT(desc->title);
  ROA_ASSERT(desc->height);

	ctx->width = desc->width;
	ctx->height = desc->height;

	unsigned bytes = strlen(desc->title) + 1 > sizeof(ctx->title) - 1 ? sizeof(ctx->title) - 1 : strlen(desc->title) + 1;
	memcpy(ctx->title, desc->title, bytes);

  glfwSetWindowSize(ctx->window, desc->width, desc->height);
  glfwSetWindowTitle(ctx->window, desc->title);
}


/* ------------------------------------------------------------- [ input ] -- */


void
roa_ctx_mouse_get_desc(
	roa_ctx_t ctx,
	struct roa_ctx_mouse_desc *out_desc)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(out_desc);

  out_desc->x_pos = ctx->mouse_x;
  out_desc->y_pos = ctx->mouse_y;

  out_desc->x_delta = ctx->mouse_delta_x;
  out_desc->y_delta = ctx->mouse_delta_y;

  out_desc->x_scroll = ctx->mouse_scroll_delta_x;
  out_desc->y_scroll = ctx->mouse_scroll_delta_y;
}


void
roa_ctx_keyboard_get_desc(
  roa_ctx_t ctx,
  struct roa_ctx_keyboard_desc *out_desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(out_desc);

  out_desc->kb_keys = ROA_ARR_DATA(ctx->kb_keys);
  out_desc->kb_key_count = ROA_ARR_COUNT(ctx->kb_keys);
}
