#include <gltf/gltf.h>
#include <json/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <roa_lib/array.h>
#include <string.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>


static float
json_to_float(struct json_value_s *val)
{
  ROA_ASSERT(val->type = json_type_number);
  struct json_number_s *json_val = (struct json_number_s*)val->payload;
  return atof(json_val->number);
}


static int
json_to_int(struct json_value_s *val)
{
  ROA_ASSERT(val->type = json_type_number);
  struct json_number_s *json_val = (struct json_number_s*)val->payload;
  return atoi(json_val->number);
}


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

          /* ----------------------------------------------- [ accessors ] -- */

          if (strcmp(root_name->string, "accessors") == 0)
          {
            struct json_value_s *accessors = (struct json_object_s*)root_obj_ele->value;
            ROA_ASSERT(accessors->type == json_type_array);

            struct json_array_s *accessor_arr = (struct json_array_s*)accessors->payload;
            struct json_array_element_s *accessor_arr_ele = (struct json_array_element_s*)accessor_arr->start;

            while (accessor_arr_ele != ROA_NULL)
            {
              struct gltf_accessor *accessor_val = ROA_NULL;

              /* first pass we just count and size up allocation */
              if(!gltf_store)
              {
                alloc_space += sizeof(*accessor_val);
                out_import->accessor_count += 1;
              }

              /* second pass we build the data */
              else if (gltf_store)
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

                  const char *attr_str = attr_name->string;

                  if (strcmp(attr_str, "name") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_string);

                    if(accessor_val)
                    {

                    }
                  }
                  else if (strcmp(attr_str, "bufferView") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_number);

                    if(accessor_val)
                    {
                      accessor_val->buffer_view = json_to_int(attr_val);
                    }
                  }
                  else if (strcmp(attr_str, "componentType") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_number);

                    if(accessor_val)
                    {
                      accessor_val->component_type = json_to_int(attr_val);
                    }
                  }
                  else if (strcmp(attr_str, "count") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_number);

                    if(accessor_val)
                    {
                      accessor_val->count = json_to_int(attr_val);
                    }
                  }
                  else if (strcmp(attr_str, "max") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_array);
                    struct json_array_s *json_val = (struct json_array_s*)attr_val->payload;
                    struct json_array_element_s *ele = (struct json_array_element_s *)json_val->start;

                    int i = 0;

                    while (ele != ROA_NULL)
                    {
                      if(accessor_val)
                      {
                        accessor_val->max[i] = json_to_float(ele->value);
                      }

                      i++;
                      ele = ele->next;
                    }
                  }
                  else if (strcmp(attr_str, "min") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_array);
                    struct json_array_s *json_val = (struct json_array_s*)attr_val->payload;
                    struct json_array_element_s *ele = (struct json_array_element_s *)json_val->start;

                    int i = 0;

                    while (ele != ROA_NULL)
                    {
                      if(accessor_val)
                      {
                        accessor_val->min[i] = json_to_float(ele->value);
                      }

                      i++;
                      ele = ele->next;
                    }
                  }
                  else if (strcmp(attr_str, "type") == 0)
                  {
                    ROA_ASSERT(attr_val->type == json_type_string);
                    struct json_string_s *json_val = (struct json_string_s*)attr_val->payload;

                    if(accessor_val)
                    {
                      if (strcmp(json_val->string, "SCALAR") == 0)
                      {
                        accessor_val->type = GLTF_TYPE_SCALAR;
                      }
                      else if(strcmp(json_val->string, "VEC2") == 0)
                      {
                        accessor_val->type = GLTF_TYPE_VEC2;
                      }
                      else if (strcmp(json_val->string, "VEC3") == 0)
                      {
                        accessor_val->type = GLTF_TYPE_VEC3;
                      }
                      else if (strcmp(json_val->string, "VEC4") == 0)
                      {
                        accessor_val->type = GLTF_TYPE_VEC4;
                      }
                    }
                  }

                  attr_ele = attr_ele->next;
                }

              }

              accessor_arr_ele = accessor_arr_ele->next;
            }
          }

          /* --------------------------------------------- [ bufferViews ] -- */

          else if (strcmp(root_name->string, "bufferViews") == 0)
          {
            printf("buffer_view");
          }

          /* --------------------------------------------------- [ asset ] -- */

          else if (strcmp(root_name->string, "asset") == 0)
          {
            printf("asset\n");
          }

          /* ------------------------------------------------- [ buffers ] -- */

          else if (strcmp(root_name->string, "buffers") == 0)
          {
            printf("buffers\n");
          }

          /* -------------------------------------------------- [ meshes ] -- */

          else if (strcmp(root_name->string, "meshes") == 0)
          {
            printf("meshes\n");
          }

          /* --------------------------------------------------- [ nodes ] -- */

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
