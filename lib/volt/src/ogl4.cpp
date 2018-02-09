#define VOLT_OGL4

#ifdef VOLT_OGL4

#include <volt/volt.h>
#include <GL/gl3w.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <stdio.h>


/* ---------------------------------------------- [ internal gl structs ] -- */


struct volt_program
{
  GLuint program;
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
  unsigned increment_stride[12];
  unsigned attrib_count[12];
  unsigned full_stride;
  unsigned count;
};


struct volt_pending_vbo {
  volt_vbo_t vbo;
  volt_vbo_desc desc;
};

struct volt_pending_program {
  volt_program_t program;
  volt_program_desc desc;
};

struct volt_ctx
{
  GLuint vao;

  /* array */ struct volt_pending_vbo *pending_create_vbo_desc;
  /* array */ struct volt_pending_program *pending_create_program_desc;
  
  /* array */ volt_renderpass_t *renderpasses;
};


/* internal */


struct volt_draw_call
{
  GLuint program;
  GLuint vbo;
  GLuint ibo;
  GLint texture[3];
};


struct volt_renderpass
{
  /* array */ volt_draw_call *draw_calls;
};


/* --------------------------------------------------------- [ lifetime ] -- */


void
volt_ctx_create(volt_ctx_t *ctx)
{
  if (gl3wInit())
  {
    /* fail */
    return;
  }

  struct volt_ctx *new_ctx = nullptr;
  new_ctx = (volt_ctx*)roa_zalloc(sizeof(new_ctx[0]));

  roa_array_create(new_ctx->renderpasses, 128);
  roa_array_create(new_ctx->pending_create_vbo_desc, 32);
  roa_array_create(new_ctx->pending_create_program_desc, 32);

  *ctx = new_ctx;

  glCreateVertexArrays(1, &new_ctx->vao);
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

  /* bind because some thridparty might have */
  glBindVertexArray(ctx->vao);

  /* create pending resources */
  {
    /* create vbos */
    {
      const unsigned vbo_count = roa_array_size(ctx->pending_create_vbo_desc);

      for (int i = 0; i < vbo_count; ++i)
      {
        const volt_pending_vbo *pending_vbo = &ctx->pending_create_vbo_desc[i];

        GLuint vbo{};

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(
          GL_ARRAY_BUFFER,
          sizeof(float) * pending_vbo->desc.count,
          pending_vbo->desc.data,
          GL_STATIC_DRAW);

        ROA_ASSERT_PEDANTIC(vbo > 0);

        pending_vbo->vbo->vbo = vbo;
      }

      roa_array_resize(ctx->pending_create_vbo_desc, 0);
    }

    /* create programs */
    {
      const unsigned program_count = roa_array_size(ctx->pending_create_program_desc);

      for (int i = 0; i < program_count; ++i)
      {
        const volt_pending_program *pending_prog = &ctx->pending_create_program_desc[i];

        GLuint shaders[VOLT_SHD_STAGE_COUNT]{};
        GLenum shader_enum[VOLT_SHD_STAGE_COUNT]{
          GL_VERTEX_SHADER,
          GL_GEOMETRY_SHADER,
          GL_FRAGMENT_SHADER,
        }; /* make sure this aligns with volt.h's enum */

        for (int j = 0; j < pending_prog->desc.stage_count; ++j)
        {
          const volt_shader_stage stage = pending_prog->desc.shader_stages_type[j];

          shaders[stage] = glCreateShader(shader_enum[stage]);
          GLchar const *src = pending_prog->desc.shader_stages_src[j];
          glShaderSource(shaders[stage], 1, &src, NULL);
          glCompileShader(shaders[stage]);

          /* check for errors */
          GLint status = 0;
          glGetShaderiv(shaders[stage], GL_COMPILE_STATUS, &status);
          
          GLchar error[1024] {};
          glGetShaderInfoLog(shaders[stage], 1024, NULL, error);

          ROA_ASSERT(status == GL_TRUE);
        }

        /* link shaders */
        {
          GLuint program = glCreateProgram();
          
          for (GLuint shd : shaders)
          {
            if (shd != 0)
            {
              glAttachShader(program, shd);
            }
          }

          glBindFragDataLocation(program, 0, "outColor");
          glLinkProgram(program);

          GLint status;
          glGetProgramiv(program, GL_LINK_STATUS, &status);
          ROA_ASSERT(status == GL_TRUE);

          pending_prog->program->program = program;

          /* check for errors */
        }
      }

      roa_array_resize(ctx->pending_create_program_desc, 0);
    }
  }

  /* destroy pending resources */
  {
    
  }

  /* process all the renderpasses */
  const unsigned rp_count = roa_array_size(ctx->renderpasses);

  glClearColor(1, 0, 0, 1);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < rp_count; ++i)
  {
    const volt_renderpass_t rp = ctx->renderpasses[i];

    /* render all the draw calls */
    const unsigned draw_count = roa_array_size(rp->draw_calls);

    for (int j = 0; j < draw_count; ++j)
    {
      const volt_draw_call *dc = &rp->draw_calls[j];

      glUseProgram(dc->program);

      glBindBuffer(GL_ARRAY_BUFFER, dc->vbo);

      // Specify the layout of the vertex data
      GLint posAttrib = glGetAttribLocation(dc->program, "position");
      glEnableVertexAttribArray(posAttrib);
      glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

      GLint colAttrib = glGetAttribLocation(dc->program, "color");
      glEnableVertexAttribArray(colAttrib);
      glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

      {
        auto err = glGetError();
        printf("GLErr: %d\n", err);
      }

      
      // Draw a triangle from the 3 vertices
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    roa_array_destroy(rp->draw_calls);
    roa_free(rp);
  }

  roa_array_resize(ctx->renderpasses, 0);
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

  volt_vbo_t new_vbo = (volt_vbo_t)roa_alloc(sizeof(*new_vbo));
  new_vbo->vbo = 0;

  *vbo = new_vbo;

  volt_pending_vbo pending{
    new_vbo,
    *desc,
  };

  /* store desc as pending type */
  roa_array_push(ctx->pending_create_vbo_desc, pending);
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
volt_renderpass_bind_input_format(
  volt_renderpass_t pass,
  volt_input_t)
{
  
}


void
volt_renderpass_bind_vertex_buffer(
  volt_renderpass_t pass,
  volt_vbo_t vbo)
{
  volt_draw_call *dc = roa_array_back(pass->draw_calls);

  dc->vbo = vbo->vbo;
}


void
volt_renderpass_bind_index_buffer(
  volt_renderpass_t pass,
  volt_ibo_t ibo)
{
  volt_draw_call *dc = roa_array_back(pass->draw_calls);

  dc->ibo = ibo->ibo;
}


void
volt_renderpass_bind_program(
  volt_renderpass_t pass,
  volt_program_t program)
{
  volt_draw_call *dc = roa_array_back(pass->draw_calls);

  dc->program = program->program;
}


void
volt_renderpass_draw(volt_renderpass_t pass)
{
  roa_array_push(pass->draw_calls, volt_draw_call{});
}



#endif