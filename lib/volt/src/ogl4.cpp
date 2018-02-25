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


/* ----------------------------------------------------------- [ common ] -- */


/* https://stackoverflow.com/questions/2535284/how-can-i-hash-a-string-to-an-int-using-c#13487193 */
/* test this, attribute or remove */
uint64_t
hash(const char *name)
{
  uint64_t hash = 5381;

  int c;
  while (c = *name++)
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}


#define GL_ASSERT ROA_ASSERT(glGetError() == 0)


/* ------------------------------------------------------ [ gl commands ] -- */


enum class volt_gl_cmd_id
{
  unknown,

  /* resource */

  create_program,
  create_uniform,
  create_buffer_ibo,
  create_buffer_vbo,
  create_buffer_input,
  create_buffer_texture,
  
  /* renderpass */

  bind_program,
  bind_vbo,
  bind_ibo,
  bind_input,
  bind_texture,
  bind_uniform,

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


struct volt_gl_cmd_create_uniform
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::create_uniform */

  volt_uniform_t uniform;
  volt_uniform_desc desc;
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


struct volt_gl_cmd_bind_texture
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::bind_texture */
  
  GLuint gl_id;
  GLint active_texture;
  GLint sampler_location;
};


struct volt_gl_cmd_bind_uniform
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::bind_uniform */

  GLint location;
  GLvoid *value;
  GLsizei count;
  GLenum type;
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
  GLuint indices;
};


/* ---------------------------------------------- [ internal gl structs ] -- */


struct volt_program
{
  GLuint program;

  struct volt_program_sampler
  {
    GLenum type;
    GLint size;
    GLint location;
  };

  uint64_t sampler_keys[16];
  volt_program_sampler samplers[16];
  unsigned sampler_count;

  struct volt_program_uniform
  {
    GLenum type;
    GLint size;
    GLint location;
  };

  uint64_t uniform_keys[32];
  volt_program_uniform uniforms[32];
  unsigned uniform_count;
  
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
  GLenum format;
};


struct volt_uniform
{
  GLvoid *value;
  GLenum type;
  GLsizei count;
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
  GLsizei byte_stride;
  GLuint element_stride;
  GLuint count;
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

  /* samplers */
  uint64_t sampler_hash[32];
  volt_texture_t sampler[32];

  /* uniforms */
  uint64_t uniform_hash[32];
  volt_uniform_t uniform[32];

  /* last bound */
  volt_vbo_t last_bound_vbo;
  volt_ibo_t last_bound_ibo;
  volt_input_t last_bound_input;
  volt_program_t last_bound_program;
  volt_rasterizer_t last_bound_rasterizer;

  /* array */ volt_gl_stream *render_stream;
};


/* ----------------------------------------------------- [ rsrc texture ] -- */


