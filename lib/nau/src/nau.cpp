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
nau_vec2_subtract(Nau_vec2 a, Nau_vec2 b) {
  return Nau_vec2{a.x - b.x, a.y - b.y};
}

static Nau_vec2
nau_vec2_add(Nau_vec2 a, Nau_vec2 b) {
  return Nau_vec2{a.x + b.x, a.y + b.y};
}

static bool
nau_scalar_between(float val, float start, float end) {
  return val > start && val < end;
}

static float
nau_scalar_min(float a, float b) {
  return a < b ? a : b;
}

static float
nau_scalar_max(float a, float b) {
  return a > b ? a : b;
}

static Nau_env
env_from_pos_size(Nau_vec2 pos, Nau_vec2 size) {
  return Nau_env{pos, nau_vec2_add(pos, size)};
}

static float
env_width(Nau_env env) {
  return env.max.x - env.min.x;
}

static Nau_env
env_expand(Nau_env env, int scale) {
  Nau_vec2 vec{(float)scale, (float)scale};
  return Nau_env{nau_vec2_subtract(env.min, vec), nau_vec2_add(env.max, vec)};
}

static bool
nau_env_contains(Nau_env env, Nau_vec2 pos) {
  const bool between_x = nau_scalar_between(pos.x, env.min.x, env.max.x);
  const bool between_y = nau_scalar_between(pos.y, env.min.y, env.max.y);
  
  return between_x && between_y;
}

static Nau_env
nau_env_clamp(Nau_env env_to_clamp, Nau_env clamp)
{
  const float min_x = nau_scalar_max(env_to_clamp.min.x, clamp.min.x);
  const float min_y = nau_scalar_max(env_to_clamp.min.y, clamp.min.y);
  const float max_x = nau_scalar_min(env_to_clamp.max.x, clamp.max.x);
  const float max_y = nau_scalar_min(env_to_clamp.max.y, clamp.max.y);
  
//  if(min_x < 0.f || min_y < 0.f || max_x < 0.f || max_y < 0.f) {
//    return Nau_env{Nau_vec2{0.f, 0.f}, Nau_vec2{0.f, 0.f}};
//  }
  
  return Nau_env{Nau_vec2{min_x, min_y}, Nau_vec2{max_x, max_y}};
}


