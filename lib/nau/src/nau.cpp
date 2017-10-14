#include <nau/nau.h>
#include <stdint.h>


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

/* general types */


struct Nau_vec2
{
  float x;
  float y;
};


struct Nau_env
{
  Nau_vec2 min;
  Nau_vec2 max;
};


/* math functions */

static Nau_vec2
nau_vec2_subtract(Nau_vec2 a, Nau_vec2 b)
{
  return Nau_vec2{a.x - b.x, a.y - b.y};
}


static Nau_vec2
nau_vec2_add(Nau_vec2 a, Nau_vec2 b)
{
  return Nau_vec2{a.x + b.x, a.y + b.y};
}


static bool
nau_scalar_between(float val, float start, float end)
{
  return val > start && val < end;
}


static bool
nau_env_contains(Nau_env env, Nau_vec2 pos)
{
  const bool between_x = nau_scalar_between(pos.x, env.min.x, env.max.x);
  const bool between_y = nau_scalar_between(pos.y, env.min.y, env.max.y);
  
  return between_x && between_y;
}


static Nau_vec2
nau_env_size(Nau_env env)
{
  return Nau_vec2{env.max.x - env.min.x, env.max.y - env.min.y};
}


static Nau_env
env_from_pos_size(Nau_vec2 pos, Nau_vec2 size)
{
  return Nau_env{pos, nau_vec2_add(pos, size)};
}


/*
  Nau_interactable
  --
  The properties of an interactable object.
*/
typedef enum
{
  NAU_INTERACT_DRAG,
  NAU_INTERACT_RESIZE,
  NAU_INTERACT_CLICKABLE,
  
} Nau_iteract_flags;

struct Nau_interactable
{
  uint64_t id; /* window | field */
  unsigned int flags;
  Nau_env env;
};


/*
  Nau_window
  --
  Represents a window on the screen.
*/
struct Nau_window
{
  uint32_t id;
  
  Nau_vec2 pos;
  Nau_vec2 size;
};


/*** state controllers ***/

/*
  Nau_theme_data
  --
  Holds all the theme data for all the UI elements.
  this is fully editable.
*/
struct Nau_theme_data
{
  unsigned int bg_color_ctx;
  unsigned int bg_color_panel;
};


/*
  Nau_device_data
  --
  Holds any device details, mouse / screen etc.
*/
typedef enum {
  
  NAU_MS_ACTION_UP    = NAU_PTR_STATUS_UP,
  NAU_MS_ACTION_DOWN  = NAU_PTR_STATUS_DOWN,
  NAU_MS_ACTION_HOLD,
  NAU_MS_ACTION_CLICK,
  
} Nau_ptr_action;

struct Nau_device_data
{
  int             width;
  int             height;
  
  Nau_vec2        ptr_pos;
  Nau_vec2        ptr_last_pos;
  Nau_vec2        ptr_diff;
  
  Nau_ptr_status  ptr_status;  // 0 up 1 down
  Nau_ptr_action  ptr_action;  // infered from status
};


/*
  The stage can be considered the area that we are rendering to. This holds
  active windows, window history and anything that is on the screen and
  interactable.
*/
struct Nau_stage_data
{
  Nau_window        *win_history;
  int                win_history_count;
  int                win_history_capacity;
  
  Nau_interactable  *interacts;
  int                interacts_count;
  int                interacts_capacity;
  
  uint64_t           interacts_current;
  Nau_vec2           interacts_coords;
};


/*
  Holds anything related to the actual GPU drawing side of things.
*/
struct Nau_draw_buffer_data
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


/* ctx */

/*
  Holds all the state for this context. You may have multiple contexts, the 
  downside of this means you need to pass in the context for each function.
*/
struct Nau_ctx
{
  Nau_draw_buffer_data  draw_data;
  Nau_device_data       device;
  Nau_theme_data        theme;
  Nau_stage_data        stage_data;
};


