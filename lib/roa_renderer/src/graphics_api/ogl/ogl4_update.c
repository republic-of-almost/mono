#ifdef ROA_RENDERER_API_GL4

#include <graphics_api/platform.h>
#include <roa_lib/array.h>
#include <GL/gl3w.h>


void
platform_update(roa_renderer_ctx_t ctx)
{
  int pending_meshes = roa_array_size(ctx->resource_desc.mesh_pending_ids);

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
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

        if (glObjectLabel) {
          const char * name = pending[i].name ? pending[i].name : "UnknownVBO";
          glObjectLabel(GL_BUFFER, vbos[i], -1, name);
        }

        unsigned v_size = sizeof(pending[i].position_vec3_array[0]) * (pending[i].vertex_count * (pending[i].position_vec3_array ? 3 : 0));
        unsigned n_size = sizeof(pending[i].normal_vec3_array[0]) * (pending[i].vertex_count * (pending[i].normal_vec3_array ? 3 : 0));
        unsigned t_size = sizeof(pending[i].texture_coord_vec2_array[0]) * (pending[i].vertex_count * (pending[i].texture_coord_vec2_array ? 2 : 0));

        unsigned buffer_size = v_size + n_size + t_size;

        glBufferData(GL_ARRAY_BUFFER, buffer_size, ROA_NULL, GL_STATIC_DRAW);

        if (v_size)
        {
          glBufferSubData(GL_ARRAY_BUFFER, 0, v_size, pending[i].position_vec3_array);
        }
        if (n_size)
        {
          glBufferSubData(GL_ARRAY_BUFFER, v_size, n_size, pending[i].normal_vec3_array);
        }
        if (v_size)
        {
          glBufferSubData(GL_ARRAY_BUFFER, v_size + n_size, t_size, pending[i].texture_coord_vec2_array);
        }
      }

      /* ibo */
      if(pending[i].index_count)
      {
        glBindBuffer(GL_ARRAY_BUFFER, ibos[i]);

        if (glObjectLabel) {
          const char * name = pending[i].name ? pending[i].name : "UnknownIBO";
          glObjectLabel(GL_BUFFER, ibos[i], -1, name);
        }

        unsigned buffer_size = sizeof(pending[i].index_array[0]) * pending[i].index_count;

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, pending[i].index_array, GL_STATIC_DRAW);
      }
    }

    for (i = 0; i < count; ++i)
    {
      struct ogl_mesh mesh;

      mesh.ibo = ibos[i];
      mesh.index_count = pending[i].index_count;

      mesh.vbo = vbos[i];
      mesh.vertex_count = pending[i].vertex_count;

      roa_array_push(ctx->graphics_api.mesh_ids, pending_ids[i]);
      roa_array_push(ctx->graphics_api.meshes, mesh);
    }

    roa_array_clear(ctx->resource_desc.mesh_pending_ids);
    roa_array_clear(ctx->resource_desc.mesh_rsrc_pending_data);
  }
}


#endif /* ROA_RENDERER_API_GL4 */
