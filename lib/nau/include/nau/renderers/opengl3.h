#ifndef OPEN_GL3_INCLUDED_F0DFC029_F493_471A_B85E_B9C512D3ABF9
#define OPEN_GL3_INCLUDED_F0DFC029_F493_471A_B85E_B9C512D3ABF9


#include <nau/nau.h>


// ------------------------------------------------------------- [ Lifetime ] --


void      nau_gl3_init();
void      nau_gl3_render(Nau_draw_cmd *renderables, int count);
void      nau_gl3_destroy();



#endif // inc guard
