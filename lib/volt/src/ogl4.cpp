#define VOLT_OGL4

#ifdef VOLT_OGL4

#include <volt/volt.h>
#include <GL/gl3w.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <stdio.h>
#include <cstdint>


/* ------------------------------------------------------ [ gl commands ] -- */


enum class volt_gl_cmd_id
{
  unknown,

  /* resource */

  create_program,
  create_buffer_ibo,
  create_buffer_vbo,
  create_buffer_input,
  
  /* renderpass */

  bind_program,
  bind_vbo,
  bind_ibo,
  bind_input,

  draw_count,
  draw_indexed,
};


struct volt_gl_cmd_unknown
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::unknown */
};


/* ------------------------------------------ [ gl resource cmd structs ] -- */


struct volt_gl_cmd_create_program
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::create_program */

  volt_program_t program;
  volt_program_desc desc;
};


struct volt_gl_cmd_create_vbo
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::create_buffer_vbo */

  volt_vbo_t vbo;
  volt_vbo_desc desc;
};


struct volt_gl_cmd_create_ibo
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::create_buffer_ibo */

  volt_ibo_t ibo;
  volt_ibo_desc desc;
};


struct volt_gl_cmd_create_input
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::create_buffer_input */

  volt_input_t input;
  volt_input_desc desc;
};


/* ---------------------------------------- [ gl renderpass cmd structs ] -- */


struct volt_gl_cmd_bind_vbo
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::bind_vbo */

  GLuint vbo;
};


struct volt_gl_cmd_bind_ibo
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::bind_ibo */

  GLuint ibo;
};


struct volt_gl_cmd_bind_input
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::bind_input */

  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  const GLvoid *pointer;
};


struct volt_gl_cmd_bind_program
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::create_program */

  GLuint program;
};


struct volt_gl_cmd_draw_count
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::draw_count */

  GLenum mode;
  GLint first;
  GLsizei count;
};


struct volt_gl_cmd_draw_indexed
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::draw_indexed */

  GLenum mode;
  GLenum type;
  GLuint count;
  const GLvoid *indices;
};


/* ---------------------------------------------- [ internal gl structs ] -- */


struct volt_program
{
  GLuint program;
};


struct volt_rasterizer
{
  GLenum prim;
  GLenum winding_order;
  GLenum culling_mode;
};


struct volt_vbo
{
  GLuint vbo;
};


struct volt_ibo
{
  GLuint ibo;
};


struct volt_texture
{
  GLuint tex;
};


struct volt_input
{
  GLuint increment_stride[12];
  GLint attrib_count[12];
  GLsizei full_stride;
  GLuint count;
};


struct volt_draw_call
{
  GLuint program;
  GLuint vbo;
  GLuint ibo;
  GLint texture[3];

  volt_input input_fmt;
};


struct volt_pending_vbo {
  volt_vbo_t vbo;
  volt_vbo_desc desc;
};


struct volt_pending_ibo {
  volt_ibo_t ibo;
  volt_ibo_desc desc;
};


struct volt_pending_program {
  volt_program_t program;
  volt_program_desc desc;
};


struct volt_ctx
{
  GLuint vao;

  /* buffer */ uint8_t *resource_create_stream;
  /* buffer */ uint8_t *render_stream;
  /* buffer */ uint8_t *resource_destroy_stream;

  /* array */ struct volt_pending_vbo *pending_create_vbo_desc;
  /* array */ struct volt_pending_ibo *pending_create_ibo_desc;
  /* array */ struct volt_pending_program *pending_create_program_desc;
  
  /* array */ volt_renderpass_t *renderpasses;
};


struct volt_renderpass
{
  /* cache */
  volt_vbo_t curr_vbo;
  volt_ibo_t curr_ibo;
  volt_input_t curr_input;
  volt_program_t curr_program;

  /* array */ uint8_t *render_stream;
};


/* ------------------------------------------------------- [ rsrc input ] -- */


