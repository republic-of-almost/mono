#if defined(VOLT_OGL4) && VOLT_OGL4 == 1


#include <volt/volt.h>
#include <GL/gl3w.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/hash.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <roa_lib/atomic.h>
#include <roa_lib/spin_lock.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


/* ----------------------------------------------------------- [ config ] -- */


#define GL_LOG_CMDS 0
#define GL_ASSERT_ON_ERRORS 0
#define GL_LOG_EXEC_ERRORS 0


/* ----------------------------------------------------------- [ common ] -- */


#define GL_ASSERT do { auto err = glGetError(); if(err) { printf("GLERR: %d\n", err); ROA_ASSERT(0); } } while(0)


/* ------------------------------------------------------ [ gl commands ] -- */


enum class volt_gl_cmd_id
{
  unknown,

  /* resource */

  create_program,
  create_uniform,
  create_buffer_ibo,
  create_buffer_vbo,
  update_buffer_vbo,
  create_buffer_input,
  create_buffer_texture,
  update_buffer_texture,
  create_buffer_framebuffer,

  /* renderpass */

  bind_program,
  bind_vbo,
  bind_ibo,
  bind_input,
  bind_texture,
  bind_uniform,
  bind_framebuffer,

  set_viewport,

  /* draw */

  draw_count,
  draw_indexed,
};


const char*
volt_gl_cmd_to_string(volt_gl_cmd_id id)
{
  switch (id)
  {
    case(volt_gl_cmd_id::create_program):               return "create_program";
    case(volt_gl_cmd_id::create_uniform):               return "create_uniform";
    case(volt_gl_cmd_id::create_buffer_ibo):            return "create_buffer_ibo";
    case(volt_gl_cmd_id::create_buffer_vbo):            return "create_buffer_vbo";
    case(volt_gl_cmd_id::update_buffer_vbo):            return "update_buffer_vbo";
    case(volt_gl_cmd_id::create_buffer_input):          return "create_buffer_input";
    case(volt_gl_cmd_id::create_buffer_texture):        return "create_buffer_texture";
    case(volt_gl_cmd_id::create_buffer_framebuffer):    return "create_buffer_framebuffer";
    case(volt_gl_cmd_id::bind_program):                 return "bind_program";
    case(volt_gl_cmd_id::bind_vbo):                     return "bind_vbo";
    case(volt_gl_cmd_id::bind_ibo):                     return "bind_ibo";
    case(volt_gl_cmd_id::bind_input):                   return "bind_input";
    case(volt_gl_cmd_id::bind_texture):                 return "bind_texture";
    case(volt_gl_cmd_id::bind_uniform):                 return "bind_uniform";
    case(volt_gl_cmd_id::bind_framebuffer):             return "bind_framebuffer";
    case(volt_gl_cmd_id::set_viewport):                 return "set_viewport";
    case(volt_gl_cmd_id::draw_count):                   return "draw_count";
    case(volt_gl_cmd_id::draw_indexed):                 return "draw_indexed";
    default:                                            return "unknown";
  }
}


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


struct volt_gl_cmd_update_vbo
{
  volt_gl_cmd_id id; /* volt_gl_cmd_id::update_buffer_vbo */

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


struct volt_gl_cmd_update_texture
{
  volt_gl_cmd_id id;

  volt_texture_t texture;
  volt_texture_desc desc;
};


struct volt_gl_cmd_create_framebuffer
{
  volt_gl_cmd_id id;

  volt_framebuffer_t framebuffer;
  volt_framebuffer_desc desc;
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



struct volt_gl_cmd_bind_framebuffer
{
  volt_gl_cmd_id id;
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


struct volt_gl_cmd_set_viewport
{
  volt_gl_cmd_id id;

