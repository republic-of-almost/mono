/* Scratch project - links with OpenGL and RoaCtx */


#include <roa_ctx/roa_ctx.h>
#include <GL/gl3w.h>
#include <stdio.h>


roa_ctx_t ctx = ROA_NULL;

int
main()
{
  roa_ctx_create(&ctx);
  
  if(!ctx)
  {
    return 1;
  }
  

  int major = 0;
  int minor = 0;
  
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  if(major == 0)
  {
    printf("%s - %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR));
  }
  else
  {
    printf("%d.%d\n", major, minor);
  }
  
  /* scratch code here */
  
  glClearColor(1,0,1,1);
  
  while(roa_ctx_new_frame(ctx))
  {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  }
  
  roa_ctx_destroy(&ctx);
  

  return 0;
}
