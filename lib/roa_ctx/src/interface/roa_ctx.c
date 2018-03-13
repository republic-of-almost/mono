#include <roa_ctx/roa_ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/log.h>
#include <roa_lib/fundamental.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl3.h>


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

  /* UI */
  struct nk_context *nk_ctx;
};


#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


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

      gl3wInit();
			
			new_ctx->width = width;
			new_ctx->height = height;
			
			int bytes = strlen(title) + 1 > sizeof(new_ctx->title) - 1 ? sizeof(new_ctx->title) - 1 : strlen(title);
			memcpy(new_ctx->title, title, bytes);

      /* nuklear */
      {
        new_ctx->nk_ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);

        {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "C:Users/SimStim/Developer/mono/thirdparty/nuklear/assets/font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        struct nk_font *default_ft = nk_font_atlas_add_default(atlas, 13.f, 0);
        nk_glfw3_font_stash_end();
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(new_ctx->nk_ctx, &default_ft->handle);
        }
      }
			
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

    nk_glfw3_new_frame();

    struct nk_colorf bg;
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    if (nk_begin(ctx->nk_ctx, "Demo", nk_rect(50, 50, 230, 250),
      NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
      NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
    {
      

      enum { EASY, HARD };
      static int op = EASY;
      static int property = 20;
      nk_layout_row_static(ctx->nk_ctx, 30, 80, 1);
      if (nk_button_label(ctx->nk_ctx, "button"))
      {
        ROA_LOG_INFO("pressed");
      }

      nk_layout_row_dynamic(ctx->nk_ctx, 30, 2);
      if (nk_option_label(ctx->nk_ctx, "easy", op == EASY)) op = EASY;
      if (nk_option_label(ctx->nk_ctx, "hard", op == HARD)) op = HARD;

      nk_layout_row_dynamic(ctx->nk_ctx, 25, 1);
      nk_property_int(ctx->nk_ctx, "Compression:", 0, &property, 100, 10, 1);

      nk_layout_row_dynamic(ctx->nk_ctx, 20, 1);
      nk_label(ctx->nk_ctx, "background:", NK_TEXT_LEFT);
      nk_layout_row_dynamic(ctx->nk_ctx, 25, 1);
      if (nk_combo_begin_color(ctx->nk_ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx->nk_ctx), 400))) {
        nk_layout_row_dynamic(ctx->nk_ctx, 120, 1);
        bg = nk_color_picker(ctx->nk_ctx, bg, NK_RGBA);
        nk_layout_row_dynamic(ctx->nk_ctx, 25, 1);
        bg.r = nk_propertyf(ctx->nk_ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
        bg.g = nk_propertyf(ctx->nk_ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
        bg.b = nk_propertyf(ctx->nk_ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
        bg.a = nk_propertyf(ctx->nk_ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
        nk_combo_end(ctx->nk_ctx);
      }
    }
    nk_end(ctx->nk_ctx);

    glViewport(0, 0, ctx->width, ctx->height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    /* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
    * with blending, scissor, face culling, depth test and viewport and
    * defaults everything back into a default state.
    * Make sure to either a.) save and restore or b.) reset your own state after
    * rendering the UI. */
    nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

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
