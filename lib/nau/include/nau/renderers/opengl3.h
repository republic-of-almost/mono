#ifndef OPEN_GL3_INCLUDED_F0DFC029_F493_471A_B85E_B9C512D3ABF9
#define OPEN_GL3_INCLUDED_F0DFC029_F493_471A_B85E_B9C512D3ABF9


#include <nau/nau.h>


// ------------------------------------------------------------- [ Lifetime ] --


struct Nau_gl_ctx;


void        nau_gl3_init(Nau_gl_ctx **gl_ctx, Nau_ctx *ui_ctx);
void        nau_gl3_render(Nau_gl_ctx *gl_ctx, Nau_ctx *ui_ctx);
void        nau_gl3_destroy(Nau_gl_ctx **gl_ctx);



#endif // inc guard
