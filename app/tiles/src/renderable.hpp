#ifndef RENDERABLE_INCLDUED_
#define RENDERABLE_INCLDUED_


#include <stddef.h>


struct
edgeRenderable
{

};


struct
edgeCamera
{

};



void
edgeInit();


void
edgeExecute(
  edgeCamera *cameras,
  size_t camera_count,
  edgeRenderable *renderables,
  size_t renderable_count
);


#endif // inc guard
