#include <nau/nau.h>


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
#ifndef NAU_LOG
#include <stdio.h>
#define NAU_LOG(str) printf("%s\n", str)
#endif
#endif


#ifndef NAU_ZERO_MEM
#include <string.h>
#define NAU_ZERO_MEM(var, type) memset(var, 0, sizeof(type))
#endif


/* ---------------------------------------------------- [ Types and Data ] -- */


struct Nau_theme
{
  unsigned int bg_color_ctx;
  unsigned int bg_color_panel;
};


struct Nau_device
{
  int width;
  int height;
  
  int ptr_x;
  int ptr_y;
  
  int ptr_status; // 0 up 1 down
};


struct Nau_env
{
  int min[2];
  int max[2];
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
  Nau_draw_buffers  draw_data;
  Nau_device        device;
  Nau_theme         theme;
};


/* ---------------------------------------------------------- [ Internal ] -- */


// Pulled from Imgui should replace with what imgui recomends
static unsigned int
nau_hash(const void *data, const size_t data_size, const unsigned int seed)
{
  static unsigned int crc32 [256] {0};
  if(!crc32[1])
  {
    const unsigned int polynomial = 0xEDB88320;
    for(unsigned int i = 0; i < 256; ++i)
    {
      unsigned int crc = i;

      for(unsigned int j = 0; j < 8; j++)
      {
        crc = (crc >> 1) ^ ((unsigned int)(-int(crc & 1)) & polynomial);

        crc32[i] = crc;
      }
    }
  }

  const unsigned int s = ~seed;
  unsigned int crc = s;
  const unsigned char *current = (const unsigned char*)data;

  if(data_size == 0)
  {
    unsigned int count_down = data_size;

    while(count_down--)
    {
      crc = (crc >> 8) ^ crc32[(crc & 0xFF) ^ *current++];
    }
  }
  else
  {
    while(unsigned char c = *current++)
    {
      crc = (crc >> 8) ^ crc32[(crc & 0xFF) ^ c];
    }
  }

  return ~crc;
}


static void
nau_submit_tris(Nau_ctx *ctx, const float verts[], const float texture_coords[], unsigned int color, int tri_count, int clip[4])
{

}


static void
nau_submit_cmd(Nau_ctx *ctx, Nau_env area, Nau_env clip, unsigned int color)
{
  /* some info */
  const size_t vertex_stride = 3 + 2 + 4;
  const size_t offset = ctx->draw_data.vbo_count / vertex_stride;

  /* add vertex information */
  {
    size_t index = ctx->draw_data.vbo_count;
    
    /* position / uv / color */
    
    ctx->draw_data.vbo[index++] = area.min[0];
    ctx->draw_data.vbo[index++] = area.min[1];
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 0.f;
    
    /**/
    
    ctx->draw_data.vbo[index++] = area.max[0];
    ctx->draw_data.vbo[index++] = area.min[1];
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 0.f;
    
    /**/
    
    ctx->draw_data.vbo[index++] = area.min[0];
    ctx->draw_data.vbo[index++] = area.max[1];
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 0.f;
    
    /**/
    
    ctx->draw_data.vbo[index++] = area.max[0];
    ctx->draw_data.vbo[index++] = area.max[1];
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 0.f;
    
    /**/
    
    ctx->draw_data.vbo_count = index;
  }
  
  /* add index information */
  {
    size_t index = ctx->draw_data.idx_count;
    
    ctx->draw_data.idx[index++] = offset + 0;
    ctx->draw_data.idx[index++] = offset + 1;
    ctx->draw_data.idx[index++] = offset + 2;
    
    /**/
    
    ctx->draw_data.idx[index++] = offset + 2;
    ctx->draw_data.idx[index++] = offset + 3;
    ctx->draw_data.idx[index++] = offset + 0;
  
    ctx->draw_data.idx_count = index;
  }
  
  /* draw cmd */
  {
    const int index = ctx->draw_data.cmd_count;
    
    int cmd_clip[4] { clip.min[0], clip.min[1], clip.max[0], clip.max[1] };
    
    memcpy(&ctx->draw_data.cmds[index].clip, cmd_clip, sizeof(cmd_clip));
    ctx->draw_data.cmds[index].count = 6;
    ctx->draw_data.cmds[index].offset = offset;
    
    ctx->draw_data.cmd_count += 1;
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
    NAU_ZERO_MEM(new_ctx, Nau_ctx);
  }
  
  /* default draw buffer */
  {
    const int vbo_count = 1024 * (3 + 3 + 2);
    const int vbo_bytes = sizeof(float) * vbo_count;
    new_ctx->draw_data.vbo = (float*)NAU_MALLOC(vbo_bytes);
    new_ctx->draw_data.vbo_count = 0;
    new_ctx->draw_data.vbo_capacity = vbo_count;

    const int idx_count = 1024 * 3;
    const int idx_bytes = sizeof(unsigned int) * idx_count;
    new_ctx->draw_data.idx = (unsigned int*)NAU_MALLOC(idx_bytes);
    new_ctx->draw_data.idx_count = 0;
    new_ctx->draw_data.idx_capacity = idx_count;
    
    const int cmd_count = 512;
    const int cmd_bytes = sizeof(Nau_draw_cmd) * cmd_count;
    new_ctx->draw_data.cmds = (Nau_draw_cmd*)NAU_MALLOC(cmd_bytes);
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
  NAU_LOG("TODO: Must release resources");
}


void
nau_new_frame(Nau_ctx *ctx)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }
  
  /* reset draw cmds */
  {
    ctx->draw_data.vbo_count = 0;
    ctx->draw_data.idx_count = 0;
    ctx->draw_data.cmd_count = 0;
  }
}


