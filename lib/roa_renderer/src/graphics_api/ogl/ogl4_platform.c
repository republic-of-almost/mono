#include <graphics_api/platform.h>
#include <GL/gl3w.h>


void
platform_setup()
{
  
}


void
platform_update()
{

}


void
platform_render()
{
  glClearColor(1,1,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void
platform_destroy()
{

}
