
#include <GLFW/glfw3.h>
#include "test_spin_cube.c"
#include "test_square.c"


/* app stuff */
GLFWwindow* window;
void app_create();
void app_destroy();
ROA_BOOL app_tick();


typedef void(*test_create_fn)();
typedef void(*test_tick_fn)();
typedef void(*test_destroy_fn)();


int
main()
{
  test_create_fn = test_cube_create;
  test_tick_fn = test_cube_tick;
  test_destroy_fn = test_cube_destroy;

  app_create();
  test_create_fn();

  while (app_tick())
  {
    test_tick_fn();
  }
  
  test_destroy_fn();
  app_destroy();

  return 0;
}


/* app details */


void
app_create()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 480, "My Title", NULL, NULL);
	glfwMakeContextCurrent(window);

  printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
}


ROA_BOOL
app_tick()
{
	glfwPollEvents();
	glfwSwapBuffers(window);

	return glfwWindowShouldClose(window) ? ROA_FALSE : ROA_TRUE;
}


void
app_destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	window = ROA_NULL;
}