void
volt_texture_create(
  volt_ctx_t ctx,
  volt_texture_t *texture,
  volt_texture_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(texture);
  ROA_ASSERT(desc);

  /* prepare */
  volt_texture_t new_texture = (volt_texture_t)roa_zalloc(sizeof(*new_texture));

  *texture = new_texture;

  /* submit cmd */
  volt_gl_stream *stream = &ctx->resource_create_stream;
  volt_gl_cmd_create_texture *cmd = (volt_gl_cmd_create_texture*)volt_gl_stream_alloc(stream, sizeof(*cmd));

  cmd->id      = volt_gl_cmd_id::create_buffer_texture;
  cmd->desc    = *desc;
  cmd->texture = *texture;
}


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
  new_input->byte_stride = 0;
  new_input->element_stride = 0;

  for (unsigned i = 0; i < cpy_count; ++i)
  {
    unsigned attrib_count;

    switch (desc->attributes[i])
    {
      case(VOLT_INPUT_FLOAT4): { attrib_count = 4; break; }
      case(VOLT_INPUT_FLOAT3): { attrib_count = 3; break; }
      case(VOLT_INPUT_FLOAT2): { attrib_count = 2; break; }
      case(VOLT_INPUT_FLOAT):  { attrib_count = 1; break; }

      default:
      {
        ROA_ASSERT(false);
        attrib_count = 1;
      }
    }

    new_input->attrib_count[i] = attrib_count;
    new_input->increment_stride[i] = new_input->byte_stride;

    new_input->byte_stride += new_input->attrib_count[i] * sizeof(float);
    new_input->element_stride += new_input->attrib_count[i];
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

  /* submit cmd */
  volt_gl_stream *stream = &ctx->resource_create_stream;
  volt_gl_cmd_create_vbo *cmd = (volt_gl_cmd_create_vbo*)volt_gl_stream_alloc(stream, sizeof(*cmd));

  cmd->id   = volt_gl_cmd_id::create_buffer_vbo;
  cmd->desc = *desc;
  cmd->vbo  = *vbo;
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
  volt_program_t new_prog = (volt_program_t)roa_zalloc(sizeof(*new_prog));
  new_prog->program = 0;

  *program = new_prog;

  /* submit cmd */
  volt_gl_stream *stream = &ctx->resource_create_stream;
  volt_gl_cmd_create_program *cmd = (volt_gl_cmd_create_program*)volt_gl_stream_alloc(stream, sizeof(*cmd));

  cmd->id = volt_gl_cmd_id::create_program;
  cmd->desc = *desc;
  cmd->program = *program;
}


/* ----------------------------------------------------- [ rsrc uniform ] -- */


void
volt_uniform_create(
  volt_ctx_t ctx,
  volt_uniform_t *uniform,
  struct volt_uniform_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(uniform);
  ROA_ASSERT(desc);
  
  /* prepare */
  volt_uniform_t new_rsrc = (volt_uniform_t)roa_zalloc(sizeof(*new_rsrc));

  *uniform = new_rsrc;

  /* submit cmd */
  volt_gl_stream *stream = &ctx->resource_create_stream;
  volt_gl_cmd_create_uniform *cmd = (volt_gl_cmd_create_uniform*)volt_gl_stream_alloc(stream, sizeof(*cmd));

  cmd->id = volt_gl_cmd_id::create_uniform;
  cmd->desc = *desc;
  cmd->uniform = *uniform;
}


