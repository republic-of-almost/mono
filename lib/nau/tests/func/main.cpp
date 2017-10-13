

#define NAU_LOGGING
#include <nau/nau.h>
#include <nau/renderers/opengl3.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdio.h>


GLFWwindow  *window = NULL;
Nau_ctx     *ctx = NULL;
Nau_gl_ctx  *gl_ctx = NULL;


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
  nau_set_viewport(*	ctx, width, height);
}


/* ------------------------------------------------------- [ Application ] -- */


int
main()
{
  const int init_width  = 800;
  const int init_height = 480;

  /* GFLW */
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
    
    window = glfwCreateWindow(init_width, init_height, "NAU Func Tests", NULL, NULL);
    glfwSetWindowUserPointer(window, (void*)&ctx);
    
    glfwSetWindowSizeCallback(window, glfw_resize);
    glfwSetErrorCallback(glfw_err);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
  }
  
  /* OpenGL */
  {
    if(gl3wInit())
    {
      printf("Failed to start GL3W\n");
      return 0;
    }
  }

  /* Nau Setup */
  {
    nau_initialize(&ctx);
    nau_gl3_init(&gl_ctx, ctx);
   
    nau_set_viewport(ctx, init_width, init_height);
  }

  /*  */

  while(!glfwWindowShouldClose(window))
  {
    /* new frame */
    {
      glClearColor(0.87,0.87,0.85,1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      nau_new_frame(ctx);
    }
    
            
    /* toolbar test */
    {
      nau_begin(ctx, "Toolbar");
      
      nau_end(ctx);
    }
    
    
    /* render */
    {
      nau_gl3_render(gl_ctx, ctx);
      
      glfwPollEvents();
      glfwSwapBuffers(window);
    }
  }
  
  nau_gl3_destroy(&gl_ctx);
  nau_destroy(&ctx);


  return 0;
}
