#include "gltf.h"
#include <json/json.h>
#include <stdio.h>


void
gltf_import(const char *filename, struct gltf_import *out_scene)
{
  unsigned char *src = 0;
  FILE *file = fopen(filename, "r");

  if (file != 0)
  {
    if (fseek(file, 0L, SEEK_END) == 0)
    {
      long src_size = ftell(file);

      if(src_size == -1) { /* error */ }

      src = malloc(sizeof(src[0]) * (src_size + 1));

      if (fseek(file, 0L, SEEK_SET) != 0)
      {
        /* error */  
      }
    }
  }

  CLEANUP:
  {
    
  }
}


void
gltf_free(struct gltf_import *scene)
{

}