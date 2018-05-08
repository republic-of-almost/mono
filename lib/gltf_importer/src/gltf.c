#include <gltf/gltf.h>
#include <json/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <roa_lib/array.h>
#include <string.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/encode.h>


/* ------------------------------------------------------ [ json helpers ] -- */


static float
json_to_float(struct json_value_s *val)
{
  ROA_ASSERT(val->type == json_type_number);
  struct json_number_s *json_val = (struct json_number_s*)val->payload;
  return atof(json_val->number);
}


static int
json_to_int(struct json_value_s *val)
{
  ROA_ASSERT(val->type == json_type_number);
  struct json_number_s *json_val = (struct json_number_s*)val->payload;
  return atoi(json_val->number);
}

static int
json_attr_is_called(struct json_object_element_s *ele, const char *name)
{
  struct json_string_s *attr_name = ele->name;
  return strcmp(attr_name->string, name) == 0 ? 1 : 0;
}


static const char *
json_to_str(struct json_value_s *val)
{
  ROA_ASSERT(val->type == json_type_string);
  struct json_string_s *json_val = (struct json_string_s *)val->payload;
  return json_val->string;
}


/* ---------------------------------------------- [ GLTF Atribute Parser ] -- */


void
gltf_meshes(struct json_value_s *meshes, struct gltf_import *out_import)
{
  /* param check */
  ROA_ASSERT(meshes);
  ROA_ASSERT(out_import);
  ROA_ASSERT(meshes->type == json_type_array);

  struct json_array_s *mesh_arr = (struct json_array_s*)meshes->payload;
  struct json_array_element_s *mesh_arr_ele = (struct json_array_element_s*)mesh_arr->start;

  while(mesh_arr_ele != ROA_NULL) {
    struct gltf_mesh mesh;
    ROA_MEM_ZERO(mesh);

    struct json_value_s *acc_val = (struct json_value_s*)mesh_arr_ele->value;
    struct json_object_s *attr_obj = (struct json_object_s*)acc_val->payload;
    struct json_object_element_s *attr_ele = (struct json_object_element_s*)attr_obj->start;

    while(attr_ele != ROA_NULL)
    {
      if(json_attr_is_called(attr_ele, "primitives"))
      {
        struct gltf_primitive prim;
        ROA_MEM_ZERO(prim);

        struct json_value_s *prim_val = (struct json_value_s*)attr_ele->value;
        struct json_array_s *prim_attr_obj = (struct json_array_s*)prim_val->payload;
        struct json_array_element_s *prim_ele = (struct json_array_element_s*)prim_attr_obj->start;

        if(prim_ele)
        {
          roa_array_create(mesh.primitives);
        }

        while(prim_ele != ROA_NULL)
        {
          struct json_value_s *acc_val = (struct json_value_s*)prim_ele->value;
          struct json_object_s *attr_obj = (struct json_object_s*)acc_val->payload;
          struct json_object_element_s *attr_ele = (struct json_object_element_s*)attr_obj->start;

          while(attr_ele != ROA_NULL)
          {

            if(json_attr_is_called(attr_ele, "attributes"))
            {
              struct gltf_attributes attrs;
              attrs.COLOR_0     = -1;
              attrs.JOINTS_0    = -1;
              attrs.NORMAL      = -1;
              attrs.POSITION    = -1;
              attrs.TANGENT     = -1;
              attrs.TEXCOORD_0  = -1;
              attrs.TEXCOORD_1  = -1;
              attrs.WEIGHTS_0   = -1;

              struct json_value_s *patt_val = (struct json_value_s*)attr_ele->value;
              struct json_object_s *pattr_obj = (struct json_object_s*)patt_val->payload;
              struct json_object_element_s *pattr_ele = (struct json_object_element_s*)pattr_obj->start;

              while (pattr_ele != ROA_NULL)
              {
                if (json_attr_is_called(pattr_ele, "COLOR_0")) {
                  attrs.COLOR_0 = json_to_int(pattr_ele->value);
                }
                else if (json_attr_is_called(pattr_ele, "JOINTS_0")) {
                  attrs.JOINTS_0 = json_to_int(pattr_ele->value);
                }
                else if (json_attr_is_called(pattr_ele, "NORMAL")) {
                  attrs.NORMAL = json_to_int(pattr_ele->value);
                }
                else if (json_attr_is_called(pattr_ele, "POSITION")) {
                  attrs.POSITION = json_to_int(pattr_ele->value);
                }
                else if (json_attr_is_called(pattr_ele, "TANGENT")) {
                  attrs.TANGENT = json_to_int(pattr_ele->value);
                }
                else if (json_attr_is_called(pattr_ele, "TEXCOORD_0")) {
                  attrs.TEXCOORD_0 = json_to_int(pattr_ele->value);
                }
                else if (json_attr_is_called(pattr_ele, "TEXCOORD_1")) {
                  attrs.TEXCOORD_1 = json_to_int(pattr_ele->value);
                }
                else if (json_attr_is_called(pattr_ele, "WEIGHTS_0")) {
                  attrs.WEIGHTS_0 = json_to_int(pattr_ele->value);
                }
                
                pattr_ele = pattr_ele->next;
              }

              prim.attributes = attrs;
            }
            else if (json_attr_is_called(prim_ele, "indices"))
            {
              prim.indices = json_to_int(prim_ele->value);
            }
            else if (json_attr_is_called(prim_ele, "material"))
            {
              prim.material = json_to_int(prim_ele->value);
            }

            attr_ele = attr_ele->next;
          }

          roa_array_push(mesh.primitives, prim);
          mesh.primitive_count += 1;

          prim_ele = prim_ele->next;
        }

        
      } else if(json_attr_is_called(attr_ele, "weights")) {
        /* not done yet */
      } else if(json_attr_is_called(attr_ele, "name")) {
        const char *name = json_to_str(attr_ele->value);
        int len = strlen(name) + 1;
        roa_array_create_with_capacity(mesh.name, len);
        roa_array_resize(mesh.name, len);
        memcpy(mesh.name, name, len);
      } else if(json_attr_is_called(attr_ele, "extensions")) {
        /* missing */
      } else if(json_attr_is_called(attr_ele, "extras")) {
        /* missing */
      }

      attr_ele = attr_ele->next;
    }

    roa_array_push(out_import->meshes, mesh);
    out_import->mesh_count += 1;

    mesh_arr_ele = mesh_arr_ele->next;
  }
}


