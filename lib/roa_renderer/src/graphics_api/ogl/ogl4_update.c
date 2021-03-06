#ifdef ROA_RENDERER_API_GL4

#include <graphics_api/platform.h>
#include <graphics_api/ogl/ogl4_helpers.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <GL/gl3w.h>
#include <stdlib.h>
#include <string.h>


void
platform_update(roa_renderer_ctx_t ctx)
{
  int pending_meshes = roa_array_size(ctx->resource_desc.mesh_pending_ids);
  int pending_textures = roa_array_size(ctx->resource_desc.texture_pending_ids);

  /* load meshes */
  if(pending_meshes > 0)
  {
    int count = pending_meshes;
    int i;

    GLuint *vbos = 0;
    roa_array_create_with_capacity(vbos, count);
    roa_array_resize(vbos, count);

    GLuint *ibos = 0;
    roa_array_create_with_capacity(ibos, count);
    roa_array_resize(ibos, count);

    glGenBuffers(count, vbos);
    glGenBuffers(count, ibos);

    struct roa_renderer_mesh_resource *pending = ctx->resource_desc.mesh_rsrc_pending_data;
    uint64_t *pending_ids = ctx->resource_desc.mesh_pending_ids;

    for (i = 0; i < count; ++i)
    {
      /* vbo */
      if (pending[i].vertex_count)
      {
        unsigned element_count = 0;
        element_count += pending[i].position_vec3_array ? 3 : 0;
        element_count += pending[i].normal_vec3_array ? 3 : 0;
        element_count += pending[i].texture_coord_vec2_array ? 2 : 0;

        float *data = 0;
        roa_array_create_with_capacity(data, element_count * pending[i].vertex_count);
        roa_array_resize(data, element_count * pending[i].vertex_count);

        int j;
        int offset = 0;
        int vertex_count = (int)pending[i].vertex_count;

        for (j = 0; j < vertex_count; ++j)
        {
          if (pending[i].position_vec3_array)
          {
            int index = j * 3;
            data[offset++] = pending[i].position_vec3_array[index + 0];
            data[offset++] = pending[i].position_vec3_array[index + 1];
            data[offset++] = pending[i].position_vec3_array[index + 2];
          }

          if (pending[i].texture_coord_vec2_array)
          {
            int index = j * 2;
            data[offset++] = pending[i].texture_coord_vec2_array[index + 0];
            data[offset++] = pending[i].texture_coord_vec2_array[index + 1];
          }

          if (pending[i].normal_vec3_array)
          {
            int index = j * 3;
            data[offset++] = pending[i].normal_vec3_array[index + 0];
            data[offset++] = pending[i].normal_vec3_array[index + 1];
            data[offset++] = pending[i].normal_vec3_array[index + 2];
          }
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

        if (OGL4_DEBUG_LABLES)
        {
          if (pending[i].name)
          {
            char name[2048];
            memset(name, 0, sizeof(name));
            strcat(name, pending[i].name);
            strcat(name, ":VBO");
            glrObjectLabel(GL_BUFFER, vbos[i], name);
          }
          else
          {
            glrObjectLabel(GL_BUFFER, vbos[i], "Unknown:VBO");
          }
        }

        unsigned buffer_size = sizeof(pending[i].position_vec3_array[0]) * element_count * pending[i].vertex_count;
        glBufferData(GL_ARRAY_BUFFER, buffer_size, data, GL_STATIC_DRAW);

        if (OGL4_ERROR_CHECKS) {
          GLuint err = glGetError();
          if (err) {
            ROA_ASSERT(0);
          }
        }
      }

      /* ibo */
      if(pending[i].index_count)
      {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[i]);

        if (OGL4_DEBUG_LABLES)
        {
          if (pending[i].name)
          {
            char name[2048];
            memset(name, 0, sizeof(name));
            strcat(name, pending[i].name);
            strcat(name, ":IBO");
            glrObjectLabel(GL_BUFFER, ibos[i], name);
          }
          else
          {
            glrObjectLabel(GL_BUFFER, ibos[i], "Unknown:IBO");
          }
        }

        unsigned buffer_size = pending[i].index_stride * pending[i].index_count;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, pending[i].index_array, GL_STATIC_DRAW);

        if (OGL4_ERROR_CHECKS) {
          GLuint err = glGetError();
          if (err) {
            ROA_ASSERT(0);
          }
        }
      }
    }

    for (i = 0; i < count; ++i)
    {
      struct ogl_mesh mesh;

      mesh.ibo = ibos[i];
      mesh.index_count = pending[i].index_count;
      mesh.index_type = stride_to_gl_index_type(pending[i].index_stride);

      mesh.vbo = vbos[i];
      mesh.vertex_count = pending[i].vertex_count;

      roa_array_push(ctx->graphics_api.mesh_ids, pending_ids[i]);
      roa_array_push(ctx->graphics_api.meshes, mesh);
    }

    roa_array_clear(ctx->resource_desc.mesh_pending_ids);
    roa_array_clear(ctx->resource_desc.mesh_rsrc_pending_data);
  }

  if (pending_textures)
  {
    int count = pending_textures;
    int i;

    GLuint *textures = 0;
    roa_array_create_with_capacity(textures, count);
    roa_array_resize(textures, count);

    glGenTextures(count, textures);

    struct roa_renderer_texture_resource *pending = ctx->resource_desc.texture_rsrc_pending_data;

    for (i = 0; i < count; ++i)
    {
      glBindTexture(GL_TEXTURE_2D, textures[i]);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (OGL4_ERROR_CHECKS)
      {
        GLuint err = glGetError();
        if (err) {
          ROA_ASSERT(0);
        }
      }

      if(OGL4_DEBUG_LABLES)
      {
        const char * name = pending[i].name ? pending[i].name : "UnknownTexture";
        glrObjectLabel(GL_TEXTURE, textures[i], name);
      }

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pending[i].width, pending[i].height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, pending[i].data);

      glGenerateMipmap(GL_TEXTURE_2D);

      ctx->graphics_api.tex = textures[i];

      if (OGL4_ERROR_CHECKS)
      {
        GLuint err = glGetError();
        if (err) {
          ROA_ASSERT(0);
        }
      }
    }

    roa_array_clear(ctx->resource_desc.texture_pending_ids);
    roa_array_clear(ctx->resource_desc.texture_rsrc_pending_data);
  }
}


#endif /* ROA_RENDERER_API_GL4 */