void
volt_input_create(
  volt_ctx_t ctx,
  volt_input_t *input,
  struct volt_input_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(input);
  ROA_ASSERT(desc);

  volt_input_t new_input = (volt_input_t)roa_alloc(sizeof(*new_input));
  
  unsigned max_count = ROA_ARR_COUNT(new_input->increment_stride);
  unsigned cpy_count = desc->count > max_count ? max_count : desc->count;

  new_input->count = cpy_count;
  new_input->full_stride = 0;

  for (int i = 0; i < cpy_count; ++i)
  {
    unsigned attrib_count;

    switch (desc->attributes[i])
    {
      case(VOLT_INPUT_FLOAT4): { attrib_count = 4; break; }
      case(VOLT_INPUT_FLOAT3): { attrib_count = 3; break; }
      case(VOLT_INPUT_FLOAT2): { attrib_count = 2; break; }
      case(VOLT_INPUT_FLOAT):  { attrib_count = 1; break; }
    }

    new_input->attrib_count[i] = attrib_count;
    new_input->increment_stride[i] = new_input->full_stride;

    new_input->full_stride += new_input->attrib_count[i] * sizeof(float);
  }

  *input = new_input;
}


/* --------------------------------------------------------- [ rsrc vbo ] -- */


void
volt_vertex_buffer_create(
  volt_ctx_t ctx,
  volt_vbo_t *vbo,
  struct volt_vbo_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(vbo);
  ROA_ASSERT(desc);

  volt_vbo_t new_vbo = (volt_vbo_t)roa_zalloc(sizeof(*new_vbo));

  *vbo = new_vbo;

  volt_pending_vbo pending{
    new_vbo,
    *desc,
  };

  /* store desc as pending type */
  roa_array_push(ctx->pending_create_vbo_desc, pending);
}

/* --------------------------------------------------------- [ rsrc ibo ] -- */


void
volt_index_buffer_create(
  volt_ctx_t ctx,
  volt_ibo_t *ibo,
  struct volt_ibo_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(ibo);
  ROA_ASSERT(desc);

  volt_ibo_t new_ibo = (volt_ibo_t)roa_zalloc(sizeof(*new_ibo));
  
  *ibo = new_ibo;

  volt_pending_ibo pending{
    new_ibo,
    *desc,
  };

  /* store desc as pending ibo */
  roa_array_push(ctx->pending_create_ibo_desc, pending);
}


/* ----------------------------------------------------- [ rsrc program ] -- */


void
volt_program_create(
  volt_ctx_t ctx,
  volt_program_t *program,
  struct volt_program_desc *desc)
{
  ROA_ASSERT(ctx);
  ROA_ASSERT(program);
  ROA_ASSERT(desc);
  
  volt_program_t new_prog = (volt_program_t)roa_alloc(sizeof(*new_prog));
  new_prog->program = 0;

  *program = new_prog;

  volt_pending_program pending{
    new_prog,
    *desc,
  };

  /* store desc as pending type */
  roa_array_push(ctx->pending_create_program_desc, pending);
}


/* -------------------------------------------------- [ rsrc rasterizer ] -- */


void
volt_rasterizer_create(
  volt_ctx_t ctx,
  volt_rasterizer_t *rasterizer,
  struct volt_rasterizer_desc *desc)
{
  ROA_ASSERT(ctx);
  ROA_ASSERT(rasterizer);
  ROA_ASSERT(desc);
}


/* ------------------------------------------------------- [ renderpass ] -- */


void
volt_renderpass_create(
  volt_ctx_t ctx,
  volt_renderpass_t *pass)
{
  volt_renderpass_t rp = (volt_renderpass_t)roa_zalloc(sizeof(*rp));

  roa_array_create(rp->draw_calls, 128);
  roa_array_push(rp->draw_calls, volt_draw_call{});

  *pass = rp;
}


void
volt_renderpass_commit(
  volt_ctx_t ctx,
  volt_renderpass_t *pass)
{
  unsigned size = roa_array_size((*pass)->draw_calls);

  if(size > 0)
  {
    roa_array_erase((*pass)->draw_calls, size - 1);
  }

  roa_array_push(ctx->renderpasses, *pass);
  *pass = ROA_NULL;
}