void
gltf_buffers(struct json_value_s *buffers, struct gltf_import *out_import)
{
  /* param check */
  ROA_ASSERT(buffers);
  ROA_ASSERT(out_import);
  ROA_ASSERT(buffers->type == json_type_array);

  struct json_array_s *buffer_arr = (struct json_array_s*)buffers->payload;
  struct json_array_element_s *buffer_arr_ele = (struct json_array_element_s*)buffer_arr->start;

  while (buffer_arr_ele != ROA_NULL) {
    struct gltf_buffer buffer;
    ROA_MEM_ZERO(buffer);

    struct json_value_s *acc_val = (struct json_value_s*)buffer_arr_ele->value;
    struct json_object_s *attr_obj = (struct json_object_s*)acc_val->payload;
    struct json_object_element_s *attr_ele = (struct json_object_element_s*)attr_obj->start;

    while (attr_ele != ROA_NULL) {

      if (json_attr_is_called(attr_ele, "byteLength")) {
        buffer.byte_length = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "uri")) {

        const char *uri_value = json_to_str(attr_ele->value);

        const char *embedded[] = {
          "data:image/png;base64,",
          "data:application/octet-stream;base64,",
        };

        int i;
        int count = ROA_ARR_COUNT(embedded);

        for (i = 0; i < count; ++i)
        {
          if (strncmp(uri_value, embedded[i], strlen(embedded[i])) == 0)
          {
            unsigned start = strlen(embedded[i]);
            unsigned len = strlen(uri_value) - start;

            unsigned decode_len = 0;
            roa_base64_decode((const unsigned char *)&uri_value[start], len, ROA_NULL, &decode_len);

            unsigned char * data = malloc(decode_len);
            roa_base64_decode((const unsigned char *)&uri_value[start], len, &data, &decode_len);

            buffer.uri_data = data;

            len = strlen(embedded[i]) + 1;
            roa_array_create_with_capacity(buffer.uri, len);
            roa_array_resize(buffer.uri, len);
            memcpy(buffer.uri, embedded[i], len);
          }
        }
      }
      else if (json_attr_is_called(attr_ele, "name")) {
        const char *name = json_to_str(attr_ele->value);
        int len = strlen(name) + 1;
        roa_array_create_with_capacity(buffer.name, len);
        roa_array_resize(buffer.name, len);
        memcpy(buffer.name, name, len);
      }
      else if (json_attr_is_called(attr_ele, "extensions"))
      {
        /* not handled */
      }
      else if (json_attr_is_called(attr_ele, "extras"))
      {
        /* not handled */
      }

      attr_ele = attr_ele->next;
    }

    roa_array_push(out_import->buffers, buffer);
    out_import->buffer_count += 1;

    buffer_arr_ele = buffer_arr_ele->next;
  }
}

