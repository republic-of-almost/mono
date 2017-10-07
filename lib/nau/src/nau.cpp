#include <nau/nau.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>


/* ------------------------------------------------------------ [ Config ] -- */


#ifndef NAU_MALLOC
#include <stdlib.h>
#define NAU_MALLOC(bytes) malloc(bytes)
#endif


#ifndef NAU_ASSERT
#include <assert.h>
#define NAU_ASSERT(expr) assert(expr)
#endif


#ifndef NAU_LOGGING
#ifndef NAU_LOGGER
#include <stdio.h>
#define NAU_LOG(str) printf("%s\n", str)
#endif
#endif


/* ------------------------------------------------------------- [ Types ] -- */


struct Nau_theme
{
  unsigned int bg_color_ctx;
  unsigned int bg_color_panel;
};

struct Nau_node
{
  Nau_panel_callback cb;
  Nau_node *children[2];
};

struct Nau_panel
{
  char **name;
  Nau_panel_callback *cb;
  
  size_t panel_count;
  size_t panel_capacity;
};

struct Nau_screen
{
  int width;
  int height;
};


struct Nau_draw_buffers
{
  float *vbo;
  int vbo_count;
  int vbo_capacity;
  
  unsigned int *idx;
  int idx_count;
  int idx_capacity;
  
  Nau_draw_cmd *cmds;
  int cmd_count;
  int cmd_capacity;
};


struct Nau_ctx
{
  Nau_panel panel_data;
  Nau_draw_buffers draw_data;
  Nau_screen screen;
  Nau_theme theme;
};



/* ---------------------------------------------------------- [ Internal ] -- */


void
nau_submit_tris(Nau_ctx *ctx, const float verts[], const float texture_coords[], unsigned int color, int tri_count, int clip[4])
{
  /* param check */
  {
    NAU_ASSERT(ctx);
    NAU_ASSERT(verts);
    NAU_ASSERT(clip);
    NAU_ASSERT(tri_count);
  }
  
  /* submit verts */
  {
    for(int i = 0; i < tri_count; ++i)
    {
      int index = 0;
    
      ctx->draw_data.vbo[index + 0] = verts[0];
      ctx->draw_data.vbo[index + 1] = verts[1];
      ctx->draw_data.vbo[index + 2] = verts[2];
      
      ctx->draw_data.vbo[index + 3] = texture_coords[0];
      ctx->draw_data.vbo[index + 4] = texture_coords[1];
      
      ctx->draw_data.vbo[index + 5] = 1.f;
      ctx->draw_data.vbo[index + 6] = 1.f;
      ctx->draw_data.vbo[index + 7] = 0.f;
      ctx->draw_data.vbo[index + 8] = 1.f;
    }
  }
  
  /* submit index */
  {
    for(int i = 0; i < tri_count; ++i)
    {
    }
  }
  
  /* submit draw cmds */
  {
    
  }
}


/* ---------------------------------------------------------- [ Lifetime ] -- */


void
nau_initialize(Nau_ctx **ctx)
{
  /* new context */
  Nau_ctx *new_ctx = NULL;
  {
    new_ctx = (Nau_ctx*)NAU_MALLOC(sizeof(Nau_ctx));
    memset(new_ctx, 0, sizeof(Nau_ctx));
  }
  
  /* default panels */
  {
    size_t default_cap = 16;
    new_ctx->panel_data.name           = (char **)NAU_MALLOC(sizeof(char*) * default_cap);
    new_ctx->panel_data.cb             = (Nau_panel_callback*)NAU_MALLOC(sizeof(Nau_panel_callback) * default_cap);
    new_ctx->panel_data.panel_capacity = default_cap;
    new_ctx->panel_data.panel_count    = 0;
  }
  
  /* default draw buffer */
  {
    size_t vbo_count = 1024 * (3 + 3 + 2);
    new_ctx->draw_data.vbo = (float*)NAU_MALLOC(sizeof(float) * vbo_count);
    new_ctx->draw_data.vbo_count = 0;
    new_ctx->draw_data.vbo_capacity = vbo_count;

    size_t idx_count = 1024 * 3;
    new_ctx->draw_data.idx = (unsigned int*)NAU_MALLOC(sizeof(unsigned int) * idx_count);
    new_ctx->draw_data.idx_count = 0;
    new_ctx->draw_data.idx_capacity = idx_count;
    
    size_t cmd_count = 512;
    new_ctx->draw_data.cmds = (Nau_draw_cmd*)NAU_MALLOC(sizeof(Nau_draw_cmd) * cmd_count);
    new_ctx->draw_data.cmd_count = 0;
    new_ctx->draw_data.cmd_capacity = cmd_count;
  }
  
  /* default theme */
  {
    new_ctx->theme.bg_color_ctx = 0x333333FF;
    new_ctx->theme.bg_color_panel = 0x999999FF;
  }
  
  *ctx = new_ctx;
  
  NAU_LOG("Created NAU CTX");
}


void
nau_destroy(Nau_ctx **ctx)
{
}


/* -------------------------------------------------------------- [ Data ] -- */


void
nau_new_frame(Nau_ctx *ctx)
{
}


void
nau_create_draw_cmds(Nau_ctx *ctx)
{
  /* param check */
  {
    NAU_ASSERT(ctx);
  }
  
  /* walk through graph */
  {
    /* submit background */
    float verts[] { 0, 0, 100, 100, 0, -100 };
    float coords[] { 0, 0, 100, 100, 0, -100 };
    int clip[] {0,0,200, 200};
    
    nau_submit_tris(ctx, verts, coords, 0xFFFFFFF, 1, clip);
    
    
    
    /* submit children */
//    nau_submit_tris(0, 0, 5, 5, color, clip);
//    nau_submit_tris(5, 5, 10, 10, color, clip);
  }
}


void
nau_render_data(Nau_ctx *ctx, Nau_draw_cmd **renderables, int *count)
{
}


/* ---------------------------------------------------------- [ Settings ] -- */


void
nau_set_viewport(Nau_ctx *ctx, const int width, const int height)
{
  /* param check */
  {
    NAU_ASSERT(ctx);
  }
  
  ctx->screen.width = width;
  ctx->screen.height = height;
}


/* ------------------------------------------------------------ [ Panels ] -- */


void
nau_register_panel(Nau_ctx *ctx, const char *name, const Nau_panel_callback cb)
{
  /* param check */
  {
    NAU_ASSERT(ctx);
    NAU_ASSERT(name);
    NAU_ASSERT(strlen(name));
    NAU_ASSERT(cb);
  }
  
  /* add panel callback */
  {
    NAU_ASSERT(ctx->panel_data.panel_count < ctx->panel_data.panel_capacity); // need more space
    
    size_t name_bytes = sizeof(char) * (strlen(name) + 1);
    
    char *cpy_name = (char *)NAU_MALLOC(name_bytes);
    memcpy(cpy_name, name, name_bytes);
    
    size_t index = ctx->panel_data.panel_count;
    ctx->panel_data.panel_count += 1;
    
    ctx->panel_data.name[index] = cpy_name;
    ctx->panel_data.cb[index] = cb;
  }
}


/* ----------------------------------------------------------- [ Widgets ] -- */





/* ------------------------------------------------------------ [ Config ] -- */

#undef NAU_MALLOC
#undef NAU_ASSERT
