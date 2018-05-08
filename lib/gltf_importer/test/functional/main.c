
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

unsigned char *ibo_raw_data;

GLuint vbo;
GLuint ibo;

int
main()
{
  /* setup sys/data */
  {
    roa_ctx_create(&hw_ctx);
    int init = gl3wInit();

    char path[2048];
    ROA_MEM_ZERO(path);

    strcat(path, roa_exe_dir());
    strcat(path, "assets/gltf_test/cube.gltf");

    gltf_import(path, &gltf);
  }

  /* setup gl */
  {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);

    int vertex_desc[3];
    {
      vertex_desc[0] = gltf.meshes[0].primitives->attributes.POSITION;
      vertex_desc[1] = gltf.meshes[0].primitives->attributes.NORMAL;
      vertex_desc[2] = gltf.meshes[0].primitives->attributes.TEXCOORD_0;
    }

    /* program */
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

    GLint err = glGetError();
    ROA_ASSERT(err == 0);

    /* vbo */

    struct gltf_buffer_view buffer_views[3];
    {
      int i;
      for (i = 0; i < ROA_ARR_COUNT(buffer_views); ++i)
      {
        buffer_views[i] = gltf.buffer_views[gltf.accessors[vertex_desc[i]].buffer_view];
      }
    }

    float *vbo_raw_data[3];
    {
      int i;
      for (i = 0; i < ROA_ARR_COUNT(vbo_raw_data); ++i)
      {
        unsigned char *buffer = gltf.buffers[buffer_views[i].buffer].uri_data;
        vbo_raw_data[i] = (float*)&buffer[buffer_views[i].byte_offset];
      }
    }
    
    float *vbo_data = ROA_NULL;
    int vbo_size = 0;
    {
      int vertex_count = gltf.accessors[vertex_desc[0]].count;
      roa_array_create_with_capacity(vbo_data, 1024);

      int i;
      for (i = 0; i < ROA_ARR_COUNT(buffer_views); ++i)
      {
        vbo_size += buffer_views[i].byte_length;
      }

      for (i = 0; i < vertex_count; ++i)
      {
        /* positions */
        roa_array_push(vbo_data, vbo_raw_data[0][(i * 3) + 0]);
        roa_array_push(vbo_data, vbo_raw_data[0][(i * 3) + 1]);
        roa_array_push(vbo_data, vbo_raw_data[0][(i * 3) + 2]);

        /* normal */
        roa_array_push(vbo_data, vbo_raw_data[1][(i * 3) + 0]);
        roa_array_push(vbo_data, vbo_raw_data[1][(i * 3) + 1]);
        roa_array_push(vbo_data, vbo_raw_data[1][(i * 3) + 2]);

        /* tex c */
        roa_array_push(vbo_data, vbo_raw_data[2][(i * 2) + 0]);
        roa_array_push(vbo_data, vbo_raw_data[2][(i * 2) + 1]);
      }
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vbo_size, vbo_data, GL_STATIC_DRAW);

    /* verts */
    {
      // Specify the layout of the vertex data
      GLint pos_attr = glGetAttribLocation(program, "vs_in_position");
      glEnableVertexAttribArray(pos_attr);
      glVertexAttribPointer(pos_attr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

      GLint tex_coord_attr = glGetAttribLocation(program, "vs_in_texcoord");
      glEnableVertexAttribArray(tex_coord_attr);
      glVertexAttribPointer(tex_coord_attr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    }

    /* check err */
    {
      GLuint err = glGetError();
      if (err) { ROA_ASSERT(0); }
    }

    /* ibo */

    ibo_raw_data = ROA_NULL;
    int ibo_size = 0;
    int ibo_count = 0;
    {
      int accessor_id = gltf.meshes[0].primitives[0].indices;
      struct gltf_buffer_view buffer_view = gltf.buffer_views[gltf.accessors[accessor_id].buffer_view];
      unsigned char *buffer = gltf.buffers[buffer_view.buffer].uri_data;
      ibo_raw_data = &buffer[buffer_view.byte_offset];
      ibo_size = buffer_view.byte_length;
      ibo_count = ibo_size / sizeof(unsigned char);
    }

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, ibo_raw_data, GL_STATIC_DRAW);

    /* check err */
    {
      GLuint err = glGetError();
      if (err) { ROA_ASSERT(0); }
    }


    /* check err */
    {
      GLuint err = glGetError();
      if (err) { ROA_ASSERT(0); }
    }
  }

  roa_mat4 view;
  roa_mat4_lookat(&view, roa_float3_set_with_values(1,1,1), roa_float3_zero(), roa_float3_set_with_values(0, 1, 0));

  roa_mat4 proj;
  roa_mat4_projection(&proj, ROA_QUART_TAU * 0.5f, 0.1, 100.0, 800.0 / 480.0);

  roa_mat4 world;
  roa_mat4_id(&world);

  roa_mat4 wvp;
  roa_mat4_multiply_three(&wvp, &world, &view, &proj);

  while (roa_ctx_new_frame(hw_ctx))
  {
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    GLint uni_wvp = glGetUniformLocation(program, "uni_wvp_mat");
    glUniformMatrix4fv(uni_wvp, 1, GL_FALSE, wvp.data);

    GLint uni_world = glGetUniformLocation(program, "uni_world_mat");
    glUniformMatrix4fv(uni_world, 1, GL_FALSE, world.data);

    // Draw a rectangle from the 2 triangles using 6 indices
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
  }

  roa_ctx_destroy(&hw_ctx);

  return 0;
}