void
gltf_buffer_views(struct json_value_s *buffer_views, struct gltf_import *out_import)
{
  /* param check */
  ROA_ASSERT(buffer_views);
  ROA_ASSERT(out_import);
  ROA_ASSERT(buffer_views->type == json_type_array);

  struct json_array_s *buffer_view_arr = (struct json_array_s*)buffer_views->payload;
  struct json_array_element_s *buffer_arr_ele = (struct json_array_element_s*)buffer_view_arr->start;

  while (buffer_arr_ele != ROA_NULL) {
    struct gltf_buffer_view buffer_view;
    ROA_MEM_ZERO(buffer_view);

    struct json_value_s *acc_val = (struct json_value_s*)buffer_arr_ele->value;
    struct json_object_s *attr_obj = (struct json_object_s*)acc_val->payload;
    struct json_object_element_s *attr_ele = (struct json_object_element_s*)attr_obj->start;

    while (attr_ele != ROA_NULL) {

      if (json_attr_is_called(attr_ele, "buffer")) {
        buffer_view.buffer = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "byteOffset")) {
        buffer_view.byte_offset = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "byteLength")) {
        buffer_view.byte_length = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "byteStride")) {
        buffer_view.byte_stride = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "target")) {
        buffer_view.target = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "name")) {
        const char *name = json_to_str(attr_ele->value);
        int len = strlen(name) + 1;
        roa_array_create_with_capacity(buffer_view.name, len);
        roa_array_resize(buffer_view.name, len);
        memcpy(buffer_view.name, name, len);
      }
      else if (json_attr_is_called(attr_ele, "extensions"))
      {
        /* not handled */
      }
      else if (json_attr_is_called(attr_ele, "extras"))
      {
        /* not handled */
      }

      attr_ele = attr_ele->next;
    }

    roa_array_push(out_import->buffer_views, buffer_view);
    out_import->buffer_view_count += 1;

    buffer_arr_ele = buffer_arr_ele->next;
  }
}


