#define VOLT_OGL4

#ifdef VOLT_OGL4

#include <volt/volt.h>
#include <GL/gl3w.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>


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

};


struct volt_ctx
{
  GLuint vao;

  /* array */ struct volt_vbo_desc *pending_create_vbo_desc;
  /* array */ struct volt_program_desc *pending_create_program_desc;
  
  /* array */ volt_renderpass *renderpasses;
};


/* internal */


struct volt_draw_call
{
  GLuint vbo;
  GLuint ibo;
  GLint texture[3];
};


struct volt_renderpass
{
  /* array */ volt_draw_call *draw_calls;
};


/* lifetime */


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
        const volt_vbo_desc *vbo_desc = &ctx->pending_create_vbo_desc[i];

        GLuint vbo{};
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(
          GL_ARRAY_BUFFER,
          sizeof(float) * vbo_desc->count,
          vbo_desc->data, GL_STATIC_DRAW);

        ROA_ASSERT_PEDANTIC(vbo > 0);
      }

      roa_array_resize(ctx->pending_create_vbo_desc, 0);
    }

    /* create programs */
    {
      const unsigned program_count = roa_array_size(ctx->pending_create_program_desc);

      for (int i = 0; i < program_count; ++i)
      {
        const volt_program_desc *prog_desc = &ctx->pending_create_program_desc[i];

        GLuint shaders[VOLT_SHD_STAGE_COUNT]{};
        GLenum shader_enum[VOLT_SHD_STAGE_COUNT]{
          GL_VERTEX_SHADER,
          GL_GEOMETRY_SHADER,
          GL_FRAGMENT_SHADER,
        }; /* make sure this aligns with volt.h's enum */

        for (int j = 0; j < prog_desc->stage_count; ++j)
        {
          const volt_shader_stage stage = prog_desc->shader_stages_type[j];

          shaders[stage] = glCreateShader(shader_enum[stage]);
          GLchar const *src = prog_desc->shader_stages_src[j];
          glShaderSource(shaders[stage], 1, &src, NULL);

          /* check for errors */
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

          glBindFragDataLocation(program, 0, "out_color");
          glLinkProgram(program);

          /* check for errors */
        }
      }
    }
  }

  /* destroy pending resources */
  {

  }

  /* process all the renderpasses */
  const unsigned rp_count = roa_array_size(ctx->renderpasses);

  for (int i = 0; i < rp_count; ++i)
  {
    const volt_renderpass *rp = &ctx->renderpasses[i];

    /* render all the draw calls */
    const unsigned draw_count = roa_array_size(rp->draw_calls);

    for (int j = 0; j < draw_count; ++j)
    {
      const volt_draw_call *dc = &rp->draw_calls[j];


    }
  }

  glClearColor(1,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/* resources */


void
volt_vertex_buffer_create(volt_ctx_t ctx, volt_vbo_t *t, struct volt_vbo_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(t);
  ROA_ASSERT(desc);
  
  /* store desc as pending type */
  roa_array_push(ctx->pending_create_vbo_desc, *desc);
}


void
volt_program_create(
  volt_ctx_t ctx,
  volt_program_t *program,
  struct volt_program_desc *desc)
{
  ROA_ASSERT(ctx);
  ROA_ASSERT(*program);
  ROA_ASSERT(desc);
  
  /* store desc as pending type */
  roa_array_push(ctx->pending_create_program_desc, *desc);
}


/* renderpass */


void
volt_renderpass_create(volt_renderpass_t *pass)
{

}


void
volt_renderpass_commit(volt_renderpass_t *pass)
{

}


void
volt_renderpass_bind_input_format(volt_renderpass_t pass, volt_input_t)
{

}


void
volt_renderpass_bind_vertex_buffer(volt_renderpass_t pass, volt_vbo_t vbo)
{

}


void
volt_renderpass_bind_index_buffer(volt_renderpass_t pass, volt_ibo_t ibo)
{

}


void
volt_renderpass_draw(volt_renderpass_t pass)
{

}



#endif