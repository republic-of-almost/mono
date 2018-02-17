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


struct volt_gl_cmd_create_texture
{
  volt_gl_cmd_id id;

  volt_texture_t texture;
  volt_texture_desc desc;
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
  GLuint pointer;
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


struct volt_texture
{
  GLuint gl_id;
  GLenum target;
};


struct volt_vbo
{
  GLuint vbo;
  GLuint element_count;
};


struct volt_ibo
{
  GLuint ibo;
  GLuint element_count;
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


struct volt_gl_stream
{
  uint8_t *data;
  unsigned count;
  unsigned capacity;
  unsigned cmd_count;
};


void*
volt_gl_stream_alloc(volt_gl_stream *stream, unsigned bytes)
{
  /* param check */
  ROA_ASSERT(stream);
  ROA_ASSERT(bytes);

  /* beware! This is not aligned, emscripten will fail */
  unsigned new_bytes = bytes + stream->count;

  if (new_bytes < stream->capacity)
  {
    void *ptr = (void*)&stream->data[stream->count];
    stream->count = new_bytes;

    stream->cmd_count += 1;

    return ptr;
  }

  return nullptr;
}

void
volt_gl_stream_clear(volt_gl_stream *stream)
{
  ROA_ASSERT(stream);

  stream->cmd_count = 0;
  stream->count = 0;
}


struct volt_ctx
{
  GLuint vao;

  volt_gl_stream resource_create_stream;
  volt_gl_stream render_stream;
  volt_gl_stream resource_destroy_stream;
  
  /* array */ volt_renderpass_t *renderpasses;
};


struct volt_renderpass
{
  /* next to bind */
  volt_vbo_t curr_vbo;
  volt_ibo_t curr_ibo;
  volt_input_t curr_input;
  volt_program_t curr_program;
  volt_rasterizer_t curr_rasterizer;

  /* last bound */
  volt_vbo_t last_bound_vbo;
  volt_ibo_t last_bound_ibo;
  volt_input_t last_bound_input;
  volt_program_t last_bound_program;
  volt_rasterizer_t last_bound_rasterizer;

  /* array */ volt_gl_stream *render_stream;
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

  /* prepare */
  volt_vbo_t new_vbo = (volt_vbo_t)roa_zalloc(sizeof(*new_vbo));

  *vbo = new_vbo;

  volt_pending_vbo pending{
    new_vbo,
    *desc,
  };

  /* submit cmd */
  volt_gl_stream *stream = &ctx->resource_create_stream;
  volt_gl_cmd_create_vbo *cmd = (volt_gl_cmd_create_vbo*)volt_gl_stream_alloc(stream, sizeof(*cmd));

  cmd->id = volt_gl_cmd_id::create_buffer_vbo;
  cmd->desc = *desc;
  cmd->vbo = *vbo;
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

  /* prepare */
  volt_ibo_t new_ibo = (volt_ibo_t)roa_zalloc(sizeof(*new_ibo));
  
  *ibo = new_ibo;

  volt_pending_ibo pending{
    new_ibo,
    *desc,
  };

  /* submit cmd */
  volt_gl_stream *stream = &ctx->resource_create_stream;
  volt_gl_cmd_create_ibo *cmd = (volt_gl_cmd_create_ibo*)volt_gl_stream_alloc(stream, sizeof(*cmd));

  cmd->id = volt_gl_cmd_id::create_buffer_ibo;
  cmd->desc = *desc;
  cmd->ibo = *ibo;
}


/* ----------------------------------------------------- [ rsrc program ] -- */


void
volt_program_create(
  volt_ctx_t ctx,
  volt_program_t *program,
  struct volt_program_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(program);
  ROA_ASSERT(desc);
  
  /* prepare */
  volt_program_t new_prog = (volt_program_t)roa_alloc(sizeof(*new_prog));
  new_prog->program = 0;

  *program = new_prog;

  volt_pending_program pending{
    new_prog,
    *desc,
  };

  /* submit cmd */
  volt_gl_stream *stream = &ctx->resource_create_stream;
  volt_gl_cmd_create_program *cmd = (volt_gl_cmd_create_program*)volt_gl_stream_alloc(stream, sizeof(*cmd));

  cmd->id = volt_gl_cmd_id::create_program;
  cmd->desc = *desc;
  cmd->program = *program;
}


/* -------------------------------------------------- [ rsrc rasterizer ] -- */


void
volt_rasterizer_create(
  volt_ctx_t ctx,
  volt_rasterizer_t *rasterizer,
  struct volt_rasterizer_desc *desc)
{
  /* param check */
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
  rp->render_stream = &ctx->render_stream;

