
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


GLuint vbo;
GLuint ibo;

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
    strcat(path, "assets/gltf_test/cube.gltf");

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
    int vertex_desc[3];
    {
      /* we need to now this up front to build vbo and be able to enable attrs */
      {
        vertex_desc[0] = gltf.meshes[0].primitives->attributes.POSITION;
        vertex_desc[1] = gltf.meshes[0].primitives->attributes.NORMAL;
        vertex_desc[2] = gltf.meshes[0].primitives->attributes.TEXCOORD_0;
      }

      int vbo_size = 0;
      struct gltf_buffer_view buffer_views[3];
      {
        int i;
        for (i = 0; i < ROA_ARR_COUNT(buffer_views); ++i)
        {
          buffer_views[i] = gltf.buffer_views[gltf.accessors[vertex_desc[i]].buffer_view];
          vbo_size += buffer_views[i].byte_length;
        }
      }

      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, vbo_size, ROA_NULL, GL_STATIC_DRAW);

      {
        int i;
        int vbo_offset = 0;

        for (i = 0; i < ROA_ARR_COUNT(buffer_views); ++i)
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
      int accessor_id = gltf.meshes[0].primitives[0].indices;
      int bv_id = gltf.accessors[accessor_id].buffer_view;

      struct gltf_buffer_view buffer_view = gltf.buffer_views[bv_id];
      unsigned char *buffer = gltf.buffers[buffer_view.buffer].uri_data;
    
      glGenBuffers(1, &ibo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        buffer_view.byte_length,
        &buffer[buffer_view.byte_offset],
        GL_STATIC_DRAW);
    }

    /* check err */
    {
      GLuint err = glGetError();
      if (err) { ROA_ASSERT(0); }
    }
  }

  roa_mat4 world, view, proj, wvp;
  {
    /* view */
    roa_mat4_lookat(
      &view,
      roa_float3_set_with_values(1,1,1),
      roa_float3_zero(),
      roa_float3_set_with_values(0, 1, 0));

    /* proj */
    struct roa_ctx_window_desc win_desc;
    roa_ctx_get_window_desc(hw_ctx, &win_desc);

    float aspect = (float)win_desc.width / (float)win_desc.height;
    roa_mat4_projection(&proj, ROA_QUART_TAU * 0.25f, 0.1, 100.0, aspect);

    /* world */
    roa_mat4_id(&world);

    /* wvp */
    roa_mat4_multiply_three(&wvp, &world, &view, &proj);
  }

  while (roa_ctx_new_frame(hw_ctx))
  {
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    GLint uni_wvp = glGetUniformLocation(program, "uni_wvp_mat");
    glUniformMatrix4fv(uni_wvp, 1, GL_FALSE, wvp.data);

    GLint uni_world = glGetUniformLocation(program, "uni_world_mat");
    glUniformMatrix4fv(uni_world, 1, GL_FALSE, world.data);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
  }

  roa_ctx_destroy(&hw_ctx);

  return 0;
}
