#include <nau/nau.h>
#include <vector>


namespace {


struct Nau_ctx
{
  uint32_t viewport[2];
  
  std::vector<Nau_renderable> renderable;
  
  
  // -- Colors -- //
  
  uint32_t inactive_window_color;
  uint32_t active_window_color;
};


inline Nau_ctx&
get_ctx()
{
  static Nau_ctx ctx;
  
  return &ctx;
}


} // anon ns



// ------------------------------------------------------------- [ Lifetime ] --


void
nau_init()
{
  
}


void
nau_destroy()
{
  
}


void
nau_new_frame()
{
  get_ctx().renderable.clear();
}


void
nau_render_data(Nau_renderable **renderables, size_t *count)
{
  *renderables = get_ctx().renderable.data();
  *count       = get_ctx().renderable.size();
}


// ------------------------------------------------------------- [ Settings ] --


void
nau_set_viewport(const uint32_t width, const uint32_t height)
{
  get_ctx().viewport[0] = width;
  get_ctx().viewport[1] = height;
}


void
nau_set_color_inactive_window(const uint32_t color)
{
  get_ctx().inactive_window_color = color;
}


void
nau_set_color_active_window(const uint32_t color)
{
  get_ctx().active_window_color = color;
}


// --------------------------------------------------------------- [ Window ] --


void
nau_begin(const char *name)
{
  get_ctx().renderable.emplace_back(
    Nau_renderable{
      { 10, 10, 15, 20 },
      { 10, 10, 15, 20 },
      inactive_window_color,
    }
  );
}


void
nau_end()
{
}


// -------------------------------------------------------------- [ Widgets ] --


void
nau_float(const char *name, float *data)
{
}