  GLint x;
  GLint y;
  GLsizei width;
  GLsizei height;
};


/* ---------------------------------------------- [ internal gl structs ] -- */


struct volt_program
{
  GLuint gl_id;

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


struct volt_viewport
{
  GLint x;
  GLint y;
  GLuint width;
  GLuint height;
};


struct volt_texture
{
  GLuint gl_id;
  GLenum target;
  GLenum format;
  GLuint width;
  GLuint height;
  GLenum sampling;
  GLenum dimention;
  GLboolean mips;
  GLvoid *data;
};


struct volt_framebuffer
{
  int unk;
};


struct volt_uniform
{
  GLvoid *value;
  GLenum type;
  GLsizei count;
};


struct volt_vbo
{
  GLuint gl_id;
  GLuint element_count;
};


struct volt_ibo
{
  GLuint gl_id;
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

  roa_atomic_int rsrc_create_lock;
  volt_gl_stream resource_create_stream;

  roa_atomic_int rdr_lock;
  volt_gl_stream render_stream;

  roa_atomic_int rsrc_destroy_lock;
  volt_gl_stream resource_destroy_stream;

  roa_atomic_int rp_lock;
  /* array */ volt_renderpass_t *renderpasses;

  volt_log_callback_fn log_callback;
};


struct volt_renderpass
{
  /* next to bind */
  volt_vbo_t curr_vbo;
  volt_ibo_t curr_ibo;
  volt_input_t curr_input;
  volt_program_t curr_program;
  volt_rasterizer_t curr_rasterizer;
  volt_viewport curr_viewport;

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


/* ------------------------------------------------------- [ gl to volt ] -- */


GLboolean
convert_volt_boolean_to_gl(VOLT_BOOL boolean)
{
  switch (boolean)
  {
    case(VOLT_TRUE): return GL_TRUE;
    case(VOLT_FALSE): return GL_FALSE;
  }

  ROA_ASSERT(false);
  return GL_FALSE;
}


VOLT_BOOL
convert_gl_boolean_to_volt(GLboolean boolean)
{
  switch (boolean)
  {
    case(GL_TRUE): return VOLT_TRUE;
    case(GL_FALSE): return VOLT_FALSE;
  }

  ROA_ASSERT(false);
  return VOLT_FALSE;
}

GLenum
convert_gl_format_to_gl_attachment(GLenum fmt)
{
  switch (fmt)
  {
    case(GL_DEPTH32F_STENCIL8): return GL_DEPTH_STENCIL_ATTACHMENT;
    case(GL_DEPTH_COMPONENT): return GL_DEPTH_ATTACHMENT;
  }

  ROA_ASSERT(false);
  return GL_NONE;
}

GLenum
convert_volt_format_to_gl_type(volt_pixel_format fmt)
{
  switch(fmt)
  {
    case(VOLT_PIXEL_FORMAT_RGB): return GL_UNSIGNED_BYTE;
    case(VOLT_PIXEL_FORMAT_RGBA): return GL_UNSIGNED_BYTE;
    case(VOLT_PIXEL_FORMAT_RGBA32F): return GL_FLOAT;
    case(VOLT_PIXEL_FORMAT_DEPTH32F): return GL_FLOAT;
    case(VOLT_PIXEL_FORMAT_DEPTH32F_STENCIL8): GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
  }
}

GLenum
convert_volt_format_to_gl_format(volt_pixel_format fmt)
{
  switch (fmt)
  {
    case(VOLT_PIXEL_FORMAT_RGB): return GL_RGB;
    case(VOLT_PIXEL_FORMAT_RGBA): return GL_RGBA;
    case(VOLT_PIXEL_FORMAT_RGBA32F): return GL_RGBA;
    case(VOLT_PIXEL_FORMAT_DEPTH32F): return GL_DEPTH_COMPONENT;
    case(VOLT_PIXEL_FORMAT_DEPTH32F_STENCIL8): return GL_DEPTH32F_STENCIL8;
  }

  ROA_ASSERT(false);
  return GL_NONE;
};

GLint
convert_volt_format_to_gl_internal_format(volt_pixel_format fmt)
{
  switch (fmt)
  {
    case(VOLT_PIXEL_FORMAT_RGB): return GL_RGB;
    case(VOLT_PIXEL_FORMAT_RGBA): return GL_RGBA;
    case(VOLT_PIXEL_FORMAT_RGBA32F): return GL_RGBA32F;
    case(VOLT_PIXEL_FORMAT_DEPTH32F): return GL_DEPTH_COMPONENT32F;
    case(VOLT_PIXEL_FORMAT_DEPTH32F_STENCIL8): return GL_DEPTH32F_STENCIL8;
  }

  ROA_ASSERT(false);
  return GL_NONE;
};


volt_pixel_format
convert_gl_format_to_volt(GLenum fmt)
{
  switch (fmt)
  {
    case(GL_RGB): return VOLT_PIXEL_FORMAT_RGB;
    case(GL_RGBA): return VOLT_PIXEL_FORMAT_RGBA;
    case(GL_RGBA32F): return VOLT_PIXEL_FORMAT_RGBA32F;
    case(GL_DEPTH_COMPONENT32F): return VOLT_PIXEL_FORMAT_DEPTH32F;
  }

  ROA_ASSERT(false);
  return VOLT_PIXEL_FORMAT_RGBA;
}


GLenum
convert_volt_tex_dimention_to_gl_target(volt_texture_dimentions di)
{
  switch (di)
  {
    case(VOLT_TEXTURE_2D): return GL_TEXTURE_2D;
  }

  ROA_ASSERT(false);
  return GL_TEXTURE_2D;
}


volt_texture_dimentions
convert_gl_dimention_to_volt(GLenum di)
{
  switch (di)
  {
    case(GL_TEXTURE_2D): return VOLT_TEXTURE_2D;
  }

  ROA_ASSERT(false);
  return VOLT_TEXTURE_2D;
}


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
  {
    roa_spin_lock_aquire(&ctx->rsrc_create_lock);

    volt_gl_stream *stream = &ctx->resource_create_stream;
    volt_gl_cmd_create_texture *cmd = (volt_gl_cmd_create_texture*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id      = volt_gl_cmd_id::create_buffer_texture;
    cmd->desc    = *desc;
    cmd->texture = *texture;

    roa_spin_lock_release(&ctx->rsrc_create_lock);
  }
}


void
volt_texture_update(
  volt_ctx_t ctx,
  volt_texture_t texture,
  volt_texture_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(texture);
  ROA_ASSERT(desc);

  /* submit cmd */
  {
    roa_spin_lock_aquire(&ctx->rsrc_create_lock);

    volt_gl_stream *stream = &ctx->resource_create_stream;
    volt_gl_cmd_update_texture *cmd = (volt_gl_cmd_update_texture*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::update_buffer_texture;
    cmd->desc = *desc;
    cmd->texture = texture;

    roa_spin_lock_release(&ctx->rsrc_create_lock);
  }
}


void
volt_texture_get_desc(
  volt_ctx_t ctx,
  volt_texture_t texture,
  struct volt_texture_desc *out_desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(texture);
  ROA_ASSERT(out_desc);

  /* convert gl to volt */
  out_desc->mip_maps    = convert_gl_boolean_to_volt(texture->mips);
  out_desc->dimentions  = convert_gl_dimention_to_volt(texture->dimention);
  out_desc->width       = texture->width;
  out_desc->height      = texture->height;
  out_desc->sampling    = VOLT_SAMPLING_BILINEAR;
  out_desc->data        = texture->data;
  out_desc->access      = texture->data ? VOLT_STREAM : VOLT_STATIC;
  out_desc->format      = convert_gl_format_to_volt(texture->format);
}


/* ------------------------------------------------- [ rsrc framebuffer ] -- */


void
volt_framebuffer_create(
  volt_ctx_t ctx,
  volt_framebuffer_t *fbo,
  struct volt_framebuffer_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(fbo);
  ROA_ASSERT(desc);

  /* prepare */
  volt_framebuffer_t new_fbo = (volt_framebuffer_t)roa_zalloc(sizeof(*new_fbo));

  *fbo = new_fbo;

  /* submit cmd */
  {
    roa_spin_lock_aquire(&ctx->rsrc_create_lock);

    volt_gl_stream *stream = &ctx->resource_create_stream;
    volt_gl_cmd_create_framebuffer *cmd = (volt_gl_cmd_create_framebuffer*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::create_buffer_framebuffer;
    cmd->desc = *desc;
    cmd->framebuffer = *fbo;

    roa_spin_lock_release(&ctx->rsrc_create_lock);
  }
}


volt_resource_status
volt_framebuffer_status(
  volt_ctx_t ctx,
  volt_framebuffer_t)
{
  return VOLT_RSRC_ERROR;
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

  /* prepare */
  volt_input_t new_input = (volt_input_t)roa_alloc(sizeof(*new_input));

  unsigned max_count = ROA_ARR_COUNT(new_input->increment_stride);
  unsigned cpy_count = desc->count > max_count ? max_count : desc->count;

  new_input->count = cpy_count;
  new_input->byte_stride = 0;
  new_input->element_stride = 0;

  /* create - no cmd needed */
  {
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
}


volt_resource_status
volt_input_status(
  volt_ctx_t ctx,
  volt_input_t input)
{
  if (input)
  {
    if (input->count > 0)
    {
      return VOLT_RSRC_VALID;
    }

    return VOLT_RSRC_PENDING_CREATE;
  }

  return VOLT_RSRC_ERROR;
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
  {
    roa_spin_lock_aquire(&ctx->rsrc_create_lock);

    volt_gl_stream *stream = &ctx->resource_create_stream;
    volt_gl_cmd_create_vbo *cmd = (volt_gl_cmd_create_vbo*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id   = volt_gl_cmd_id::create_buffer_vbo;
    cmd->desc = *desc;
    cmd->vbo  = *vbo;

    roa_spin_lock_release(&ctx->rsrc_create_lock);
  }
}


volt_resource_status
volt_vertex_buffer_status(
  volt_ctx_t ctx,
  volt_vbo_t vbo)
{
  if (vbo)
  {
    if (vbo->gl_id)
    {
      return VOLT_RSRC_VALID;
    }

    return VOLT_RSRC_PENDING_CREATE;
  }

  return VOLT_RSRC_ERROR;
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
  {
    roa_spin_lock_aquire(&ctx->rsrc_create_lock);

    volt_gl_stream *stream = &ctx->resource_create_stream;
    volt_gl_cmd_create_ibo *cmd = (volt_gl_cmd_create_ibo*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::create_buffer_ibo;
    cmd->desc = *desc;
    cmd->ibo = *ibo;

    roa_spin_lock_release(&ctx->rsrc_create_lock);
  }
}


volt_resource_status
volt_index_buffer_status(
  volt_ctx_t ctx,
  volt_ibo_t ibo)
{
  if (ibo)
  {
    if (ibo->gl_id)
    {
      return VOLT_RSRC_VALID;
    }

    return VOLT_RSRC_PENDING_CREATE;
  }

  return VOLT_RSRC_ERROR;
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

  *program = new_prog;

  /* submit cmd */
  {
    roa_spin_lock_aquire(&ctx->rsrc_create_lock);

    volt_gl_stream *stream = &ctx->resource_create_stream;
    volt_gl_cmd_create_program *cmd = (volt_gl_cmd_create_program*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::create_program;
    cmd->desc = *desc;
    cmd->program = *program;

    roa_spin_lock_release(&ctx->rsrc_create_lock);
  }
}


volt_resource_status
volt_program_buffer_status(
  volt_ctx_t ctx,
  volt_program_t program)
{
  if (program)
  {
    if (program->gl_id)
    {
      return VOLT_RSRC_VALID;
    }

    return VOLT_RSRC_PENDING_CREATE;
  }

  return VOLT_RSRC_ERROR;
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
  {
    roa_spin_lock_aquire(&ctx->rsrc_create_lock);

    volt_gl_stream *stream = &ctx->resource_create_stream;
    volt_gl_cmd_create_uniform *cmd = (volt_gl_cmd_create_uniform*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::create_uniform;
    cmd->desc = *desc;
    cmd->uniform = *uniform;


    roa_spin_lock_release(&ctx->rsrc_create_lock);
  }
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
  volt_renderpass_t *pass,
  const char *pass_name,
  volt_framebuffer_t target)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(pass);

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
  const uint64_t hash_name = roa_hash(sampler_name);

  /* check to see if its already bound */
  const unsigned sampler_slot_count = ROA_ARR_COUNT(pass->sampler_hash);

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
  const uint64_t hash_name = roa_hash(uniform_name);

  /* check to see if its already bound */
  const unsigned uniform_slot_count = ROA_ARR_COUNT(pass->uniform_hash);

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
volt_renderpass_set_viewport(
  volt_renderpass_t pass,
  int x,
  int y,
  unsigned width,
  unsigned height)
{
  ROA_ASSERT(pass);

  volt_viewport vp{x,y,width,height};

  pass->curr_viewport = vp;
}


void
volt_renderpass_draw(volt_renderpass_t pass)
{
  /* param */
  ROA_ASSERT(pass);

  volt_gl_stream *stream = pass->render_stream;

  /* viewport */
  {
    volt_gl_cmd_set_viewport *cmd = (volt_gl_cmd_set_viewport*)volt_gl_stream_alloc(stream, sizeof(*cmd));

    cmd->id = volt_gl_cmd_id::set_viewport;
    cmd->x = pass->curr_viewport.x;
    cmd->y = pass->curr_viewport.y;
    cmd->width = pass->curr_viewport.width;
    cmd->height = pass->curr_viewport.height;
  }

  if (pass->curr_program != pass->last_bound_program)
  {
    /* prepare */
    const GLuint program = pass->curr_program ? pass->curr_program->gl_id : 0;

    /* cmd */
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
    const GLuint vbo = pass->curr_vbo ? pass->curr_vbo->gl_id : 0;

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
    const GLuint ibo = pass->curr_ibo ? pass->curr_ibo->gl_id : 0;

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
      for (int j = 0; j < ROA_ARR_COUNT(pass->sampler_hash); ++j)
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
      for (int j = 0; j < ROA_ARR_COUNT(pass->uniform_hash); ++j)
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
          cmd->value      = pass->uniform[j]->value;
          cmd->type       = pass->uniform[j]->type;
          cmd->count      = pass->uniform[j]->count;
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
  cmd->vbo->gl_id = vbo;
  cmd->vbo->element_count = cmd->desc.count;

  GL_ASSERT;
}


static void
volt_gl_update_vbo(const volt_gl_cmd_update_vbo *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);
  ROA_ASSERT(cmd->vbo);
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
  cmd->ibo->gl_id = ibo;
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

  cmd->program->gl_id = program;

  /* get uniforms and samplers */
  if (status == GL_TRUE)
  {
    GLint uniform_count;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

    /* more uniforms than space*/
    /* increase space or dynamic allocation */
    ROA_ASSERT(ROA_ARR_COUNT(cmd->program->uniforms) > uniform_count);

    GLint data_count = 0;
    GLint samp_count = 0;

    GLchar name_buffer[1024];

    for (int i = 0; i < uniform_count; ++i)
    {
			ROA_MEM_ZERO(name_buffer);

      GLint size;
      GLenum type;
      GLsizei length;

      glGetActiveUniform(
        program,
        i,
        ROA_ARR_COUNT(name_buffer),
        &length,
        &size,
        &type,
        ROA_ARR_DATA(name_buffer));

      /* users text will hash incorrectly */
      /* increase buffer or dynamic allocations */
      ROA_ASSERT(ROA_ARR_COUNT(name_buffer) > length);

      uint64_t uni_hash = roa_hash(ROA_ARR_DATA(name_buffer));

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
  GLvoid *data          = cmd->desc.data;
  const GLint level     = 0;
  const GLenum type     = convert_volt_format_to_gl_type(cmd->desc.format);
  const GLint border    = 0;
  const GLenum target   = convert_volt_tex_dimention_to_gl_target(cmd->desc.dimentions);
  const GLint in_format = convert_volt_format_to_gl_internal_format(cmd->desc.format);
  const GLenum format   = convert_volt_format_to_gl_format(cmd->desc.format);
  const GLboolean mips  = convert_volt_boolean_to_gl(cmd->desc.mip_maps);

  GLuint texture        = 0;

  /* create texture */
  glGenTextures(1, &texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(target, texture);
  glTexImage2D(
    target,
    level,
    in_format,
    width,
    height,
    border,
    format,
    type,
    data);

  /* mips if desired */
  if (mips == GL_TRUE)
  {
    glGenerateMipmap(target);
  }

  /* set a debug name if one provided */
  if(cmd->desc.name)
  {
    glObjectLabel(GL_TEXTURE, texture, -1, cmd->desc.name);
  }

  /* texture wrap mode */
  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  /* texture filter */
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /* save texture */
  cmd->texture->gl_id     = texture;
  cmd->texture->target    = target;
  cmd->texture->dimention = target;
  cmd->texture->format    = format;
  cmd->texture->mips      = mips;
  cmd->texture->width     = width;
  cmd->texture->height    = height;
  cmd->texture->data      = cmd->desc.access == VOLT_STREAM ? data : VOLT_NULL;

  GL_ASSERT;
}


static void
volt_gl_update_texture(const volt_gl_cmd_update_texture *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);
  ROA_ASSERT(cmd->texture);

  /* prepare */
  const GLuint width = cmd->desc.width;
  const GLuint height = cmd->desc.height;
  const GLint level = 0;
  const GLint border = 0;
  GLvoid *data = cmd->desc.data;
  const GLenum type = convert_volt_format_to_gl_type(cmd->desc.format);
  const GLenum target = convert_volt_tex_dimention_to_gl_target(cmd->desc.dimentions);
  const GLint internal_format = convert_volt_format_to_gl_internal_format(cmd->desc.format);
  const GLenum format = convert_volt_format_to_gl_format(cmd->desc.format);
  const GLboolean mips = convert_volt_boolean_to_gl(cmd->desc.mip_maps);

  glBindTexture(target, cmd->texture->gl_id);

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
}


static void
volt_gl_create_framebuffer(const volt_gl_cmd_create_framebuffer *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);

  /* create fbo */
  GLuint frame_buffer = 0;
  glGenFramebuffers(1, &frame_buffer);

  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

  GLenum draw_buffers[16];

  int i;
  int attach_count = cmd->desc.attachment_count;

  for(i = 0; i < attach_count; ++i)
  {
    GLuint color_buffer = cmd->desc.attachments[i]->gl_id;

    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0 + i,
      GL_TEXTURE_2D,
      color_buffer,
      0
    );

    draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
  }

  ROA_ASSERT(attach_count < ROA_ARR_COUNT(draw_buffers));

  int draw_buf_count = attach_count > ROA_ARR_COUNT(draw_buffers) ? ROA_ARR_COUNT(draw_buffers) : attach_count;

  glDrawBuffers(draw_buf_count, draw_buffers);

  if(cmd->desc.depth)
  {
    GLuint depth_buffer = cmd->desc.depth->gl_id;

    GLenum attachment = convert_gl_format_to_gl_attachment(cmd->desc.depth->format);

    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      attachment,
      GL_TEXTURE_2D,
      depth_buffer,
      0);
  }

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  ROA_ASSERT(status == GL_FRAMEBUFFER_COMPLETE);

  GL_ASSERT;
}


static void
volt_gl_create_uniform(const volt_gl_cmd_create_uniform *cmd)
{
  /* param check */
  ROA_ASSERT(cmd);

  /* prepare */
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


static void
volt_gl_set_viewport(const volt_gl_cmd_set_viewport *cmd)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(cmd);

  /* prepare */
  const GLint x = cmd->x;
  const GLint y = cmd->y;
  const GLsizei width = cmd->width;
  const GLsizei height = cmd->height;

  /* set vp */
  glViewport(x, y, width, height);
}


/* -------------------------------------- [ gl cmd destroy rsrc actions ] -- */



/* --------------------------------------------------------- [ lifetime ] -- */


void
volt_ctx_create(volt_ctx_t *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  if (gl3wInit())
  {
    /* fail */
    return;
  }

  gl3wIsSupported(4, 0);

  struct volt_ctx *new_ctx = nullptr;
  new_ctx = (volt_ctx*)roa_zalloc(sizeof(*new_ctx));

  roa_spin_lock_init(&new_ctx->rsrc_create_lock);
  new_ctx->resource_create_stream.data = (uint8_t*)roa_zalloc(sizeof(uint8_t) * 1024);
  new_ctx->resource_create_stream.capacity = sizeof(uint8_t) * 1024;

  roa_spin_lock_init(&new_ctx->rsrc_destroy_lock);
  new_ctx->resource_destroy_stream.data = (uint8_t*)roa_zalloc(sizeof(uint8_t) * 1024);
  new_ctx->resource_destroy_stream.capacity = sizeof(uint8_t) * 1024;

  roa_spin_lock_init(&new_ctx->rdr_lock);
  int count = 1 << 13;
  new_ctx->render_stream.data = (uint8_t*)roa_zalloc(sizeof(uint8_t) * count);
  new_ctx->render_stream.capacity = sizeof(uint8_t) * count;

  roa_spin_lock_init(&new_ctx->rp_lock);
  roa_array_create_with_capacity(new_ctx->renderpasses, 128);

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


VOLT_BOOL
volt_ctx_has_pending_rsrcs(volt_ctx_t ctx)
{
  VOLT_BOOL pending = VOLT_FALSE;

  roa_spin_lock_aquire(&ctx->rsrc_create_lock);

  unsigned cmd_count = ctx->resource_create_stream.cmd_count;

  pending = cmd_count ? VOLT_TRUE : VOLT_FALSE;

  roa_spin_lock_release(&ctx->rsrc_create_lock);

  return pending;
}


void
volt_ctx_logging_callback(volt_ctx_t ctx, volt_log_callback_fn callback)
{
  /* param assert */
  ROA_ASSERT(ctx);
  ROA_ASSERT(callback);

  ctx->log_callback = callback;
}


void
volt_ctx_execute(volt_ctx_t ctx)
{
  ROA_ASSERT(ctx);

  auto err = glGetError(); /* clear msgs */
  //ROA_ASSERT(err == 0);

  /* create resource stream  */
  {
    roa_spin_lock_aquire(&ctx->rsrc_create_lock);

    const uint8_t *data = ctx->resource_create_stream.data;
    const unsigned bytes = roa_array_size(data);
    unsigned cmd_count = ctx->resource_create_stream.cmd_count;

    while ((cmd_count--) > 0)
    {
      const volt_gl_cmd_unknown *uk_cmd = (const volt_gl_cmd_unknown*)data;
      ROA_ASSERT(uk_cmd);

      /* log cmd */
      if (ROA_IS_ENABLED(GL_LOG_CMDS)) {
        if (ctx->log_callback) {
          char buffer[1024]{};
          strcat(buffer, "CMD: ");
          strcat(buffer, volt_gl_cmd_to_string(uk_cmd->id));
          strcat(buffer, "\n");
          ctx->log_callback(buffer);
        }
      }

      switch (uk_cmd->id)
      {
        case(volt_gl_cmd_id::create_buffer_vbo):
        {
          const volt_gl_cmd_create_vbo *cmd = (const volt_gl_cmd_create_vbo*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_create_vbo(cmd);
          break;
        }
        case(volt_gl_cmd_id::update_buffer_vbo):
        {
          const volt_gl_cmd_update_vbo *cmd = (const volt_gl_cmd_update_vbo*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_update_vbo(cmd);
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
        case(volt_gl_cmd_id::update_buffer_texture):
        {
          const volt_gl_cmd_update_texture *cmd = (const volt_gl_cmd_update_texture*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_update_texture(cmd);
          break;
        }
        case(volt_gl_cmd_id::create_buffer_framebuffer):
        {
          const volt_gl_cmd_create_framebuffer *cmd = (const volt_gl_cmd_create_framebuffer*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_create_framebuffer(cmd);
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

      /* log err */
      if(ROA_IS_ENABLED(GL_LOG_EXEC_ERRORS))
      {
        GLint err = glGetError();

        if(err)
        {
          if (ctx->log_callback)
          {
            char buffer[1024]{};
            strcat(buffer, "GL CREATE ERR: ");
            strcat(buffer, "\n");
            ctx->log_callback(buffer);
          }

          if (ROA_IS_ENABLED(GL_ASSERT_ON_ERRORS))
          {
            ROA_ASSERT(false);
          }
        }
      }
    }

    volt_gl_stream_clear(&ctx->resource_create_stream);
    roa_spin_lock_release(&ctx->rsrc_create_lock);
  }

  /* execute render stream */
  {
    glClearColor(0.5, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_SCISSOR_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    //glViewport(0, 0, 800, 480);
    //glScissor(0, 0, 800, 480);
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_NONE);

    const uint8_t *data = ctx->render_stream.data;
    const unsigned bytes = roa_array_size(data);
    unsigned cmd_count = ctx->render_stream.cmd_count;

    while ((cmd_count--) > 0)
    {
      const volt_gl_cmd_unknown *uk_cmd = (const volt_gl_cmd_unknown*)data;
      ROA_ASSERT(uk_cmd);

      /* log cmd */
      if (ROA_IS_ENABLED(GL_LOG_CMDS)) {
        if (ctx->log_callback) {
          char buffer[1024]{};
          strcat(buffer, "CMD: ");
          strcat(buffer, volt_gl_cmd_to_string(uk_cmd->id));
          strcat(buffer, "\n");
          ctx->log_callback(buffer);
        }
      }

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
        case(volt_gl_cmd_id::set_viewport):
        {
          const volt_gl_cmd_set_viewport *cmd = (const volt_gl_cmd_set_viewport*)uk_cmd;
          data += sizeof(*cmd);
          volt_gl_set_viewport(cmd);
          break;
        }
        default:
          /* only renderpass cmds should be here */
          ROA_ASSERT(false);
      }

      /* log err */
      if (ROA_IS_ENABLED(GL_LOG_EXEC_ERRORS))
      {
        GLint err = glGetError();

        if(err)
        {
          if (ctx->log_callback)
          {
            char buffer[1024]{};
            strcat(buffer, "GL RENDER ERR: ");
            strcat(buffer, "\n");
            ctx->log_callback(buffer);
          }


          if (ROA_IS_ENABLED(GL_ASSERT_ON_ERRORS))
          {
            ROA_ASSERT(false);
          }
        }
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


#undef GL_LOG_CMDS
#undef GL_ASSERT
#undef GL_ASSERT_ON_ERRORS
#undef GL_LOG_EXEC_ERRORS


#endif
