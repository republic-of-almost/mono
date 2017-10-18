
#define NAU_LOGGING
#include <nau/nau.h>
#define NAU_GL_LOGGING
#include <nau/renderers/opengl3.h>

#include <GL/gl3w.h> // include before glfw
#include <GLFW/glfw3.h>
#include <stdio.h>


/* -------------------------------------------------- [ Application Vars ] -- */


GLFWwindow  *window = NULL;
Nau_ctx     *ctx    = NULL;
Nau_gl_ctx  *gl_ctx = NULL;
int         view_port_width = 800;
int         view_port_height = 480;


/* ------------------------------------------------- [ Application Funcs ] -- */


void
test_interface()
{
  static bool basic_test = true;

  /* basic test */
  {
    if(basic_test)
    {
      nau_begin(ctx, "Basic Window", &basic_test);
      
      if(nau_button(ctx, "click me"))
      {
        printf("Button clicked\n");
      }
      
      nau_end(ctx);
    }
  }
  
  
  /* toolbar test */
  {
    nau_win_prop_flags(ctx, NAU_WIN_NO_DRAG | NAU_WIN_NO_RESIZE | NAU_WIN_NO_CLOSE | NAU_WIN_NO_MIN);
    
    float size[2] {120, (float)view_port_height};
    nau_win_prop_size(ctx, size);
    
    float pos[2]{view_port_width - size[0], 0.f};
    nau_win_prop_position(ctx, pos);
    
    nau_begin(ctx, "Toolbar", NULL);
    
    if(nau_button(ctx, "click me"))
    {
      printf("Button clicked\n");
      basic_test = true;
    }
    
    if(nau_button(ctx, "click me too"))
    {
      printf("Other button clicked\n");
    }
    
    nau_end(ctx);
  }
}


void
render_interface()
{
  glfwMakeContextCurrent(window);
  
  glViewport(0,0,view_port_width, view_port_height);
  glScissor(0,0,view_port_width, view_port_height);
  
  glClearColor(0.1,0.1,0.1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  nau_gl3_render(gl_ctx, ctx);

  glfwSwapBuffers(window);
}


/* ---------------------------------------------------- [ GLFW Callbacks ] -- */


void
glfw_err(int err, const char *str)
{
  printf("%d: %s\n", err, str);
}


void
glfw_resize(GLFWwindow *win, int width, int height)
{
  printf("Resize: %d x %d\n", width, height);

  Nau_ctx **ctx = (Nau_ctx**)glfwGetWindowUserPointer(win);
  nau_set_viewport(*ctx, width, height);
  
  view_port_height = height;
  view_port_width = width;
  
  /* re-render interface */
  glViewport(0,0,width,height);
  render_interface();
}


void
glfw_ms_button(GLFWwindow *win, int id, int action, int mod)
{
  if (id == GLFW_MOUSE_BUTTON_1)
  {
    const Nau_ptr_status status = action == GLFW_PRESS ? NAU_PTR_STATUS_DOWN : NAU_PTR_STATUS_UP;
    Nau_ctx **ctx = (Nau_ctx**)glfwGetWindowUserPointer(win);
    
    nau_set_pointer_status(*ctx, status);
  }
}


void
glfw_ms_move(GLFWwindow *win, double x, double y)
{
  Nau_ctx **ctx = (Nau_ctx**)glfwGetWindowUserPointer(win);
  
  int vp_x, vp_y;
  nau_get_viewport(*ctx, &vp_x, &vp_y);
  
  nau_set_pointer_coords(*ctx, x, y);
}


/* ------------------------------------------------------- [ Application ] -- */


int
main()
{
  /* ------------------------------------------------------ [ GLFW Setup ] -- */
  {
    if(!glfwInit())
    {
      printf("Failed to start GLFW\n");
      return 0;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    #if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(view_port_width, view_port_height, "NAU Tests", NULL, NULL);
    glfwSetWindowUserPointer(window, (void*)&ctx);
    
    glfwSetWindowSizeCallback(window, glfw_resize);
    glfwSetErrorCallback(glfw_err);
    glfwSetMouseButtonCallback(window, glfw_ms_button);
    glfwSetCursorPosCallback(window, glfw_ms_move);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
  }
  
  /* ------------------------------------------------------- [ OGL Setup ] -- */
  {
    if(gl3wInit())
    {
      printf("Failed to start GL3W\n");
      return 0;
    }
  }

  /* ------------------------------------------------------- [ NAU Setup ] -- */
  {
    nau_initialize(&ctx);
    nau_gl3_init(&gl_ctx, ctx);
   
    nau_set_viewport(ctx, view_port_width, view_port_height);
  }

  /* ------------------------------------------------- [ Application Loop] -- */

  while(!glfwWindowShouldClose(window))
  {
    nau_new_frame(ctx);
    
    test_interface();
    
    render_interface();
    glfwPollEvents();
  }
  
  nau_gl3_destroy(&gl_ctx);
  nau_destroy(&ctx);


  return 0;
}
