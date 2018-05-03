#include "gltf.h"
#include <json/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <roa_lib/array.h>


void
gltf_import(const char *filename, struct gltf_import *out_scene)
{
  struct json_value_s *root = 0;

  /* load gltf */
  {
    unsigned char *src = 0;
    int src_length = 0;
    FILE *file = fopen(filename, "r");

    if (file != 0)
    {
      if (fseek(file, 0L, SEEK_END) == 0)
      {
        src_length = ftell(file);

        if(src_length == -1) { /* error */ }

        src = malloc(sizeof(src[0]) * (src_length));

        if (fseek(file, 0L, SEEK_SET) != 0)
        {
          /* error */
        }

        fread(src, 1, src_length, file);

        fclose(file);
      }
    }

    root = json_parse((void*)src, src_length);
  }

  /* process gltf */
  {

  }
}


void
gltf_free(struct gltf_import *scene)
{

}