void
volt_renderpass_bind_rasterizer(
  volt_renderpass_t pass,
  volt_rasterizer_t rasterizer)
{

}


void
volt_renderpass_bind_input_format(
  volt_renderpass_t pass,
  volt_input_t input)
{
  pass->curr_input = input;
}


void
volt_renderpass_bind_vertex_buffer(
  volt_renderpass_t pass,
  volt_vbo_t vbo)
{
  pass->curr_vbo = vbo;
}


void
volt_renderpass_bind_index_buffer(
  volt_renderpass_t pass,
  volt_ibo_t ibo)
{
  pass->curr_ibo = ibo;
}


void
volt_renderpass_bind_program(
  volt_renderpass_t pass,
  volt_program_t program)
{
  pass->curr_program = program;
}


void
volt_renderpass_draw(volt_renderpass_t pass)
{
  /* param */
  ROA_ASSERT(pass);

  /* build the draw call */
  volt_draw_call *dc = roa_array_back(pass->draw_calls);

  ROA_ASSERT(pass->curr_program);

  if(pass->curr_program)
  {
    dc->program = pass->curr_program->program;
  }

  if(pass->curr_vbo)
  {
    ROA_ASSERT(pass->curr_input);

    dc->input_fmt = *pass->curr_input;
    dc->vbo = pass->curr_vbo->vbo;
  }

  if(pass->curr_ibo)
  {
    ROA_ASSERT(pass->curr_vbo);

    dc->ibo = pass->curr_ibo->ibo;
  }

  roa_array_push(pass->draw_calls, volt_draw_call{});
}


/* --------------------------------------- [ gl cmd create rsrc actions ] -- */


static void
volt_gl_create_vbo(const volt_gl_cmd_create_vbo *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);
  ROA_ASSERT(cmd->vbo);

  /* prepare */
  GLuint vbo{};
  const GLsizeiptr size = sizeof(float) * cmd->desc.count;
  const GLvoid *data = cmd->desc.data;
  const GLenum useage = GL_STATIC_DRAW;
  const GLenum target = GL_ARRAY_BUFFER;

  /* create vbo */
  glGenBuffers(1, &vbo);
  glBindBuffer(target, vbo);
  glBufferData(target, size, data, useage);

  ROA_ASSERT_PEDANTIC(vbo > 0);

  /* save vbo */
  cmd->vbo->vbo = vbo;
}


static void
volt_gl_create_ibo(const volt_gl_cmd_create_ibo *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);
  ROA_ASSERT(cmd->ibo);

  /* prepare */
  GLuint ibo{};
  const GLsizeiptr size = sizeof(float) * cmd->desc.count;
  const GLvoid *data = cmd->desc.data;
  const GLenum useage = GL_STATIC_DRAW;
  const GLenum target = GL_ELEMENT_ARRAY_BUFFER;

  /* create vbo */
  glGenBuffers(1, &ibo);
  glBindBuffer(target, ibo);
  glBufferData(target, size, data, useage);

  ROA_ASSERT_PEDANTIC(ibo > 0);

  /* save vbo */
  cmd->ibo->ibo = ibo;
}


