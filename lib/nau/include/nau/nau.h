#ifndef NAU_INCLUDED_38EB9B7F_8D78_411D_8EF1_E08A411AC701
#define NAU_INCLUDED_38EB9B7F_8D78_411D_8EF1_E08A411AC701


#include <stdint.h>
#include <stddef.h>


// ----------------------------------------------------------------- [ Data ] --


struct Nau_renderable
{
  int32_t     box[4];
  int32_t     clip[4];
  uint32_t    color;
};


// ------------------------------------------------------------- [ Lifetime ] --


void      nau_init();
void      nau_destroy();
void      nau_new_frame();
void      nau_render_data(Nau_renderable **renderables, size_t *count);


// ------------------------------------------------------------- [ Settings ] --


void      nau_set_viewport(const uint32_t width, const uint32_t height);
void      nau_set_color_inactive_window(const uint32_t color);
void      nau_set_color_active_window(const uint32_t color);


// --------------------------------------------------------------- [ Window ] --


void      nau_begin(const char *name);
void      nau_end();


// -------------------------------------------------------------- [ Widgets ] --


void      nau_float(const char *name, float *data);


#endif // inc guard