  *pass = rp;
}


void
volt_renderpass_commit(
  volt_ctx_t ctx,
  volt_renderpass_t *pass)
{
  roa_free(*pass);
  *pass = ROA_NULL;
}


void
volt_renderpass_bind_rasterizer(
  volt_renderpass_t pass,
  volt_rasterizer_t rasterizer)
{
  if (pass->curr_rasterizer != rasterizer)
  {
    pass->curr_rasterizer = rasterizer;
  }
}


void
volt_renderpass_bind_input_format(
  volt_renderpass_t pass,
  volt_input_t input)
{
  if (pass->curr_input != input)
  {
    pass->curr_input = input;
  }
}


void
volt_renderpass_bind_vertex_buffer(
  volt_renderpass_t pass,
  volt_vbo_t vbo)
{
  if (pass->curr_vbo != vbo)
  {
    pass->curr_vbo = vbo;
    pass->last_bound_input = VOLT_NULL;
  }
}


void
volt_renderpass_bind_index_buffer(
  volt_renderpass_t pass,
  volt_ibo_t ibo)
{
  if (pass->curr_ibo != ibo)
  {
    pass->curr_ibo = ibo;
  }
}


void
volt_renderpass_bind_program(
  volt_renderpass_t pass,
  volt_program_t program)
{
  if (pass->curr_program != program)
  {
    pass->curr_program = program;
  }
}


void
volt_renderpass_draw(volt_renderpass_t pass)
{
  /* param */
  ROA_ASSERT(pass);

  /* bind any changes that are required */
  if (pass->curr_vbo != pass->last_bound_vbo)
  {
    /* cmd */
    const GLuint vbo = pass->curr_vbo ? pass->curr_vbo->vbo : 0;

    /* prepare */
    volt_gl_stream *stream = pass->render_stream;
    volt_gl_cmd_bind_vbo *cmd = (volt_gl_cmd_bind_vbo*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::bind_vbo;
    cmd->vbo = vbo;

    pass->last_bound_vbo = pass->curr_vbo;
  }

  if (pass->curr_ibo != pass->last_bound_ibo)
  {
    /* prepare */
    const GLuint ibo = pass->curr_ibo ? pass->curr_ibo->ibo : 0;

    /* cmd */
    volt_gl_stream *stream = pass->render_stream;
    volt_gl_cmd_bind_ibo *cmd = (volt_gl_cmd_bind_ibo*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::bind_ibo;
    cmd->ibo = ibo;

    pass->last_bound_ibo = pass->curr_ibo;
  }

  if (pass->curr_input != pass->last_bound_input)
  {
    if (pass->curr_input)
    {
      int input_count = pass->curr_input->count;

      for (int i = 0; i < input_count; ++i)
      {
        /* prepare */
        GLuint index          = i;
        GLint size            = pass->curr_input->attrib_count[i];
        GLenum type           = GL_FLOAT;
        GLboolean normalized  = GL_FALSE;
        GLsizei stride        = pass->curr_input->full_stride;
        GLuint pointer        = pass->curr_input->increment_stride[i];

        /* cmd */
        volt_gl_stream *stream = pass->render_stream;
        volt_gl_cmd_bind_input *cmd = (volt_gl_cmd_bind_input*)volt_gl_stream_alloc(stream, sizeof(*cmd));

        cmd->id         = volt_gl_cmd_id::bind_input;
        cmd->index      = index;
        cmd->size       = size;
        cmd->type       = type;
        cmd->normalized = normalized;
        cmd->stride     = stride;
        cmd->pointer    = pointer;
      }
    }

    pass->last_bound_input = pass->curr_input;
  }

  if (pass->curr_program != pass->last_bound_program)
  {
    /* prepare */
    const GLuint program = pass->curr_program ? pass->curr_program->program : 0;

    /* cmd */
    volt_gl_stream *stream        = pass->render_stream;
    volt_gl_cmd_bind_program *cmd = (volt_gl_cmd_bind_program*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id      = volt_gl_cmd_id::bind_program;
    cmd->program = program;

    pass->last_bound_program = pass->curr_program;
  }

  if (pass->curr_rasterizer != pass->last_bound_rasterizer)
  {

  }

  /* cmd */
  if(pass->curr_ibo == ROA_NULL)
  {
    volt_gl_stream *stream = pass->render_stream;
    volt_gl_cmd_draw_count *cmd = (volt_gl_cmd_draw_count*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    GLuint count = pass->curr_input->full_stride / pass->curr_vbo->element_count;

    cmd->id     = volt_gl_cmd_id::draw_count;
    cmd->first  = 0;
    cmd->count  = count;
    cmd->mode   = GL_TRIANGLES;
  }
  else
  {
    volt_gl_stream *stream = pass->render_stream;
    volt_gl_cmd_draw_indexed *cmd = (volt_gl_cmd_draw_indexed*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id     = volt_gl_cmd_id::draw_indexed;
    cmd->count  = pass->curr_ibo->element_count;
    cmd->type   = GL_UNSIGNED_INT;
    cmd->mode   = GL_TRIANGLES;
  }
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
  cmd->vbo->element_count = cmd->desc.count;
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
  cmd->ibo->element_count = cmd->desc.count;
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
    const GLenum stage;
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
    glShaderSource(new_shd.gl_id, 1, &new_shd.src, ROA_NULL);
    glCompileShader(new_shd.gl_id);

    /* check for errors */
    GLint status = 0;
    glGetShaderiv(new_shd.gl_id, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
      GLchar error[1024]{};
      GLsizei length = 0;
      glGetShaderInfoLog(
        new_shd.gl_id,
        ROA_ARR_COUNT(error),
        &length,
        ROA_ARR_DATA(error));

      ROA_ASSERT(false);
    }
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
  
  if (status == GL_FALSE)
  {
    GLchar error[1024]{};
    GLsizei length = 0;

    glGetProgramInfoLog(
      program,
      ROA_ARR_COUNT(error),
      &length,
      ROA_ARR_DATA(error));

    ROA_ASSERT(false);
  }

  cmd->program->program = program;
}


static void
volt_gl_create_input(const volt_gl_cmd_create_input *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);
  ROA_ASSERT(cmd->input);
}


static void
volt_gl_create_texture(const volt_gl_cmd_create_texture *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);
  ROA_ASSERT(cmd->texture);

  /* prepare */
  const GLuint width    = cmd->desc.width;
  const GLuint height   = cmd->desc.height;
  const GLvoid *data    = cmd->desc.data;

  GLenum target = GL_TEXTURE_2D;
  GLuint texture  = 0;

  /* create texture */
  glGenTextures(1, &texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(target, texture);
  glTexImage2D(
    target,
    0,
    GL_RGB,
    width,
    height,
    0,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    data);


  if (cmd->desc.mip_maps == VOLT_TRUE)
  {
    glGenerateMipmap(target);
  }

  /* texture wrap mode */
  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  /* texture filter */
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /* save texture */
  cmd->texture->gl_id = texture;
  cmd->texture->target = target;
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
  const GLvoid *pointer = (GLvoid*)cmd->pointer;

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

  glDrawElements(mode, count, type, 0);
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
  new_ctx = (volt_ctx*)roa_zalloc(sizeof(*new_ctx));

  new_ctx->resource_create_stream.data = (uint8_t*)roa_zalloc(sizeof(uint8_t) * 1024);
  new_ctx->resource_create_stream.capacity = sizeof(uint8_t) * 1024;

  new_ctx->resource_destroy_stream.data = (uint8_t*)roa_zalloc(sizeof(uint8_t) * 1024);
  new_ctx->resource_destroy_stream.capacity = sizeof(uint8_t) * 1024;

  new_ctx->render_stream.data = (uint8_t*)roa_zalloc(sizeof(uint8_t) * 1024);
  new_ctx->render_stream.capacity = sizeof(uint8_t) * 1024;

  roa_array_create(new_ctx->renderpasses, 128);

  *ctx = new_ctx;

  glGenVertexArrays(1, &new_ctx->vao);
  glBindVertexArray(new_ctx->vao);
}


void
volt_ctx_destroy(volt_ctx_t *ctx)
{
  /* destroy streams */

  roa_free(*ctx);
}


void
volt_ctx_execute(volt_ctx_t ctx)
{
  ROA_ASSERT(ctx);

  /* create resource stream  */
  {
    const uint8_t *data = ctx->resource_create_stream.data;
    const unsigned bytes = roa_array_size(data);
    unsigned cmd_count = ctx->resource_create_stream.cmd_count;

    while ((cmd_count--) > 0)
    {      
      const volt_gl_cmd_unknown *uk_cmd = (const volt_gl_cmd_unknown*)data;
      ROA_ASSERT(uk_cmd);

      switch (uk_cmd->id)
      {
        case(volt_gl_cmd_id::create_buffer_vbo):
        {
          const volt_gl_cmd_create_vbo *cmd = (const volt_gl_cmd_create_vbo*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_create_vbo(cmd);
          break;
        }
        case(volt_gl_cmd_id::create_buffer_ibo):
        {
          const volt_gl_cmd_create_ibo *cmd = (const volt_gl_cmd_create_ibo*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_create_ibo(cmd);
          break;
        }
        case(volt_gl_cmd_id::create_buffer_input):
        {
          const volt_gl_cmd_create_input *cmd = (const volt_gl_cmd_create_input*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_create_input(cmd);
          break;
        }
        case(volt_gl_cmd_id::create_program):
        {
          const volt_gl_cmd_create_program *cmd = (const volt_gl_cmd_create_program*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_create_program(cmd);
          break;
        }
        default:
          /* only create cmds should be here */
          ROA_ASSERT(false);
      }
    }

    volt_gl_stream_clear(&ctx->resource_create_stream);
  }

  /* execute render stream */
  {
    const uint8_t *data = ctx->render_stream.data;
    const unsigned bytes = roa_array_size(data);
    unsigned cmd_count = ctx->render_stream.cmd_count;

    while ((cmd_count--) > 0)
    {
      const volt_gl_cmd_unknown *uk_cmd = (const volt_gl_cmd_unknown*)data;
      ROA_ASSERT(uk_cmd);

      switch (uk_cmd->id)
      {
        case(volt_gl_cmd_id::bind_vbo):
        {
          const volt_gl_cmd_bind_vbo *cmd = (const volt_gl_cmd_bind_vbo*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_bind_vbo(cmd);
          break;
        }
        case(volt_gl_cmd_id::bind_ibo):
        {
          const volt_gl_cmd_bind_ibo *cmd = (const volt_gl_cmd_bind_ibo*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_bind_ibo(cmd);
          break;
        }
        case(volt_gl_cmd_id::bind_program):
        {
          const volt_gl_cmd_bind_program *cmd = (const volt_gl_cmd_bind_program*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_bind_program(cmd);
          break;
        }
        case(volt_gl_cmd_id::bind_input):
        {
          const volt_gl_cmd_bind_input *cmd = (const volt_gl_cmd_bind_input*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_bind_input(cmd);
          break;
        }
        case(volt_gl_cmd_id::draw_count):
        {
          const volt_gl_cmd_draw_count *cmd = (const volt_gl_cmd_draw_count*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_draw_count(cmd);
          break;
        }
        case(volt_gl_cmd_id::draw_indexed):
        {
          const volt_gl_cmd_draw_indexed *cmd = (const volt_gl_cmd_draw_indexed*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_draw_indexed(cmd);
          break;
        }
        default:
          /* only renderpass cmds should be here */
          ROA_ASSERT(false);
      }
    }

    volt_gl_stream_clear(&ctx->render_stream);
  }

  /* destroy resource stream */
  {
    const uint8_t *next = ctx->resource_destroy_stream.data;
    const unsigned bytes = roa_array_size(next);

  }
}



#endif