/* -------------------------------------------------------------- [ Data ] -- */


void
nau_get_vbo(Nau_ctx *ctx, float **vbo, int *count)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
    NAU_ASSERT(vbo != NULL);
    NAU_ASSERT(count != NULL);
  }
  
  /* fetch data */
  {
    *vbo = ctx->draw_data.vbo;
    *count = ctx->draw_data.vbo_count;
  }
}


void
nau_get_idx(Nau_ctx *ctx, unsigned int **idx, int *count)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
    NAU_ASSERT(idx != NULL);
    NAU_ASSERT(count != NULL);
  }

  /* fetch data */
  {
    *idx = ctx->draw_data.idx;
    *count = ctx->draw_data.idx_count;
  }
}


void
nau_get_cmds(Nau_ctx *ctx, Nau_draw_cmd **cmds, int *count)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
    NAU_ASSERT(cmds != NULL);
    NAU_ASSERT(count != NULL);
  }
  
  /* fetch data */
  {
    *cmds = ctx->draw_data.cmds;
    *count = ctx->draw_data.cmd_count;
  }
}


void
nau_get_viewport(Nau_ctx *ctx, int *width, int *height)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
    NAU_ASSERT(width != NULL);
    NAU_ASSERT(height != NULL);
  }

  /* fetch data */
  {
    *width = ctx->device.width;
    *height = ctx->device.height;
  }
}


/* ---------------------------------------------------------- [ Settings ] -- */


void
nau_set_viewport(Nau_ctx *ctx, int width, int height)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }
  
  /* update window size */
  {
    ctx->device.width = width;
    ctx->device.height = height;
  }
}


void
nau_set_pointer_coords(Nau_ctx *ctx, int x, int y)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }

  /* update ptr coords */
  {
    ctx->device.ptr_x = x;
    ctx->device.ptr_y = y;
  }
}


void
nau_set_pointer_status(Nau_ctx *ctx, int status)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }

  /* update ptr status */
  {
    ctx->device.ptr_status = status;
  }
}


/* ------------------------------------------------------------ [ Panels ] -- */


void
nau_begin(Nau_ctx *ctx, const char *name)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
    NAU_ASSERT(name != NULL);
    NAU_ASSERT(strlen(name) > 0);
  }

  /* window id */
  const unsigned int win_id = nau_hash(name, strlen(name), 0);
  
  /* window body */
  {
    Nau_env area{{10, 10}, {100, 100}};
    Nau_env clip{{10, 10}, {100, 100}};
    unsigned int color = 0xFF0000FF;
  
    nau_submit_cmd(ctx, area, clip, color);
  }
}


void
nau_end(Nau_ctx *ctx)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }
}


/* ----------------------------------------------------------- [ Widgets ] -- */


void
nau_button(Nau_ctx *ctx, const char *name)
{
}


/* ------------------------------------------------------------ [ Config ] -- */


#undef NAU_MALLOC
#undef NAU_ASSERT
#undef NAU_LOGGING
#undef NAU_ZERO_MEM