static void
volt_gl_create_program(const volt_gl_cmd_create_program *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);
  ROA_ASSERT(cmd->program);

  /* prepare */
  struct shader_inout
  {
    GLenum stage;
    GLuint gl_id;
    const GLchar *src;
  };

  shader_inout shaders[VOLT_SHD_STAGE_COUNT] = {
    { GL_VERTEX_SHADER , 0, nullptr },
    { GL_GEOMETRY_SHADER , 0, nullptr },
    { GL_FRAGMENT_SHADER , 0, nullptr },
  };

  for (int j = 0; j < cmd->desc.stage_count; ++j)
  {
    const volt_program_desc *desc = &cmd->desc;

    const volt_shader_stage stage = desc->shader_stages_type[j];
    shaders[stage].src = desc->shader_stages_src[j];
  }

  /* create shader stages */
  for (shader_inout &new_shd : shaders)
  {
    if (new_shd.src == nullptr)
    {
      continue;
    }

    new_shd.gl_id = glCreateShader(new_shd.stage);
    glShaderSource(new_shd.stage, 1, &new_shd.src, NULL);
    glCompileShader(new_shd.stage);

    /* check for errors */
    GLint status = 0;
    glGetShaderiv(new_shd.stage, GL_COMPILE_STATUS, &status);

    GLchar error[1024]{};
    glGetShaderInfoLog(new_shd.stage, 1024, NULL, error);

    ROA_ASSERT(status == GL_TRUE);
  }

  /* create program */
  GLuint program = glCreateProgram();

  for (const shader_inout &new_shd : shaders)
  {
    if (new_shd.gl_id)
    {
      glAttachShader(program, new_shd.gl_id);
    }
  }

  glBindFragDataLocation(program, 0, "outColor");
  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  ROA_ASSERT(status == GL_TRUE);

  cmd->program->program = program;
}


static void
volt_gl_create_input(const volt_gl_cmd_create_input *cmd)
{
  ROA_ASSERT(cmd);
}


/* ---------------------------------------- [ gl cmd renderpass actions ] -- */


static void
volt_gl_bind_vbo(const volt_gl_cmd_bind_vbo *cmd)
{
  ROA_ASSERT_PEDANTIC(cmd);
  ROA_ASSERT_PEDANTIC(cmd->vbo);

  const GLenum target = GL_ARRAY_BUFFER;
  const GLuint buffer = cmd->vbo;

  glBindBuffer(target, buffer);
}


static void
volt_gl_bind_ibo(const volt_gl_cmd_bind_ibo *cmd)
{
  ROA_ASSERT_PEDANTIC(cmd);
  ROA_ASSERT_PEDANTIC(cmd->ibo);

  const GLenum target = GL_ELEMENT_ARRAY_BUFFER;
  const GLuint buffer = cmd->ibo;

  glBindBuffer(target, buffer);
}


static void
volt_gl_bind_program(const volt_gl_cmd_bind_program *cmd)
{
  ROA_ASSERT_PEDANTIC(cmd);
  ROA_ASSERT_PEDANTIC(cmd->program);

  const GLuint program = cmd->program;

  glUseProgram(program);
}


static void
volt_gl_bind_input(const volt_gl_cmd_bind_input *cmd)
{
  ROA_ASSERT_PEDANTIC(cmd);

  const GLuint index = cmd->index;
  const GLint size = cmd->size;
  const GLenum type = cmd->type;
  const GLboolean normalized = cmd->normalized;
  const GLsizei stride = cmd->stride;
  const GLvoid *pointer = cmd->pointer;

  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}


static void
volt_gl_draw_count(const volt_gl_cmd_draw_count *cmd)
{
  ROA_ASSERT_PEDANTIC(cmd);

  const GLenum mode = cmd->mode;
  const GLint first = cmd->first;
  const GLsizei count = cmd->count;

  glDrawArrays(mode, first, count);
}


static void
volt_gl_draw_indexed(const volt_gl_cmd_draw_indexed *cmd)
{
  ROA_ASSERT_PEDANTIC(cmd);

  const GLenum mode = cmd->mode;
  const GLsizei count = cmd->count;
  const GLenum type = cmd->type;
  const GLvoid *indices = cmd->indices;

  glDrawElements(mode, count, type, indices);
}


/* -------------------------------------- [ gl cmd destroy rsrc actions ] -- */



/* --------------------------------------------------------- [ lifetime ] -- */


void
volt_ctx_create(volt_ctx_t *ctx)
{
  if (gl3wInit())
  {
    /* fail */
    return;
  }

  gl3wIsSupported(1, 1);

  struct volt_ctx *new_ctx = nullptr;
  new_ctx = (volt_ctx*)roa_zalloc(sizeof(new_ctx[0]));

  roa_array_create(new_ctx->renderpasses, 128);
  roa_array_create(new_ctx->pending_create_vbo_desc, 32);
  roa_array_create(new_ctx->pending_create_vbo_desc, 32);
  roa_array_create(new_ctx->pending_create_program_desc, 32);

  *ctx = new_ctx;

  glGenVertexArrays(1, &new_ctx->vao);
  glBindVertexArray(new_ctx->vao);
}