void
gltf_accessors(struct json_value_s *accessors, struct gltf_import *out_import)
{
  /* param check */
  ROA_ASSERT(accessors);
  ROA_ASSERT(out_import);
  ROA_ASSERT(accessors->type == json_type_array);

  struct json_array_s *accessor_arr = (struct json_array_s*)accessors->payload;
  struct json_array_element_s *accessor_arr_ele = (struct json_array_element_s*)accessor_arr->start;

  while (accessor_arr_ele != ROA_NULL) {
    struct gltf_accessor accessor;
    ROA_MEM_ZERO(accessor);

    struct json_value_s *acc_val = (struct json_value_s*)accessor_arr_ele->value;
    struct json_object_s *attr_obj = (struct json_object_s*)acc_val->payload;
    struct json_object_element_s *attr_ele = (struct json_object_element_s*)attr_obj->start;

    while (attr_ele != ROA_NULL) {
      if (json_attr_is_called(attr_ele, "name")) {
        const char *name = json_to_str(attr_ele->value);
        int len = strlen(name) + 1;
        roa_array_create_with_capacity(accessor.name, len);
        roa_array_resize(accessor.name, len);
        memcpy(accessor.name, name, len);
      }
      else if (json_attr_is_called(attr_ele, "bufferView")) {
        accessor.buffer_view = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "componentType")) {
        accessor.component_type = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "count")) {
        accessor.count = json_to_int(attr_ele->value);
      }
      else if (json_attr_is_called(attr_ele, "max")) {
        ROA_ASSERT(attr_ele->value->type == json_type_array);
        struct json_array_s *json_val = (struct json_array_s*)attr_ele->value->payload;
        struct json_array_element_s *ele = (struct json_array_element_s *)json_val->start;

        roa_array_create(accessor.max);

        while (ele != ROA_NULL)
        {
          roa_array_push(accessor.max, json_to_float(ele->value));

          ele = ele->next;
        }

        accessor.max_count = roa_array_size(accessor.max);
      }
      else if (json_attr_is_called(attr_ele, "min")) {
        ROA_ASSERT(attr_ele->value->type == json_type_array);
        struct json_array_s *json_val = (struct json_array_s*)attr_ele->value->payload;
        struct json_array_element_s *ele = (struct json_array_element_s *)json_val->start;

        roa_array_create(accessor.min);

        while (ele != ROA_NULL) {
          roa_array_push(accessor.min, json_to_float(ele->value));

          ele = ele->next;
        }

        accessor.min_count = roa_array_size(accessor.min);
      }
      else if (json_attr_is_called(attr_ele, "type")) {
        ROA_ASSERT(attr_ele->value->type == json_type_string);
        struct json_string_s *json_val = (struct json_string_s*)attr_ele->value->payload;

        if (strcmp(json_val->string, "SCALAR") == 0) {
          accessor.type = GLTF_TYPE_SCALAR;
        }
        else if (strcmp(json_val->string, "VEC2") == 0) {
          accessor.type = GLTF_TYPE_VEC2;
        }
        else if (strcmp(json_val->string, "VEC3") == 0) {
          accessor.type = GLTF_TYPE_VEC3;
        }
        else if (strcmp(json_val->string, "VEC4") == 0) {
          accessor.type = GLTF_TYPE_VEC4;
        }
      }
      else if (json_attr_is_called(attr_ele, "normalized")) {
        accessor.normalized = 1;
      }

      attr_ele = attr_ele->next;
    }

    roa_array_push(out_import->accessors, accessor);
    out_import->accessor_count += 1;

    accessor_arr_ele = accessor_arr_ele->next;
  }
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

  /* setup import */
  {
    roa_array_create_with_capacity(out_import->accessors, 32);
    roa_array_create_with_capacity(out_import->buffers, 32);
    roa_array_create_with_capacity(out_import->buffer_views, 32);
    roa_array_create_with_capacity(out_import->meshes, 32);
  }

  /* process gltf */
  {
    if (root->type == json_type_object)
    {
      struct json_object_s *root_obj = (struct json_object_s*)root->payload;
      struct json_object_element_s *root_obj_ele = root_obj->start;

      while(root_obj_ele != ROA_NULL) {
        struct json_string_s *root_name = root_obj_ele->name;

        /* ------------------------------------------------- [ accessors ] -- */

        if (json_attr_is_called(root_obj_ele, "accessors")) {
          gltf_accessors(root_obj_ele->value, out_import);
        }

        /* ----------------------------------------------- [ bufferViews ] -- */

        else if (strcmp(root_name->string, "bufferViews") == 0) {
          gltf_buffer_views(root_obj_ele->value, out_import);
        }

        /* ----------------------------------------------------- [ asset ] -- */

        else if (strcmp(root_name->string, "asset") == 0) {
          printf("asset\n");
        }

        /* --------------------------------------------------- [ buffers ] -- */

        else if (strcmp(root_name->string, "buffers") == 0) {
          gltf_buffers(root_obj_ele->value, out_import);
        }

        /* ---------------------------------------------------- [ meshes ] -- */

        else if (strcmp(root_name->string, "meshes") == 0) {
          gltf_meshes(root_obj_ele->value, out_import);
        }

        /* ----------------------------------------------------- [ nodes ] -- */

        else if (strcmp(root_name->string, "nodes") == 0) {
          printf("nodes\n");
        }

        /* next item */
        root_obj_ele = root_obj_ele->next;
      }
    }
  }
}


void
gltf_free(struct gltf_import *scene)
{

}