void
volt_uniform_update(
  volt_ctx_t ctx,
  volt_uniform_t uniform,
  void *data)
{
  ROA_UNUSED(ctx);

  uniform->value = data;
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
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(pass);

  /* free */
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
volt_renderpass_bind_texture_buffer(
  volt_renderpass_t pass,
  volt_texture_t texture,
  const char *sampler_name)
{
  const uint64_t hash_name = hash(sampler_name);

  /* check to see if its already bound */
  const unsigned sampler_slot_count = ROA_ARRAY_COUNT(pass->sampler_hash);

  for (int i = 0; i < sampler_slot_count; ++i)
  {
    if (pass->sampler_hash[i] == hash_name)
    {
      /* already bound */
      return;
    }
  }

  /* add if not bound */
  for (int i = 0; i < sampler_slot_count; ++i)
  {
    if (pass->sampler_hash[i] == 0)
    {
      /* bound */
      pass->sampler_hash[i] = hash_name;
      pass->sampler[i] = texture;

      return;
    }
  }

  /* all slots bound */
  ROA_ASSERT(false);
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
volt_renderpass_bind_uniform(
  volt_renderpass_t pass,
  volt_uniform_t uniform,
  const char *uniform_name)
{
  const uint64_t hash_name = hash(uniform_name);

  /* check to see if its already bound */
  const unsigned uniform_slot_count = ROA_ARRAY_COUNT(pass->uniform_hash);

  for (int i = 0; i < uniform_slot_count; ++i)
  {
    if (pass->uniform_hash[i] == hash_name)
    {
      /* already bound */
      return;
    }
  }

  /* add if not bound */
  for (int i = 0; i < uniform_slot_count; ++i)
  {
    if (pass->uniform_hash[i] == 0)
    {
      /* bound */
      pass->uniform_hash[i] = hash_name;
      pass->uniform[i] = uniform;

      return;
    }
  }

  /* all slots bound */
  ROA_ASSERT(false);
}


void
volt_renderpass_draw(volt_renderpass_t pass)
{
  /* param */
  ROA_ASSERT(pass);

  if (pass->curr_program != pass->last_bound_program)
  {
    /* prepare */
    const GLuint program = pass->curr_program ? pass->curr_program->program : 0;

    /* cmd */
    volt_gl_stream *stream = pass->render_stream;
    volt_gl_cmd_bind_program *cmd = (volt_gl_cmd_bind_program*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::bind_program;
    cmd->program = program;

    pass->last_bound_program = pass->curr_program;
  }

  if (pass->curr_input != pass->last_bound_input)
  {
    if (pass->curr_input)
    {
      int input_count = pass->curr_input->count;

      for (int i = 0; i < input_count; ++i)
      {
        /* prepare */
        GLuint index = i;
        GLint size = pass->curr_input->attrib_count[i];
        GLenum type = GL_FLOAT;
        GLboolean normalized = GL_FALSE;
        GLsizei stride = pass->curr_input->byte_stride;
        GLuint pointer = pass->curr_input->increment_stride[i];

        /* cmd */
        volt_gl_stream *stream = pass->render_stream;
        volt_gl_cmd_bind_input *cmd = (volt_gl_cmd_bind_input*)volt_gl_stream_alloc(stream, sizeof(*cmd));

        cmd->id = volt_gl_cmd_id::bind_input;
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

  if (pass->curr_rasterizer != pass->last_bound_rasterizer)
  {

  }

  /* textures */
  {
    /* todo! don't rebind textures */

    /* check to see if textures have been bound */
    unsigned sampler_count = pass->curr_program->sampler_count;
    GLint active_textures = 0;

    for(unsigned i = 0; i < sampler_count; ++i)
    {
      const uint64_t prog_sampler_hash = pass->curr_program->sampler_keys[i];

      /* check bound textures */
      for (int j = 0; j < ROA_ARRAY_COUNT(pass->sampler_hash); ++j)
      {
        const uint64_t bound_sampler_hash = pass->sampler_hash[j];

        if (bound_sampler_hash == prog_sampler_hash)
        {
          /* bind texture */
          volt_gl_stream *stream = pass->render_stream;
          volt_gl_cmd_bind_texture *cmd = (volt_gl_cmd_bind_texture*)volt_gl_stream_alloc(stream, sizeof(*cmd));

          cmd->id = volt_gl_cmd_id::bind_texture;
          cmd->gl_id = pass->sampler[j]->gl_id;
          cmd->active_texture = active_textures;
          cmd->sampler_location = pass->curr_program->samplers[i].location;

          active_textures += 1;
        }
      }
    }
  }

  /* uniforms */
  {
    /* todo! don't rebind uniforms */

    /* check to see if uniforms have been bound */
    unsigned uniform_count = pass->curr_program->uniform_count;

    for (unsigned i = 0; i < uniform_count; ++i)
    {
      const uint64_t prog_uniform_hash = pass->curr_program->uniform_keys[i];

      /* check bound uniforms */
      for (int j = 0; j < ROA_ARRAY_COUNT(pass->uniform_hash); ++j)
      {
        const uint64_t bound_hash = pass->uniform_hash[j];

        if (bound_hash == prog_uniform_hash)
        {
          /* bind uniform */
          volt_gl_stream *stream = pass->render_stream;
          volt_gl_cmd_bind_uniform *cmd = (volt_gl_cmd_bind_uniform*)volt_gl_stream_alloc(stream, sizeof(*cmd));

          /* must check types */
          cmd->id         = volt_gl_cmd_id::bind_uniform;
          cmd->location   = pass->curr_program->uniforms[i].location;
          cmd->value      = pass->uniform[i]->value;
          cmd->type       = pass->uniform[i]->type;
          cmd->count      = pass->uniform[i]->count;
        }
      }
    }
  }

  /* cmd */
  if(pass->curr_ibo == ROA_NULL)
  {
    volt_gl_stream *stream = pass->render_stream;
    volt_gl_cmd_draw_count *cmd = (volt_gl_cmd_draw_count*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    const GLuint stride     = pass->curr_input->element_stride;
    const GLuint ele_count  = pass->curr_vbo->element_count;
    const GLuint count      = ele_count / stride;

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
  const GLvoid *data    = cmd->desc.data;
  const GLenum useage   = GL_STATIC_DRAW;
  const GLenum target   = GL_ARRAY_BUFFER;

  /* create vbo */
  glGenBuffers(1, &vbo);
  glBindBuffer(target, vbo);
  glBufferData(target, size, data, useage);

  ROA_ASSERT_PEDANTIC(vbo > 0);

  /* save vbo */
  cmd->vbo->vbo = vbo;
  cmd->vbo->element_count = cmd->desc.count;
  
  GL_ASSERT;
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
  const GLvoid *data    = cmd->desc.data;
  const GLenum useage   = GL_STATIC_DRAW;
  const GLenum target   = GL_ELEMENT_ARRAY_BUFFER;

  /* create vbo */
  glGenBuffers(1, &ibo);
  glBindBuffer(target, ibo);
  glBufferData(target, size, data, useage);

  ROA_ASSERT_PEDANTIC(ibo > 0);

  /* save vbo */
  cmd->ibo->ibo = ibo;
  cmd->ibo->element_count = cmd->desc.count;

  GL_ASSERT;
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

  for (unsigned j = 0; j < cmd->desc.stage_count; ++j)
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

  /* get uniforms and samplers */
  if (status == GL_TRUE)
  {
    GLint uniform_count;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

    /* more uniforms than space*/
    /* increase space or dynamic allocation */
    ROA_ASSERT(ROA_ARRAY_COUNT(cmd->program->uniforms) > uniform_count);

    GLint data_count = 0;
    GLint samp_count = 0;

    GLchar name_buffer[1024];

    for (int i = 0; i < uniform_count; ++i)
    {
      memset(name_buffer, 0, sizeof(name_buffer));

      GLint size;
      GLenum type;
      GLsizei length;

      glGetActiveUniform(
        program,
        i,
        ROA_ARRAY_COUNT(name_buffer),
        &length,
        &size,
        &type,
        ROA_ARRAY_PTR(name_buffer));

      /* users text will hash incorrectly */
      /* increase buffer or dynamic allocations */
      ROA_ASSERT(ROA_ARRAY_COUNT(name_buffer) > length);

      uint64_t uni_hash = hash(ROA_ARRAY_PTR(name_buffer));

      /* seperate samplers and data */
      if ((type >= GL_SAMPLER_1D) && (type <= GL_SAMPLER_2D_SHADOW))
      {
        cmd->program->sampler_keys[samp_count] = uni_hash;
        cmd->program->samplers[samp_count].location = i;
        cmd->program->samplers[samp_count].size = size;
        cmd->program->samplers[samp_count].type = type;

        samp_count += 1;
      }
      else
      {
        cmd->program->uniform_keys[data_count] = uni_hash;
        cmd->program->uniforms[data_count].location = i;
        cmd->program->uniforms[data_count].size = size;
        cmd->program->uniforms[data_count].type = type;

        data_count +=1;
      }
    }

    /* save count */
    cmd->program->sampler_count = samp_count;
    cmd->program->uniform_count = data_count;
  }

  GL_ASSERT;
}


static void
volt_gl_create_input(const volt_gl_cmd_create_input *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);
  ROA_ASSERT(cmd->input);

  GL_ASSERT;
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
  const GLint level     = 0;
  const GLenum type     = GL_UNSIGNED_BYTE;
  const GLint border    = 0;
  const GLenum target   = GL_TEXTURE_2D;
  GLuint texture        = 0;
  GLint internal_format = GL_RGB;
  GLenum format         = GL_RGB;

  switch (cmd->desc.format)
  {
    case(VOLT_COLOR_RGB):
    {
      internal_format = GL_RGB;
      format          = GL_RGB;
      break;
    }
    case(VOLT_COLOR_RGBA):
    {
      internal_format = GL_RGBA;
      format          = GL_RGBA;
      break;
    }
  }

  /* create texture */
  glGenTextures(1, &texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(target, texture);
  glTexImage2D(
    target,
    level,
    internal_format,
    width,
    height,
    border,
    format,
    type,
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

  GL_ASSERT;
}


static void
volt_gl_create_uniform(const volt_gl_cmd_create_uniform *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);

  GLenum type = 0;

  switch (cmd->desc.data_type)
  {
    case(VOLT_DATA_FLOAT): type = GL_FLOAT; break;
    case(VOLT_DATA_FLOAT2): type = GL_FLOAT_VEC2; break;
    case(VOLT_DATA_FLOAT3): type = GL_FLOAT_VEC3; break;
    case(VOLT_DATA_FLOAT4): type = GL_FLOAT_VEC4; break;
    case(VOLT_DATA_MAT2): type = GL_FLOAT_MAT2; break;
    case(VOLT_DATA_MAT3): type = GL_FLOAT_MAT3; break;
    case(VOLT_DATA_MAT4): type = GL_FLOAT_MAT4; break;
    default:
      ROA_ASSERT(false);
  }

  cmd->uniform->value = NULL;
  cmd->uniform->type = type;
  cmd->uniform->count = (GLuint)cmd->desc.count;

  GL_ASSERT;
}


/* ---------------------------------------- [ gl cmd renderpass actions ] -- */


static void
volt_gl_bind_vbo(const volt_gl_cmd_bind_vbo *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);
  ROA_ASSERT_PEDANTIC(cmd->vbo);

  /* prepare */
  const GLenum target = GL_ARRAY_BUFFER;
  const GLuint buffer = cmd->vbo;

  /* bind */
  glBindBuffer(target, buffer);

  GL_ASSERT;
}


static void
volt_gl_bind_ibo(const volt_gl_cmd_bind_ibo *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);
  ROA_ASSERT_PEDANTIC(cmd->ibo);

  /* prepare */
  const GLenum target = GL_ELEMENT_ARRAY_BUFFER;
  const GLuint buffer = cmd->ibo;

  /* bind */
  glBindBuffer(target, buffer);

  GL_ASSERT;
}


static void
volt_gl_bind_program(const volt_gl_cmd_bind_program *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);
  ROA_ASSERT_PEDANTIC(cmd->program);

  /* prepare */
  const GLuint program = cmd->program;

  /* bind program */
  glUseProgram(program);

  GL_ASSERT;
}


static void
volt_gl_bind_input(const volt_gl_cmd_bind_input *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);

  /* prepare */
  const GLuint index          = cmd->index;
  const GLint size            = cmd->size;
  const GLenum type           = cmd->type;
  const GLboolean normalized  = cmd->normalized;
  const GLsizei stride        = cmd->stride;
  const GLvoid *pointer       = (GLvoid*)cmd->pointer;

  /* enable vertex attribute */
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);

  GL_ASSERT;
}


static void
volt_gl_bind_texture(const volt_gl_cmd_bind_texture *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);

  /* prepare */
  const GLenum gl_id          = cmd->gl_id;
  const GLuint texture_offset = cmd->active_texture;
  const GLenum texture_slot   = GL_TEXTURE0 + texture_offset;
  const GLenum target         = GL_TEXTURE_2D;
  const GLint location        = cmd->sampler_location;

  /* bind */
  glActiveTexture(texture_slot);
  glBindTexture(target, gl_id);
  glUniform1i(location, texture_offset);

  GL_ASSERT;
}


static void
volt_gl_bind_uniform(const volt_gl_cmd_bind_uniform *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);

  /* prepare */
  const GLenum location     = cmd->location;
  const GLenum data_type    = cmd->type;
  const GLsizei count       = cmd->count;
  const GLvoid *value       = cmd->value;
  const GLboolean transpose = GL_FALSE;

  /* bind */
  switch(data_type)
  {
    case(GL_FLOAT): glUniform1fv(location, count, (GLfloat*)value); break;
    case(GL_FLOAT_VEC2): glUniform2fv(location, count, (GLfloat*)value); break;
    case(GL_FLOAT_VEC3): glUniform3fv(location, count, (GLfloat*)value); break;
    case(GL_FLOAT_VEC4): glUniform4fv(location, count, (GLfloat*)value); break;

    case(GL_FLOAT_MAT2): glUniformMatrix2fv(location, 1, transpose, (GLfloat*)value); break;
    case(GL_FLOAT_MAT3): glUniformMatrix3fv(location, 1, transpose, (GLfloat*)value); break;
    case(GL_FLOAT_MAT4): glUniformMatrix4fv(location, 1, transpose, (GLfloat*)value); break;
      
    case(GL_INT): glUniform1iv(location, count, (GLint*)value); break;
    case(GL_INT_VEC2): glUniform2iv(location, count, (GLint*)value); break;
    case(GL_INT_VEC3): glUniform3iv(location, count, (GLint*)value); break;
    case(GL_INT_VEC4): glUniform4iv(location, count, (GLint*)value); break;
  }

  GL_ASSERT;
}


static void
volt_gl_draw_count(const volt_gl_cmd_draw_count *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);

  /* prepare */
  const GLenum mode   = cmd->mode;
  const GLint first   = cmd->first;
  const GLsizei count = cmd->count;

  /* draw */
  glDrawArrays(mode, first, count);

  GL_ASSERT;
}


