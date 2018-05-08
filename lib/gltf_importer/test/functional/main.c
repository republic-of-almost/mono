
#include <roa_ctx/roa_ctx.h>
#include <roa_lib/dir.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <gltf/gltf.h>
#include <GL/gl3w.h>
#include <string.h>
#include <scratch/glsl.h>


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
      for (i = 0; i < vertex_count; ++i)
      {
        /* positions */
        roa_array_push(vbo_data, vbo_raw_data[0][(i * 3) + 0]);
        roa_array_push(vbo_data, vbo_raw_data[0][(i * 3) + 1]);
        roa_array_push(vbo_data, vbo_raw_data[0][(i * 3) + 2]);

        vbo_size += (sizeof(float) * 3);

        /* normal */
        roa_array_push(vbo_data, vbo_raw_data[1][(i * 3) + 0]);
        roa_array_push(vbo_data, vbo_raw_data[1][(i * 3) + 1]);
        roa_array_push(vbo_data, vbo_raw_data[1][(i * 3) + 2]);

        vbo_size += (sizeof(float) * 3);

        /* tex c */
        roa_array_push(vbo_data, vbo_raw_data[2][(i * 2) + 0]);
        roa_array_push(vbo_data, vbo_raw_data[2][(i * 2) + 1]);

        vbo_size += (sizeof(float) * 2);
      }
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vbo_size, vbo_data, GL_STATIC_DRAW);

    /* ibo */

    unsigned int *ibo_raw_data = ROA_NULL;
    int ibo_size = 0;
    int ibo_count = 0;
    {
      int accessor_id = gltf.meshes[0].primitives[0].indices;
      struct gltf_buffer_view buffer_view = gltf.buffer_views[gltf.accessors[accessor_id].buffer_view];
      unsigned char *buffer = gltf.buffers[buffer_view.buffer].uri_data;
      ibo_raw_data = (unsigned int*)buffer[buffer_view.byte_offset];
      ibo_size = buffer_view.byte_length;
      ibo_count = ibo_size / sizeof(unsigned int);
    }

    glGenBuffers(1, ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, ibo_raw_data, GL_STATIC_DRAW);

    /* verts */
    {
      // Specify the layout of the vertex data
      GLint pos_attr = glGetAttribLocation(program, "vs_in_position");
      glEnableVertexAttribArray(pos_attr);
      glVertexAttribPointer(pos_attr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

      GLint tex_coord_attr = glGetAttribLocation(program, "vs_in_texcoord");
      glEnableVertexAttribArray(tex_coord_attr);
      glVertexAttribPointer(tex_coord_attr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    }
  }

  while (roa_ctx_new_frame(hw_ctx))
  {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a rectangle from the 2 triangles using 6 indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  roa_ctx_destroy(&hw_ctx);

  return 0;
}
