
#include <roa_ctx/roa_ctx.h>
#include <roa_lib/dir.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <gltf/gltf.h>
#include <GL/gl3w.h>
#include <string.h>
#include <stdio.h>
#include <scratch/glsl.h>
#include <roa_math/math.h>


roa_ctx_t hw_ctx;
struct gltf_import gltf;

GLuint vao;
GLuint program;

struct mesh_data
{
  GLuint vao;
  GLuint vbo;
  GLuint ibo;
  GLsizei ibo_count;
  GLenum ibo_index_type;
};
 

#define MESH_MAX 10
struct mesh_data mesh_data[MESH_MAX];
int mesh_data_count = 0;


float distance[3];

int
main()
{
  /* setup sys/data */
  {
    roa_ctx_create(&hw_ctx);
    gl3wInit();

    char path[2048];
    ROA_MEM_ZERO(path);

    strcat(path, roa_exe_dir());
    //strcat(path, "assets/gltf_test/cube.gltf");
    strcat(path, "assets/gltf_test/plane_trainer.gltf");

    gltf_import(path, &gltf);
  }

  /* setup gl */
  {
    /* state */
    {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      glEnable(GL_DEPTH_TEST);
    }

    /* program */
    {
      GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
      const char *vs_src = glsl_fullbright_vs();
      glShaderSource(vert_shd, 1, &vs_src, NULL);
      glCompileShader(vert_shd);

      // Create and compile the fragment shader
      GLuint frag_shd = glCreateShader(GL_FRAGMENT_SHADER);
      const char *fs_src = glsl_fullbright_fs();
      glShaderSource(frag_shd, 1, &fs_src, NULL);
      glCompileShader(frag_shd);

      // Link the vertex and fragment shader into a shader program
      program = glCreateProgram();
      glAttachShader(program, vert_shd);
      glAttachShader(program, frag_shd);
      glBindFragDataLocation(program, 0, "fs_out_fragcolor");
      glLinkProgram(program);
      glUseProgram(program);

      glDeleteShader(vert_shd);
      glDeleteShader(frag_shd);
    }

    /* check err */
    {
      GLuint err = glGetError();
      if (err) { ROA_ASSERT(0); }
    }

    /* vbo */
    /*
      vbo data is stored in chunks |pos|pos|pos|norm|norm|norm|tex|tex|tex|
      this is because we can just pass the pointer to the gltf data buffer.
      All we need is the data pointers and now big the vbo should be.
    */

    int mesh_data_cap = ROA_ARR_COUNT(mesh_data);
    mesh_data_count = gltf.mesh_count > mesh_data_cap ? mesh_data_cap : gltf.mesh_count;
    int m;

    for(m = 0; m < mesh_data_count; ++m)
    {
      struct gltf_mesh *mesh = &gltf.meshes[m];
      struct mesh_data *curr_mesh = &mesh_data[m];
      
      glGenVertexArrays(1, &curr_mesh->vao);
      glBindVertexArray(curr_mesh->vao);

      int vertex_desc[3];
      {
        /* we need to now this up front to build vbo and be able to enable attrs */
        {
          vertex_desc[0] = mesh->primitives->attributes.POSITION;
          vertex_desc[1] = mesh->primitives->attributes.NORMAL;
          vertex_desc[2] = mesh->primitives->attributes.TEXCOORD_0;
        }

        /* where to place camera */
        {
          int pos = mesh->primitives->attributes.POSITION;
          distance[0] = gltf.accessors[pos].max[0] * 4;
          distance[1] = gltf.accessors[pos].max[1] * 4;
          distance[2] = gltf.accessors[pos].max[2] * 4;
        }

        int vbo_size = 0;
        struct gltf_buffer_view buffer_views[3];
        {
          int i;
          for (i = 0; i < ROA_ARR_COUNT(buffer_views); ++i)
          {
            if(vertex_desc[i] >= 0)
            {
              int attr = vertex_desc[i];
              int buffer_view = gltf.accessors[attr].buffer_view;
              buffer_views[i] = gltf.buffer_views[buffer_view];
              vbo_size += buffer_views[i].byte_length;
            }
          }
        }

        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vbo_size, ROA_NULL, GL_STATIC_DRAW);

        curr_mesh->vbo = vbo;

        {
          int i;
          int vbo_offset = 0;

          for (i = 0; i < ROA_ARR_COUNT(buffer_views); ++i)
          {
            if(vertex_desc[i] >= 0)
            {
              unsigned char *buffer = gltf.buffers[buffer_views[i].buffer].uri_data;
              int byte_offset = vbo_offset;
              int byte_length = buffer_views[i].byte_length;
              void *data = &buffer[buffer_views[i].byte_offset];

              vbo_offset += byte_length;

              glBufferSubData(GL_ARRAY_BUFFER, byte_offset, byte_length, data);
            }
          }
        }
      }

      /* check err */
      {
        GLuint err = glGetError();
        if (err) { ROA_ASSERT(0); }
      }

      /* vert attrs */
      {
        int i;
        int buffer_offset = 0;
        for (i = 0; i < ROA_ARR_COUNT(vertex_desc); ++i)
        {
          if(vertex_desc[i] < 0)
          {
            continue;
          }

          struct gltf_accessor accessor = gltf.accessors[vertex_desc[i]];

          GLenum comp_type = accessor.component_type;
          int comp_size = 0;
          {
            if (accessor.component_type == GLTF_COMPONENT_TYPE_FLOAT) {
              comp_size = sizeof(GLfloat);
            } else if (accessor.component_type == GLTF_COMPONENT_TYPE_UNSIGNED_INT) {
              comp_size = sizeof(GLenum);
            } else {
              ROA_ASSERT(0)
            }
          }

          int components = 0;
          {
            if (accessor.type == GLTF_TYPE_SCALAR)    { components = 1; }
            else if (accessor.type == GLTF_TYPE_VEC2) { components = 2; }
            else if (accessor.type == GLTF_TYPE_VEC3) { components = 3; }
            else if (accessor.type == GLTF_TYPE_VEC4) { components = 4; }
            else { ROA_ASSERT(0); }
          }

          GLsizei stride = components * comp_size;
          GLboolean normalized = accessor.normalized ? GL_TRUE : GL_FALSE;
        
          glEnableVertexAttribArray(i);
          glVertexAttribPointer(
            i,
            components,
            comp_type,
            normalized,
            stride,
            (void*)(buffer_offset));

          struct gltf_buffer_view buffer_view = gltf.buffer_views[accessor.buffer_view];
          buffer_offset = buffer_view.byte_length;
        }
      }

      /* check err */
      {
        GLuint err = glGetError();
        if (err) { ROA_ASSERT(0); }
      }

      /* ibo */
      {
        int accessor_id = mesh->primitives[0].indices;
        int bv_id = gltf.accessors[accessor_id].buffer_view;

        struct gltf_buffer_view buffer_view = gltf.buffer_views[bv_id];
        unsigned char *buffer = gltf.buffers[buffer_view.buffer].uri_data;
     
        GLuint ibo = 0;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
          GL_ELEMENT_ARRAY_BUFFER,
          buffer_view.byte_length,
          &buffer[buffer_view.byte_offset],
          GL_STATIC_DRAW);

        curr_mesh->ibo = ibo;
        curr_mesh->ibo_count = gltf.accessors[accessor_id].count;
        curr_mesh->ibo_index_type = gltf.accessors[accessor_id].component_type;
      }

      /* check err */
      {
        GLuint err = glGetError();
        if (err) { ROA_ASSERT(0); }
      }
    }
  }

  roa_mat4 world, view, proj, wvp;
  {
    /* view */
    roa_mat4_lookat(
      &view,
      roa_float3_set_with_values(distance[0],distance[1],distance[2]),
      roa_float3_zero(),
      roa_float3_set_with_values(0, 1, 0));

    /* proj */
    struct roa_ctx_window_desc win_desc;
    roa_ctx_get_window_desc(hw_ctx, &win_desc);

    float aspect = (float)win_desc.width / (float)win_desc.height;
    roa_mat4_projection(&proj, ROA_QUART_TAU * 0.125f, 0.1, 100.0, aspect);

    /* world */
    roa_mat4_id(&world);

    /* wvp */
    roa_mat4_multiply_three(&wvp, &world, &view, &proj);
  }

  while (roa_ctx_new_frame(hw_ctx))
  {
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    int m;
    for(m = 0; m < mesh_data_count; ++m)
    {
      glBindVertexArray(mesh_data[m].vao);

      GLint uni_wvp = glGetUniformLocation(program, "uni_wvp_mat");
      glUniformMatrix4fv(uni_wvp, 1, GL_FALSE, wvp.data);

      GLint uni_world = glGetUniformLocation(program, "uni_world_mat");
      glUniformMatrix4fv(uni_world, 1, GL_FALSE, world.data);

      glBindBuffer(GL_ARRAY_BUFFER, mesh_data[m].vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[m].ibo);

      glDrawElements(GL_TRIANGLES, mesh_data[m].ibo_count, mesh_data[m].ibo_index_type, 0);
    }
  }

  roa_ctx_destroy(&hw_ctx);

  return 0;
}