/*
  Nau_interactable
  --
  The properties of an interactable object.
*/
typedef enum
{
  NAU_INTERACT_NONE      = 0,
  NAU_INTERACT_DRAG      = 1 << 0,
  NAU_INTERACT_RESIZE    = 1 << 1,
  NAU_INTERACT_CLICKABLE = 1 << 2,
  NAU_INTERACT_HOLDABLE  = 1 << 3,
  NAU_INTERACT_HOVER     = 1 << 4,
  NAU_INTERACT_CLOSEABLE = 1 << 5,
  
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


/*
  Nau_window_hints
  --
  Set of hints that can be applied to a window.
*/
struct Nau_window_hints
{
  uint32_t  flags;
  float     size[2];
  float     position[2];
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
  uint32_t color_border;
  uint32_t color_win_title;
  uint32_t color_win_body;
  uint32_t color_field;
  uint32_t color_button;
  uint32_t color_button_hover;
  uint32_t color_button_down;
  uint32_t color_drag_handle;
  uint32_t color_close_button;
  
  int32_t size_border;
  int32_t size_inner_margin;
  int32_t size_row_height;
  int32_t size_row_spacing;
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
  Nau_iteract_flags  interacts_type;
  Nau_vec2           interacts_coords;
  
  Nau_window         active_window;
  Nau_vec2           cursor;
  float              window_margin;
  Nau_env            child_env;
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
  Nau_window_hints      window_hints;
};


/* helper functions */
static bool
nau_has_active_window(Nau_ctx *c) { return c->stage_data.active_window.id > 0; }


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
nau_submit_raw_verts_cmd(Nau_ctx *ctx, Nau_env clip, float *pos_vec2, float *uv_vec2, uint32_t *index, uint32_t vert_count, uint32_t color)
{
  /* some info */
  const size_t vertex_stride = 3 + 2 + 4;
  const size_t offset = ctx->draw_data.vbo_count / vertex_stride;

  /* add vertex information */
  {
    size_t index = ctx->draw_data.vbo_count;
    
    const float r = ((color >> 24) & 0xFF) / 255.f;
    const float g = ((color >> 16) & 0xFF) / 255.f;
    const float b = ((color >> 8) & 0xFF) / 255.f;
    const float a = ((color >> 0) & 0xFF) / 255.f;
    
    /* position / uv / color */
    
    ctx->draw_data.vbo[index++] = pos_vec2[0];
    ctx->draw_data.vbo[index++] = pos_vec2[1];
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = r;
    ctx->draw_data.vbo[index++] = g;
    ctx->draw_data.vbo[index++] = b;
    ctx->draw_data.vbo[index++] = a;
    
    ctx->draw_data.vbo[index++] = uv_vec2[0];
    ctx->draw_data.vbo[index++] = uv_vec2[1];
    
    /**/
    
    ctx->draw_data.vbo[index++] = pos_vec2[2];
    ctx->draw_data.vbo[index++] = pos_vec2[3];
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = r;
    ctx->draw_data.vbo[index++] = g;
    ctx->draw_data.vbo[index++] = b;
    ctx->draw_data.vbo[index++] = a;
    
    ctx->draw_data.vbo[index++] = uv_vec2[2];
    ctx->draw_data.vbo[index++] = uv_vec2[3];
    
    /**/
    
    ctx->draw_data.vbo[index++] = pos_vec2[4];
    ctx->draw_data.vbo[index++] = pos_vec2[5];
    ctx->draw_data.vbo[index++] = 1.f;
    
    ctx->draw_data.vbo[index++] = r;
    ctx->draw_data.vbo[index++] = g;
    ctx->draw_data.vbo[index++] = b;
    ctx->draw_data.vbo[index++] = a;
    
    ctx->draw_data.vbo[index++] = uv_vec2[4];
    ctx->draw_data.vbo[index++] = uv_vec2[5];
    
    if(vert_count > 3) // quad
    {
      ctx->draw_data.vbo[index++] = pos_vec2[6];
      ctx->draw_data.vbo[index++] = pos_vec2[7];
      ctx->draw_data.vbo[index++] = 1.f;
      
      ctx->draw_data.vbo[index++] = r;
      ctx->draw_data.vbo[index++] = g;
      ctx->draw_data.vbo[index++] = b;
      ctx->draw_data.vbo[index++] = a;
      
      ctx->draw_data.vbo[index++] = uv_vec2[6];
      ctx->draw_data.vbo[index++] = uv_vec2[7];
    }
    
    /**/
    
    ctx->draw_data.vbo_count = index;
  }
  
  /* add index information */
  uint32_t index_count = 0;
  {
    size_t index = ctx->draw_data.idx_count;
    
    ctx->draw_data.idx[index++] = offset + 0;
    ctx->draw_data.idx[index++] = offset + 1;
    ctx->draw_data.idx[index++] = offset + 2;
    
    index_count = 3;
  
    if(vert_count > 3) // quad
    {
      ctx->draw_data.idx[index++] = offset + 1;
      ctx->draw_data.idx[index++] = offset + 2;
      ctx->draw_data.idx[index++] = offset + 3;
      
      index_count = 6;
    }
  
  
    ctx->draw_data.idx_count = index;
  }
  
  /* draw cmd */
  {
    const int index = ctx->draw_data.cmd_count;
    
    int cmd_clip[4] { (int)clip.min.x, (int)clip.min.y, (int)clip.max.x, (int)clip.max.y };
    
    memcpy(&ctx->draw_data.cmds[index].clip, cmd_clip, sizeof(cmd_clip));
    ctx->draw_data.cmds[index].count  = index_count;
    ctx->draw_data.cmds[index].offset = ctx->draw_data.idx_count - index_count;
    
    ctx->draw_data.cmd_count += 1;
  }

}


static void
nau_submit_cmd(Nau_ctx *ctx, Nau_env area, Nau_env clip, uint32_t color)
{

  float verts[] {
    area.min.x, area.min.y,
    area.max.x, area.min.y,
    area.min.x, area.max.y,
    area.max.x, area.max.y,
  };
  
  float uvs[] {
    0.f, 0.f,
    0.f, 0.f,
    0.f, 0.f,
    0.f, 0.f,
  };
  
  uint32_t index[] {
    0,1,2,
    1,2,3,
  };

  nau_submit_raw_verts_cmd(ctx, clip, verts, uvs, index, 4, color);
}


static void
nau_default_hints(Nau_ctx *ctx)
{
  ctx->window_hints.flags = 0;
  ctx->window_hints.size[0] = 100.f;
  ctx->window_hints.size[1] = 150.f;
  ctx->window_hints.position[0] = 10.f * (ctx->stage_data.win_history_count + 1.f);
  ctx->window_hints.position[1] = 10.f * (ctx->stage_data.win_history_count + 1.f);
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
  
  /* default theme */
  {
    new_ctx->theme.color_border         = 0xE26700FF;
    new_ctx->theme.color_win_body       = 0x303030FF;
    new_ctx->theme.color_win_title      = 0x575757FF;
    new_ctx->theme.color_field          = 0xFFFFFFFF;
    new_ctx->theme.color_button         = 0xE26700FF;
    new_ctx->theme.color_button_hover   = 0xFF7964FF;
    new_ctx->theme.color_button_down    = 0xEB3D00FF;
    new_ctx->theme.color_drag_handle    = 0xE26700FF;
    new_ctx->theme.color_close_button   = 0xE26700FF;
    
    new_ctx->theme.size_border       = 1;
    new_ctx->theme.size_inner_margin = 3;
    new_ctx->theme.size_row_height   = 20;
    new_ctx->theme.size_row_spacing  = 4;
  }
  
  /* hints */
  {
    nau_default_hints(new_ctx);
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
  
  /* update interactions */
  {
    const bool is_held  = ctx->device.ptr_action == NAU_MS_ACTION_HOLD;
    const bool is_click = ctx->device.ptr_action == NAU_MS_ACTION_CLICK;
    const bool is_down  = (ctx->device.ptr_action == NAU_MS_ACTION_DOWN) | is_held | is_click;
    
    /* drag and resize need to continue to be active */
    if(!is_held)
    {
      ctx->stage_data.interacts_current = 0;
      ctx->stage_data.interacts_type = NAU_INTERACT_NONE;
    }
    
    if((is_held || is_click) && ctx->stage_data.interacts_current == 0)
    {
      const uint32_t count = ctx->stage_data.interacts_count;
      const Nau_interactable *interact = ctx->stage_data.interacts;
      const Nau_vec2 point = ctx->device.ptr_pos;
      
      for(int32_t i = count - 1; i > -1; --i)
      {
        if(nau_env_contains(interact[i].env, point))
        {
          if(is_click && (interact[i].flags & NAU_INTERACT_CLICKABLE))
          {
            ctx->stage_data.interacts_current = interact[i].id;
            ctx->stage_data.interacts_type = NAU_INTERACT_CLICKABLE;
          }
          else if(is_held && (interact[i].flags & NAU_INTERACT_DRAG))
          {
            ctx->stage_data.interacts_current = interact[i].id;
            ctx->stage_data.interacts_type = NAU_INTERACT_DRAG;
          }
          else if(is_held && (interact[i].flags & NAU_INTERACT_RESIZE))
          {
            ctx->stage_data.interacts_current = interact[i].id;
            ctx->stage_data.interacts_type = NAU_INTERACT_RESIZE;
          }
          else if(is_held && (interact[i].flags & NAU_INTERACT_HOLDABLE))
          {
            ctx->stage_data.interacts_current = interact[i].id;
            ctx->stage_data.interacts_type = NAU_INTERACT_HOLDABLE;
          }
          else if(is_held && (interact[i].flags & NAU_INTERACT_CLOSEABLE))
          {
            ctx->stage_data.interacts_current = interact[i].id;
            ctx->stage_data.interacts_type = NAU_INTERACT_CLOSEABLE;
          }
          
          break;
        }
      }
    }
    
    /* Look for something to hover over */
    if(!is_down && ctx->stage_data.interacts_current == 0)
    {
      const uint32_t count = ctx->stage_data.interacts_count;
      const Nau_interactable *interact = ctx->stage_data.interacts;
      const Nau_vec2 point = ctx->device.ptr_pos;
      
      for(int32_t i = count - 1; i > -1; --i)
      {
        if(nau_env_contains(interact[i].env, point))
        {
          ctx->stage_data.interacts_current = interact[i].id;
          ctx->stage_data.interacts_type = NAU_INTERACT_HOVER;
          
          break;
        }
      }
    }
  }
  
  /* update input */
  {
    ctx->device.ptr_diff = nau_vec2_subtract(ctx->device.ptr_pos, ctx->device.ptr_last_pos);
    ctx->device.ptr_last_pos = ctx->device.ptr_pos;
    
    const bool is_down = ctx->device.ptr_action == NAU_MS_ACTION_DOWN;
    ctx->device.ptr_action = is_down ? NAU_MS_ACTION_HOLD : ctx->device.ptr_action;
    
    const bool is_click = ctx->device.ptr_action == NAU_MS_ACTION_CLICK;
    ctx->device.ptr_action = is_click ? NAU_MS_ACTION_UP : ctx->device.ptr_action;
    
    ctx->stage_data.interacts_coords = ctx->device.ptr_pos;
  }
  
  /* reset the context */
  {
    ctx->stage_data.interacts_count = 0;
    
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
nau_win_prop_flags(Nau_ctx *ctx, int flags)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }
  
  ctx->window_hints.flags = flags;
}

void
nau_win_prop_size(Nau_ctx *ctx, float size[2])
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }
  
  memcpy(ctx->window_hints.size, size, sizeof(ctx->window_hints.size));
}