static void
volt_gl_draw_indexed(const volt_gl_cmd_draw_indexed *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);

  /* prepare */
  const GLenum mode     = cmd->mode;
  const GLsizei count   = cmd->count;
  const GLenum type     = cmd->type;
  const GLuint indices  = cmd->indices;

  /* draw */
  glDrawElements(mode, count, type, (GLvoid*)indices);

  GL_ASSERT;
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

  const GLubyte *version  = glGetString(GL_VERSION);
  const GLubyte *vendor   = glGetString(GL_VENDOR);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *glsl_ver = glGetString(GL_SHADING_LANGUAGE_VERSION);

  printf("GL: %s , %s, %s, %s\n", (const char*)version, (const char*)vendor, (const char *)renderer, (const char *)glsl_ver);
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
  
  auto err = glGetError(); /* clear msgs */
  ROA_ASSERT(err == 0);

  glClearColor(0.5, 0, 0, 1);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glDisable(GL_CULL_FACE);
  
  glViewport(0, 0, 800, 480);
  glScissor(0, 0, 800, 480);
  //glFrontFace(GL_CCW);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_NONE);

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
        case(volt_gl_cmd_id::create_buffer_texture):
        {
          const volt_gl_cmd_create_texture *cmd = (const volt_gl_cmd_create_texture*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_create_texture(cmd);
          break;
        }
        case(volt_gl_cmd_id::create_uniform):
        {
          const volt_gl_cmd_create_uniform *cmd = (const volt_gl_cmd_create_uniform*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_create_uniform(cmd);
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
        case(volt_gl_cmd_id::bind_texture):
        {
          const volt_gl_cmd_bind_texture *cmd = (const volt_gl_cmd_bind_texture*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_bind_texture(cmd);
          break;
        }
        case(volt_gl_cmd_id::bind_uniform):
        {
          const volt_gl_cmd_bind_uniform *cmd = (const volt_gl_cmd_bind_uniform*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_bind_uniform(cmd);
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