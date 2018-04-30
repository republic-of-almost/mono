#ifdef ROA_RENDERER_API_GL4

#include <graphics_api/platform.h>
#include <GL/gl3w.h>
#include <roa_lib/assert.h>
#include <scratch/glsl.h>
#include <scratch/geometry.h>
#include <stdlib.h>


void
platform_setup(roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  /* VAO */
  {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    ctx->gfx_api.vao = vao;
  }

  /* mesh program */
  {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vertexSource = glsl_fullbright_vs();
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    /* check for errors */
    GLint status = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
      GLchar error[1024];
      GLsizei length = 0;
      glGetShaderInfoLog(
        vertexShader,
        ROA_ARR_COUNT(error),
        &length,
        ROA_ARR_DATA(error));
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fragmentSource = glsl_fullbright_fs();
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "fs_out_fragcolor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    ctx->gfx_api.mesh_program = shaderProgram;
  }

  /* vbo */
  {
    geom_vert_desc vs_desc[] = {
      GEOM_VERT_POSITION3,
      GEOM_NORMAL,
      GEOM_UV
    };

    unsigned count = 0;
    geometry_generate_cube(vs_desc, 3, 1,1,1, ROA_NULL, &count);

    unsigned bytes = sizeof(float) * count;
    float *verts = malloc(bytes);

    geometry_generate_cube(vs_desc, 3, 1,1,1, verts, &count);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bytes, verts, GL_STATIC_DRAW);

    ctx->gfx_api.vbo = vbo;
  }
}


void
platform_destroy(roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);

  /* param check */
  ROA_ASSERT(ctx);
}

#endif /* ROA_RENDERER_API_GL4 */