void
nau_win_prop_position(Nau_ctx *ctx, float pos[2])
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
  }
  
  memcpy(ctx->window_hints.position, pos, sizeof(ctx->window_hints.position));
}


void
nau_begin(Nau_ctx *ctx, const char *name, bool *close_handle)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL);
    NAU_ASSERT(name != NULL);
    NAU_ASSERT(strlen(name) > 0);
  }

  /* window properties */
  const uint32_t flags = ctx->window_hints.flags;
  
  const bool can_resize = !(flags & NAU_WIN_NO_RESIZE);
  const bool can_drag   = !(flags & NAU_WIN_NO_DRAG);
  const bool can_close  = !(flags & NAU_WIN_NO_CLOSE);

  /* window id */
  bool win_cached = false;
  
  Nau_window window;
  window.id = nau_hash(name, strlen(name), 0);
  
  /* get window history */
  {
    const uint32_t count = ctx->stage_data.win_history_count;
    const Nau_window *windows = ctx->stage_data.win_history;
    
    for(uint32_t i = 0; i < count; ++i)
    {
      if(windows[i].id == window.id)
      {
        window = windows[i];
        win_cached = true;
      }
    }
    
    if(!win_cached)
    {
      window.pos.x = ctx->window_hints.position[0];
      window.pos.y = ctx->window_hints.position[1];
      
      window.size.x = ctx->window_hints.size[0];
      window.size.y = ctx->window_hints.size[1];
    }
  }
  
  Nau_env area = env_from_pos_size(window.pos, window.size);
  
  /* check interacts */
  {
    const uint64_t curr_interact = ctx->stage_data.interacts_current;
    const Nau_iteract_flags curr_interact_type = ctx->stage_data.interacts_type;
    const uint64_t id = window.id | 0xFF;
    
    if(curr_interact == id && curr_interact_type == NAU_INTERACT_DRAG)
    {
      window.pos = nau_vec2_add(window.pos, ctx->device.ptr_diff);
      
      // Reset area
      area = env_from_pos_size(window.pos, window.size);
    }
    else if(curr_interact == id && curr_interact_type == NAU_INTERACT_RESIZE)
    {
      window.size = nau_vec2_add(window.size, ctx->device.ptr_diff);
      
      // Reset size
      area = env_from_pos_size(window.pos, window.size);
    }
    else if(curr_interact == id && curr_interact_type == NAU_INTERACT_CLOSEABLE)
    {
      if(close_handle)
      {
        *close_handle = false;
      }
    }
  }
  
  /* active window */
  {	
    ctx->stage_data.active_window = window;
    ctx->stage_data.cursor = window.pos;
  }
  
  /* main body */
  {
    /* border */
    {
      const uint32_t color = ctx->theme.color_border;
      
      nau_submit_cmd(ctx, area, area, color);
    }
    
    /* window body */
    {
      const Nau_env body_env = env_expand(area, -ctx->theme.size_border);
      ctx->stage_data.child_env = body_env;
      const uint32_t color = ctx->theme.color_win_body;
    
      nau_submit_cmd(ctx, body_env, body_env, color);
      
      ctx->stage_data.cursor = body_env.min;
    }
    
    /* submit drag interactable */
    if(can_drag)
    {
      const int index = ctx->stage_data.interacts_count;
      ++ctx->stage_data.interacts_count;
      
      Nau_interactable *inter = &ctx->stage_data.interacts[index];
      
      inter->id    = window.id | 0xFF;
      inter->env   = area;
      inter->flags = NAU_INTERACT_DRAG;
    }
  }
  
  /* title */
  {
    /* name bar */
    {
      const Nau_vec2 title_pos = ctx->stage_data.cursor;
      
      const float width = env_width(ctx->stage_data.child_env);
      const float height = (float)ctx->theme.size_row_height;
      const Nau_vec2 title_size{width, height};
      
      const Nau_env title_area = env_from_pos_size(
        title_pos,
        title_size
      );
      
      const uint32_t color = ctx->theme.color_win_title;
    
      nau_submit_cmd(ctx, title_area, title_area, color);
    }
    
    /* close button */
    if(can_close)
    {
      const float button_size = 10.f;
      const float half_button_size = button_size * 0.5f;
    
      const Nau_vec2 top_left_corner = ctx->stage_data.cursor;
      const Nau_vec2 width           = Nau_vec2{window.size.x - button_size,0.f};
      const Nau_vec2 offset          = Nau_vec2{-half_button_size - half_button_size, (ctx->theme.size_row_height * 0.5f) - half_button_size};
      const Nau_vec2 header_corner   = nau_vec2_add(nau_vec2_add(width, top_left_corner), offset);

      const Nau_env close_button = env_from_pos_size(
        header_corner,
        Nau_vec2{button_size, button_size}
      );

      const uint32_t color = ctx->theme.color_close_button;

      nau_submit_cmd(ctx, close_button, close_button, color);

      /* interactable */
      const int index = ctx->stage_data.interacts_count;
      ++ctx->stage_data.interacts_count;

      Nau_interactable *inter = &ctx->stage_data.interacts[index];

      inter->id    = window.id | 0xFF;
      inter->env   = close_button;
      inter->flags = NAU_INTERACT_CLOSEABLE;
    }
    
    /* minimize button */
    {
    }
    
    nau_line_break(ctx);
  }
  
  
  /* footer */
  if(can_resize)
  {
    /* handle size change */
    {
    }
  
    /* resize handle */
    {
      const float button_size = 17.f;
    
      const Nau_vec2 height        = Nau_vec2{0.f, window.size.y - button_size};
      const Nau_vec2 width         = Nau_vec2{window.size.x - button_size,0.f};
      const Nau_vec2 footer_corner = nau_vec2_add(width, nau_vec2_add(window.pos, height));
      
      const Nau_env drag_handle = env_from_pos_size(
        footer_corner,
        Nau_vec2{button_size, button_size}
      );
      
      const uint32_t color = ctx->theme.color_drag_handle;
      
      float verts[] {
        drag_handle.max.x, drag_handle.min.y,
        drag_handle.max.x, drag_handle.max.y,
        drag_handle.min.x, drag_handle.max.y,
      };
      
      float uvs[] {
        0.f, 0.f,
        0.f, 0.f,
        0.f, 0.f,
      };
      
      uint32_t idx[] {
        0,1,2,
      };
      
      nau_submit_raw_verts_cmd(ctx, drag_handle, verts, uvs, idx, 3, color);
      
      /* interactable */
      const int index = ctx->stage_data.interacts_count;
      ++ctx->stage_data.interacts_count;
      
      Nau_interactable *inter = &ctx->stage_data.interacts[index];
      
      inter->id    = window.id | 0xFF;
      inter->env   = drag_handle;
      inter->flags = NAU_INTERACT_RESIZE;
    }
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
        if(windows[i].id == window.id)
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
    NAU_ASSERT(ctx != NULL); // valid context
    NAU_ASSERT(nau_has_active_window(ctx) == true); // did you miss a begin call
  }
  
  /* end active window */
  {
    nau_default_hints(ctx);
    ctx->stage_data.active_window = Nau_window{};
  }
}