/* ---------------------------------------------------------- [ Internal ] -- */


// Pulled from Imgui should replace with what imgui recomends
static uint32_t
nau_hash(const void *data, const size_t data_size, const uint32_t seed)
{
  static uint32_t crc32 [256] {0};
  if(!crc32[1])
  {
    const uint32_t polynomial = 0xEDB88320;
    for(uint32_t i = 0; i < 256; ++i)
    {
      uint32_t crc = i;

      for(uint32_t j = 0; j < 8; j++)
      {
        crc = (crc >> 1) ^ ((uint32_t)(-int32_t(crc & 1)) & polynomial);

        crc32[i] = crc;
      }
    }
  }

  const uint32_t s = ~seed;
  uint32_t crc = s;
  const uint8_t *current = (const uint8_t*)data;

  if(data_size == 0)
  {
    uint32_t count_down = data_size;

    while(count_down--)
    {
      crc = (crc >> 8) ^ crc32[(crc & 0xFF) ^ *current++];
    }
  }
  else
  {
    while(uint8_t c = *current++)
    {
      crc = (crc >> 8) ^ crc32[(crc & 0xFF) ^ c];
    }
  }

  return ~crc;
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
    
    ctx->draw_data.vbo[index++] = area.min.x;
    ctx->draw_data.vbo[index++] = area.min.y;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 0.f;
    
    /**/
    
    ctx->draw_data.vbo[index++] = area.max.x;
    ctx->draw_data.vbo[index++] = area.min.y;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 0.f;
    
    /**/
    
    ctx->draw_data.vbo[index++] = area.min.x;
    ctx->draw_data.vbo[index++] = area.max.y;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 1.f;
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = 0.f;
    ctx->draw_data.vbo[index++] = 0.f;
    
    /**/
    
    ctx->draw_data.vbo[index++] = area.max.x;
    ctx->draw_data.vbo[index++] = area.max.y;
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
    
    ctx->draw_data.idx[index++] = offset + 1;
    ctx->draw_data.idx[index++] = offset + 2;
    ctx->draw_data.idx[index++] = offset + 3;
  
    ctx->draw_data.idx_count = index;
  }
  
  /* draw cmd */
  {
    const int index = ctx->draw_data.cmd_count;
    
    int cmd_clip[4] { (int)clip.min.x, (int)clip.min.y, (int)clip.max.x, (int)clip.max.y };
    
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
  
  /* default stage */
  {
    const int win_count = 32;
    const int win_bytes = sizeof(Nau_window) * win_count;
    new_ctx->stage_data.win_history = (Nau_window*)NAU_MALLOC(win_bytes);
    new_ctx->stage_data.win_history_count = 0;
    new_ctx->stage_data.win_history_capacity = win_count;
    
    const int inter_count = 128;
    const int inter_bytes = sizeof(Nau_interactable) * inter_count;
    new_ctx->stage_data.interacts = (Nau_interactable*)NAU_MALLOC(inter_bytes);
    new_ctx->stage_data.interacts_count = 0;
    new_ctx->stage_data.interacts_capacity = inter_count;
    new_ctx->stage_data.interacts_current = 0;
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
  
  /* update input */
  {
    ctx->device.ptr_diff = nau_vec2_subtract(ctx->device.ptr_pos, ctx->device.ptr_last_pos);
    ctx->device.ptr_last_pos = ctx->device.ptr_pos;
  
    ctx->device.ptr_action = ctx->device.ptr_action == NAU_MS_ACTION_DOWN ? NAU_MS_ACTION_HOLD : ctx->device.ptr_action;
    ctx->device.ptr_action = ctx->device.ptr_action == NAU_MS_ACTION_CLICK ? NAU_MS_ACTION_UP : ctx->device.ptr_action;
    
    ctx->stage_data.interacts_coords = ctx->device.ptr_pos;
  }
  
  /* update interactions */
  {
    const bool is_held  = ctx->device.ptr_action == NAU_MS_ACTION_HOLD;
    const bool is_click = ctx->device.ptr_action == NAU_MS_ACTION_CLICK;
    const bool no_curr  = ctx->stage_data.interacts_current == 0;
    
    if((is_held || is_click) && no_curr)
    {
      const uint32_t count = ctx->stage_data.interacts_count;
      const Nau_interactable *interact = ctx->stage_data.interacts;
      const Nau_vec2 point = ctx->device.ptr_pos;
      
      for(uint32_t i = 0; i < count; ++i)
      {
        if(nau_env_contains(interact[i].env, point))
        {
          ctx->stage_data.interacts_current = interact[i].id;
        }
      }
    }
  }
  
  /* reset the context */
  {
    ctx->stage_data.interacts_count = 0;
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
    ctx->device.ptr_pos.x = (float)x;
    ctx->device.ptr_pos.y = (float)y;
  }
}


void
nau_set_pointer_status(Nau_ctx *ctx, Nau_ptr_status status)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }

  /* update ptr status */
  {
    ctx->device.ptr_status = status;
    
    if(status == NAU_PTR_STATUS_UP)
    {
      const bool is_down = ctx->device.ptr_action == NAU_MS_ACTION_DOWN;
      const bool is_held = ctx->device.ptr_action == NAU_MS_ACTION_HOLD;
    
      if(is_down || is_held)
      {
        ctx->device.ptr_action = NAU_MS_ACTION_CLICK;
      }
      else
      {
        ctx->device.ptr_action = NAU_MS_ACTION_UP;
      }
    }
    else
    {
      ctx->device.ptr_action = NAU_MS_ACTION_DOWN;
    }
    
    ctx->stage_data.interacts_current = 0;
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
  const uint32_t win_id = nau_hash(name, strlen(name), 0);
  Nau_window window;
  bool win_cached = false;
  window.id = win_id;
  window.pos = Nau_vec2{10,10};
  window.size = Nau_vec2{100, 130};
  
  /* get window history */
  {
    const uint32_t count = ctx->stage_data.win_history_count;
    const Nau_window *windows = ctx->stage_data.win_history;
    
    for(uint32_t i = 0; i < count; ++i)
    {
      if(windows[i].id == win_id)
      {
        window = windows[i];
        win_cached = true;
      }
    }
  }
  
  Nau_env area = env_from_pos_size(window.pos, window.size);
  
  /* check interacts */
  {
    const uint64_t curr_interact = ctx->stage_data.interacts_current;
    const uint64_t test = win_id | 0xFF;
    
    if(curr_interact == test)
    {
      window.pos = nau_vec2_add(window.pos, ctx->device.ptr_diff);
      
      // Reset area
      area = env_from_pos_size(window.pos, window.size);
    }
  }
  
  /* window body */
  {
    const uint32_t color = 0xFF0000FF;
  
    nau_submit_cmd(ctx, area, area, color);
  }
  
  /* submit interactable */
  {
    const int index = ctx->stage_data.interacts_count;
    ++ctx->stage_data.interacts_count;
    
    Nau_interactable *inter = &ctx->stage_data.interacts[index];
    
    inter->id    = win_id | 0xFF;
    inter->env   = area;
    inter->flags = NAU_INTERACT_DRAG;
  }
  
  /* save window */
  {
    const uint32_t count = ctx->stage_data.win_history_count;
    
    /* udpate cache */
    if(win_cached)
    {
      const uint32_t count = ctx->stage_data.win_history_count;
      Nau_window *windows = ctx->stage_data.win_history;
      
      for(uint32_t i = 0; i < count; ++i)
      {
        if(windows[i].id == win_id)
        {
          windows[i] = window;
        }
      }
    }
    /* new window */
    else
    {
      ctx->stage_data.win_history[count] = window;
      ctx->stage_data.win_history_count += 1;
    }
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