void
volt_ctx_destroy(volt_ctx_t *ctx)
{
  roa_array_destroy((*ctx)->pending_create_program_desc);
  roa_array_destroy((*ctx)->pending_create_vbo_desc);
  roa_array_destroy((*ctx)->renderpasses);

  roa_free(*ctx);
}


void
volt_ctx_execute(volt_ctx_t ctx)
{
  ROA_ASSERT(ctx);

  /* create resource stream  */
  {
    const uint8_t *next = ctx->resource_create_stream;
    const unsigned bytes = roa_array_size(next);

    while (next < next + bytes)
    {
      const volt_gl_cmd_unknown *uk_cmd = (const volt_gl_cmd_unknown*)next;
      ROA_ASSERT(uk_cmd);

      switch (uk_cmd->id)
      {
        case(volt_gl_cmd_id::create_buffer_vbo):
        {
          const volt_gl_cmd_create_vbo *cmd = (const volt_gl_cmd_create_vbo*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_create_vbo(cmd);
          break;
        }
        case(volt_gl_cmd_id::create_buffer_ibo):
        {
          const volt_gl_cmd_create_ibo *cmd = (const volt_gl_cmd_create_ibo*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_create_ibo(cmd);
          break;
        }
        case(volt_gl_cmd_id::create_buffer_input):
        {
          const volt_gl_cmd_create_input *cmd = (const volt_gl_cmd_create_input*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_create_input(cmd);
          break;
        }
        case(volt_gl_cmd_id::create_program):
        {
          const volt_gl_cmd_create_program *cmd = (const volt_gl_cmd_create_program*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_create_program(cmd);
          break;
        }
        default:
          /* only create cmds should be here */
          ROA_ASSERT(false);
      }
    }
  }

  /* execute render stream */
  {
    const uint8_t *next = ctx->render_stream;
    const unsigned bytes = roa_array_size(next);

    while (next < next + bytes)
    {
      const volt_gl_cmd_unknown *uk_cmd = (const volt_gl_cmd_unknown*)next;
      ROA_ASSERT(uk_cmd);

      switch (uk_cmd->id)
      {
        case(volt_gl_cmd_id::bind_vbo):
        {
          const volt_gl_cmd_bind_vbo *cmd = (const volt_gl_cmd_bind_vbo*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_bind_vbo(cmd);
          break;
        }
        case(volt_gl_cmd_id::bind_ibo):
        {
          const volt_gl_cmd_bind_ibo *cmd = (const volt_gl_cmd_bind_ibo*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_bind_ibo(cmd);
          break;
        }
        case(volt_gl_cmd_id::bind_program):
        {
          const volt_gl_cmd_bind_program *cmd = (const volt_gl_cmd_bind_program*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_bind_program(cmd);
          break;
        }
        case(volt_gl_cmd_id::bind_input):
        {
          const volt_gl_cmd_bind_input *cmd = (const volt_gl_cmd_bind_input*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_bind_input(cmd);
          break;
        }
        case(volt_gl_cmd_id::draw_count):
        {
          const volt_gl_cmd_draw_count *cmd = (const volt_gl_cmd_draw_count*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_draw_count(cmd);
          break;
        }
        case(volt_gl_cmd_id::draw_indexed):
        {
          const volt_gl_cmd_draw_indexed *cmd = (const volt_gl_cmd_draw_indexed*)uk_cmd;
          next += sizeof(*cmd);
          volt_gl_draw_indexed(cmd);
          break;
        }
        default:
          /* only renderpass cmds should be here */
          ROA_ASSERT(false);
      }
    }
  }

  /* destroy resource stream */
  {
    const uint8_t *next = ctx->resource_destroy_stream;
    const unsigned bytes = roa_array_size(next);

  }
}



#endif