/* ----------------------------------------------------------- [ Widgets ] -- */


void
nau_text(Nau_ctx *ctx, const char *text)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL); // valid context
    NAU_ASSERT(nau_has_active_window(ctx) == true); // did you miss a begin call
    NAU_ASSERT(text != NULL); // need string to render
  }
}


bool
nau_button(Nau_ctx *ctx, const char *name)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL); // valid context
    NAU_ASSERT(nau_has_active_window(ctx) == true); // did you miss a begin call
    NAU_ASSERT(name != NULL); // need a name
    NAU_ASSERT(strlen(name) > 0); // need a name
  }
  
  uint64_t interact_id = 0;
  const Nau_window window = ctx->stage_data.active_window;
  
  /* interact id */
  {
    interact_id = window.id | nau_hash(name, strlen(name), 0);
  }
  
  /* check was clicked on */
  bool is_clicked = false;
  bool is_down    = false;
  bool is_hover   = false;
  {
    const bool id_match    = ctx->stage_data.interacts_current == interact_id;
    const bool is_clicking = ctx->stage_data.interacts_type == NAU_INTERACT_CLICKABLE;
    const bool is_held     = ctx->stage_data.interacts_type == NAU_INTERACT_HOLDABLE;
    const bool is_hovering = ctx->stage_data.interacts_type == NAU_INTERACT_HOVER;
  
    if(id_match && is_clicking)
    {
      is_clicked = true;
    }
    else if(id_match && is_held)
    {
      is_down = true;
    }
    else if(id_match && is_hovering)
    {
      is_hover = true;
    }
  }
  
  /* add button */
  {
    uint32_t color = ctx->theme.color_button;
    if(is_clicked)
    {
      color = ctx->theme.color_button_down;
    }
    else if(is_down)
    {
      color = ctx->theme.color_button_down;
    }
    else if(is_hover)
    {
      color = ctx->theme.color_button_hover;
    }
  
    Nau_vec2 l_margin = ctx->stage_data.cursor;
    l_margin.x += ctx->theme.size_inner_margin;
    
    Nau_vec2 size = window.size;
    size.y = ctx->theme.size_row_height;
    size.x -= ((ctx->theme.size_inner_margin + ctx->theme.size_border) * 2);
    
    Nau_env area = env_from_pos_size(l_margin, size);
    Nau_env clamped_area = nau_env_clamp(area, ctx->stage_data.child_env);
    
    nau_submit_cmd(ctx, area, clamped_area, color);
    
    /* add interactable */
    {
      const int index = ctx->stage_data.interacts_count;
      ++ctx->stage_data.interacts_count;
      
      Nau_interactable *inter = &ctx->stage_data.interacts[index];
      
      inter->id    = interact_id;
      inter->env   = area;
      inter->flags = NAU_INTERACT_CLICKABLE | NAU_INTERACT_HOLDABLE;
    }
    
    nau_line_break(ctx);
  }
  
  return is_clicked;
}


/* -------------------------------------------------------- [ Formatting ] -- */


void
nau_line_break(Nau_ctx *ctx)
{
  /* param check */
  {
    NAU_ASSERT(ctx != NULL); // valid context
    NAU_ASSERT(nau_has_active_window(ctx) == true); // forgot a begin?
  }
  
  /* move cursor down */
  {
    ctx->stage_data.cursor.y += (ctx->theme.size_row_height + ctx->theme.size_row_spacing);
  }
}


/* ------------------------------------------------------------ [ Config ] -- */


#undef NAU_MALLOC
#undef NAU_ASSERT
#undef NAU_LOGGING
#undef NAU_ZERO_MEM
