#ifndef WINDOW_API_INCLUDED_B55BBD51_659B_4C38_957F_ACDB2B9CA6A2
#define WINDOW_API_INCLUDED_B55BBD51_659B_4C38_957F_ACDB2B9CA6A2


#include <coretech/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


/* --------------------------------------------------- [ Window API Hook ] -- */


typedef uint32_t(*ct_api_window_create_func)(struct ct_codex);
typedef void(*ct_api_window_destroy_func)(struct ct_codex, uint32_t);
typedef void(*ct_api_window_set_width_func)(struct ct_codex, uint32_t, uint32_t);
typedef uint32_t(*ct_api_window_get_width_func)(struct ct_codex, uint32_t);
typedef void(*ct_api_window_set_height_func)(struct ct_codex, uint32_t, uint32_t);
typedef uint32_t(*ct_api_window_get_height_func)(struct ct_codex, uint32_t);
typedef void(*ct_api_window_set_fullscreen_func)(struct ct_codex, uint32_t, bool);
typedef int(*ct_api_window_get_fullscreen_func)(struct ct_codex, bool);


struct ct_api_window
{
  ct_api_window_create_func create;
  ct_api_window_destroy_func destroy;

  ct_api_window_set_width_func set_width;
  ct_api_window_get_width_func get_width;

  ct_api_window_set_height_func set_height;
  ct_api_window_get_height_func get_height;

  ct_api_window_set_fullscreen_func set_fullscreen;
  ct_api_window_get_fullscreen_func get_fullscreen;
};


void      ct_window_api(struct ct_api_window *api);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
