#include <roa/roa_api.h>
#include <roa_lib/assert.h>
#include <roa_lib/fundamental.h>
#include <interface/context.h>
#include <optio/dispatcher.h>
#include <GLFW/glfw3.h>


/* --------------------------------------------------------- [ GLFW Jobs ]-- */


static void glfw_callback_key(
  GLFWwindow *window,
  int key,
  int scancode,
  int action,
  int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    roa_application_destroy();
  }
}


struct GLFWsetup
{
  GLFWwindow **window;
  unsigned width;
  unsigned height;
  const char *title;
  void *user_data; /* pass roa_ctx */
};


ROA_JOB(job_glfw_setup, struct GLFWsetup*)
{ 
  ROA_ASSERT(arg);

  if (glfwInit())
  {
    GLFWwindow *window = glfwCreateWindow(
      arg->width,
      arg->height,
      arg->title,
      NULL,
      NULL);

    ROA_ASSERT(window);

    if(window)
    {
      glfwMakeContextCurrent(window);
      glfwSwapInterval(1);
      glfwSetWindowUserPointer(window, arg->user_data);
      glfwSetKeyCallback(window, glfw_callback_key);

      /* save out the window */
      *arg->window = window;
    }
  }
}


ROA_JOB(job_glfw_quit, GLFWwindow*)
{
  ROA_ASSERT(arg);

  glfwDestroyWindow(arg);
  glfwTerminate();
}


ROA_JOB(job_glfw_new_frame, GLFWwindow*)
{
  ROA_ASSERT(arg);

  glClearColor(1,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwPollEvents();
  glfwSwapBuffers(arg);
}


/* ------------------------------------------------- [ Application Jobs ] -- */


ROA_JOB(job_app_new_frame, void*)
{
  ROA_ASSERT(arg == ROA_NULL);

  struct roa_context *ctx = roa_ctx();
  GLFWwindow *window_arg = ctx->glfw_window;

  /* things that need to update */
  {
    struct optio_job_desc job_desc[1];
    job_desc[0].func    = job_glfw_new_frame,
    job_desc[0].arg     = (void*) window_arg;
    job_desc[0].keep_on_calling_thread = ROA_NULL;

    unsigned new_frame_marker = optio_dispatcher_add_jobs(
      roa_ctx()->optio_ctx,
      ROA_ARR_DATA(job_desc),
      ROA_ARR_COUNT(job_desc));

    optio_dispatcher_wait_for_counter(
      roa_ctx()->optio_ctx,
      new_frame_marker
    );
  }

  /* re-queue users frame job */
  {
    struct optio_job_desc job_desc[1];
    job_desc[0].func    = (optio_job_func)ctx->user_frame_func;
    job_desc[0].arg     = (void*)ctx->user_frame_arg;
    job_desc[0].keep_on_calling_thread = ROA_NULL;
    
    unsigned new_user_frame_marker = optio_dispatcher_add_jobs(
      roa_ctx()->optio_ctx,
      ROA_ARR_DATA(job_desc),
      ROA_ARR_COUNT(job_desc));

    optio_dispatcher_wait_for_counter(
      roa_ctx()->optio_ctx,
      new_user_frame_marker
    );
  }

  /* re-queue this job if not quitting */
  {
    int should_quit = glfwWindowShouldClose(ctx->glfw_window);

    if(!should_quit && !ctx->quit)
    {
      struct optio_job_desc job_desc[1];
      job_desc[0].func    = job_app_new_frame;
      job_desc[0].arg     = ROA_NULL;
      job_desc[0].keep_on_calling_thread = ROA_NULL;

      optio_dispatcher_add_jobs(
        roa_ctx()->optio_ctx,
        ROA_ARR_DATA(job_desc),
        ROA_ARR_COUNT(job_desc));
     }
   }
}


ROA_JOB(job_app_quit, void*)
{
  ROA_ASSERT(arg == ROA_NULL);

  struct roa_context *ctx = roa_ctx();
  GLFWwindow* glfw_window = ctx->glfw_window;

  struct optio_job_desc job_desc[1];
  job_desc[0].func    = job_glfw_quit;
  job_desc[0].arg     = (void*)glfw_window;
  job_desc[0].keep_on_calling_thread = ROA_NULL;

  unsigned destroy_marker = optio_dispatcher_add_jobs(
    roa_ctx()->optio_ctx,
    ROA_ARR_DATA(job_desc),
    ROA_ARR_COUNT(job_desc));

  optio_dispatcher_wait_for_counter(
    roa_ctx()->optio_ctx,
    destroy_marker);
}


ROA_JOB(job_app_setup, const struct roa_application_desc*)
{
  ROA_ASSERT(arg);

  /* create all setup tasks */
  struct roa_context *ctx = roa_ctx();

  struct GLFWsetup setup_arg;
  setup_arg.window    = &ctx->glfw_window;
  setup_arg.width     = arg->width;
  setup_arg.height    = arg->height;
  setup_arg.title     = arg->title;
  setup_arg.user_data = (void*)ctx;

  ctx->user_frame_arg  = arg->application_arg;
  ctx->user_frame_func = (void*)arg->application_job;

  struct optio_job_desc job_desc[1];
  job_desc[0].func    = job_glfw_setup;
  job_desc[0].arg     = &setup_arg;
  job_desc[0].keep_on_calling_thread = ROA_NULL;

  const unsigned setup_marker = optio_dispatcher_add_jobs(
    roa_ctx()->optio_ctx,
    ROA_ARR_DATA(job_desc),
    ROA_ARR_COUNT(job_desc));

  optio_dispatcher_wait_for_counter(
    roa_ctx()->optio_ctx,
    setup_marker);


  struct optio_job_desc frame[] = {
    {
      job_app_new_frame,
      ROA_NULL,
      ROA_NULL,
    },
  };

  const unsigned frame_marker = optio_dispatcher_add_jobs(
    roa_ctx()->optio_ctx,
    ROA_ARR_DATA(frame),
    ROA_ARR_COUNT(frame));

  /* required to keep alive so that frame can copy args */
  optio_dispatcher_wait_for_counter(
    roa_ctx()->optio_ctx,
    frame_marker);
}


/* --------------------------------------------------- [ Application API ] -- */


ROA_BOOL
roa_application_create(
  const struct roa_application_desc *app_desc)
{
  /* optio */
  {
    struct optio_dispatcher_desc optio_desc =
    {
      1,
      1 << 7,
      1 << 10,
    };

    optio_dispatcher_create(&roa_ctx()->optio_ctx, &optio_desc);
  }

  /* start up the dispatcher manager */
  {
    struct optio_job_desc desc;
    desc.func = job_app_setup;
    desc.arg = (void*)app_desc;
    desc.keep_on_calling_thread = 1;

    optio_dispatcher_add_jobs(
      roa_ctx()->optio_ctx,
      &desc,
      1);

    optio_dispatcher_run(roa_ctx()->optio_ctx);
  }

  /* destroy application */
  {
    struct optio_job_desc desc = {
      job_app_quit,
      ROA_NULL,
      ROA_NULL,
    };

    optio_dispatcher_add_jobs(
      roa_ctx()->optio_ctx,
      &desc,
      1);

    optio_dispatcher_run(roa_ctx()->optio_ctx);
  }

  return ROA_TRUE;
}


ROA_BOOL
roa_application_destroy()
{
  struct roa_context *ctx = roa_ctx();
  ctx->quit = 1;

  return ROA_TRUE;
}