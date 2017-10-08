

#include <rov/rov.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdio.h>


GLFWwindow *window = NULL;


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

  /*  */

  while(!glfwWindowShouldClose(window))
  {
    glClearColor(0.11,0.1,0.1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  
  


  return 0;
}
