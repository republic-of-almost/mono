#ifndef WINDOW_INCLUDED_B832065A_6E8F_4A8B_B0C3_B51C3D724382
#define WINDOW_INCLUDED_B832065A_6E8F_4A8B_B0C3_B51C3D724382


#include <coretech/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


/* --------------------------------------------------- [ Window Lifetime ] -- */


uint64_t    ct_window_create();
void        ct_window_destroy(uint64_t win_id);


/* ------------------------------------------------- [ Window Attributes ] -- */


void        ct_window_set_width(uint64_t win_id, uint32_t width);
uint32_t    ct_window_get_width(uint64_t win_id);

void        ct_window_set_height(uint64_t win_id, uint32_t height);
uint32_t    ct_window_get_height(uint64_t win_id);

void        ct_window_set_fullscreen(uint64_t win_id, bool fullscreen);
bool        ct_window_get_fullscreen(uint64_t win_id);


#ifdef __cplusplus
}
#endif



#endif /* inc guard */
