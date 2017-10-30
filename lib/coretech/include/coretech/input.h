#ifndef INPUT_INCLUDED_030093CF_9340_4B64_BCFB_D6D0D5A2D3C0
#define INPUT_INCLUDED_030093CF_9340_4B64_BCFB_D6D0D5A2D3C0


#include <coretech/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------- [ Input Controller ] -- */


size_t    ct_controller_get_count();
float     ct_controller_get_axis_01(uint64_t input_id);
float     ct_controller_get_axis_02(uint64_t input_id);


/* ------------------------------------------------------- [ Input Mouse ] -- */


size_t    ct_mouse_get_count();
void      ct_mouse_get_delta(uint64_t input_id, float *out_x, float out_y);
void      ct_mouse_get_position(uint64_t input_id, float *out_x, float *out_y);
void      ct_mouse_set_hold(uint64_t input_id, bool hold);
bool      ct_mouse_get_hold(uint64_t input_id);


/* ---------------------------------------------------- [ Input Keyboard ] -- */


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
