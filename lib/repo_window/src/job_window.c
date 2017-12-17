#include <repo/repo.h>
#include <GLFW/glfw3.h>
#include <string.h>


/* -------------------------------------------------------------- [ Data ] -- */


struct GLFWwindow *window;

unsigned width;
unsigned height;
unsigned fullscreen;
char title[256];

unsigned update_job_id;

/* ------------------------------------------------------------- [ Tasks ] -- */


void
update_window_job(void *arg)
{
  
};


/* ------------------------------------------------------------ [ Window ] -- */


void
glfw_setup()
{
  if (!glfwInit())
  {
    /* do something */    
  }

  window = glfwCreateWindow(width, height, title, NULL, NULL);
  glfwMakeContextCurrent(window);
}


void
glfw_process()
{
  glfwSwapBuffers(window);
  glfwPollEvents();
}


void
glfw_shutdown()
{
  glfwTerminate();
}


void
glfw_set_desc(const struct repo_window_desc *desc)
{
  /* copy description */
  width = desc->width;
  height = desc->height;
  fullscreen = desc->fullscreen;
  
  unsigned title_size = strlen(desc->title) + 1;
  unsigned bytes_to_cpy = title_size > sizeof(title) ? sizeof(title) : title_size;
  memcpy(title, desc->title, bytes_to_cpy);

  /* kick off update job */
  struct repo_job_desc job_desc;
  job_desc.function = update_window_job;
  job_desc.argument = 0;

  update_job_id = repo_job_submit(&job_desc, 1);
}


void
glfw_get_desc(struct repo_window_desc *out_desc)
{
  /* might be a pending update */
  repo_job_wait(update_job_id);
}


int
glfw_is_closing()
{
  return glfwWindowShouldClose(window) ? 1 : 0;
}


/* ------------------------------------------------------ [ Entry Points ] -- */


REPO_API void REPO_API_CALL
repo_module_create()
{
  /* clear data */
  window = 0;

  width = 0;
  height = 0;
  fullscreen = 0;
  memset(title, 0, sizeof(title));

  update_job_id = 0;

  /* setup api */
  struct repo_api_window win_api;
  win_api.window_start_process = glfw_setup;
  win_api.window_process       = glfw_process;
  win_api.window_close_process = glfw_shutdown;
  win_api.window_set_desc      = glfw_set_desc;
  win_api.window_get_desc      = glfw_get_desc;
  win_api.window_is_closing    = glfw_is_closing;

  repo_register_window_api(&win_api);
}


REPO_API void REPO_API_CALL
repo_module_destroy()
{

}
