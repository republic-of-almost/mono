#include "gltf.h"
#include <json/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <roa_lib/array.h>
#include <string.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>


void
gltf_import(const char *filename, struct gltf_import *out_import)
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
    if (root->type == json_type_object)
    {
      unsigned alloc_space = 0;
      unsigned char *gltf_alloc = ROA_NULL;
      unsigned char *gltf_store = ROA_NULL;

      while(gltf_alloc == ROA_NULL)
      {
        if (alloc_space)
        {
          gltf_alloc = malloc(alloc_space);
          gltf_store = gltf_alloc;
        }

        struct json_object_s *root_obj = (struct json_object_s*)root->payload;
        struct json_object_element_s *root_obj_ele = root_obj->start;

        while(root_obj_ele != ROA_NULL)
        {
          struct json_string_s *root_name = root_obj_ele->name;

          /* ------------------------------------------------ [ accessors ] -- */

          if (strcmp(root_name->string, "accessors") == 0)
          {
            struct json_value_s *accessors = (struct json_object_s*)root_obj_ele->value;
            ROA_ASSERT(accessors->type == json_type_array);

            struct json_array_s *accessor_arr = (struct json_array_s*)accessors->payload;
            struct json_array_element_s *accessor_arr_ele = (struct json_array_element_s*)accessor_arr->start;

            while (accessor_arr_ele != ROA_NULL)
            {
              struct gltf_accessor *accessor_val = ROA_NULL;

              /* second pass we build the data */
              if (gltf_store)
              {
                /* Store the start of the array */
                if (out_import->accessor_count && out_import->accessors == ROA_NULL)
                {
                  out_import->accessors = gltf_store;
                }

                accessor_val = (struct gltf_accessor*)gltf_store;
                gltf_store += sizeof(*accessor_val);

                struct json_value_s *acc_val = (struct json_value_s*)accessor_arr_ele->value;
                struct json_object_s *attr_obj = (struct json_object_s*)acc_val->payload;
                struct json_object_element_s *attr_ele = (struct json_object_element_s*)attr_obj->start;
                
                while (attr_ele != ROA_NULL)
                {
                  struct json_string_s *attr_name = attr_ele->name;
                  struct json_value_s *attr_val = (struct json_val_s*)attr_ele->value;

                  if (strcmp(attr_name->string, "bufferView") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_number);
                    struct json_number_s *json_val = (struct json_number_s*)attr_val->payload;
                    accessor_val->buffer_view = atoi(json_val->number);
                  }
                  else if (strcmp(attr_name->string, "componentType") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_number);
                    struct json_number_s *json_val = (struct json_number_s*)attr_val->payload;
                    accessor_val->component_type = atoi(json_val->number);
                  }
                  else if (strcmp(attr_name->string, "count") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_number);
                    struct json_number_s *json_val = (struct json_number_s*)attr_val->payload;
                    accessor_val->count = atoi(json_val->number);
                  }
                  else if (strcmp(attr_name->string, "max") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_array);
                  }
                  else if (strcmp(attr_name->string, "min") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_array);
                  }
                  else if (strcmp(attr_name->string, "type") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_string);
                    struct json_string_s *json_val = (struct json_string_s*)attr_val->payload;
                    int i = 0;
                  }

                  attr_ele = attr_ele->next;
                }

              }

              /* first pass we just count and size up allocation */
              else
              {
                alloc_space += sizeof(*accessor_val);
                out_import->accessor_count += 1;
              }

              accessor_arr_ele = accessor_arr_ele->next;
            }

            printf("accessors\n");
          }

          /* ---------------------------------------------- [ bufferViews ] -- */

          else if (strcmp(root_name->string, "bufferViews") == 0)
          {
            printf("buffer_view\n");
          }

          /* ---------------------------------------------------- [ asset ] -- */

          else if (strcmp(root_name->string, "asset") == 0)
          {
            printf("asset\n");
          }

          /* -------------------------------------------------- [ buffers ] -- */

          else if (strcmp(root_name->string, "buffers") == 0)
          {
            printf("buffers\n");
          }

          /* --------------------------------------------------- [ meshes ] -- */

          else if (strcmp(root_name->string, "meshes") == 0)
          {
            printf("meshes\n");
          }

          /* ---------------------------------------------------- [ nodes ] -- */

          else if (strcmp(root_name->string, "nodes") == 0)
          {
            printf("nodes\n");
          }

          /* next item */
          root_obj_ele = root_obj_ele->next;
        }

      }
      
      int i = 0;
    }
  }
}


void
gltf_free(struct gltf_import *scene)
{

}
