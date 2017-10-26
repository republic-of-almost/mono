#ifndef WINDOW_INCLUDED_2289921C_2090_445D_AE61_6A7B644CA8AF
#define WINDOW_INCLUDED_2289921C_2090_445D_AE61_6A7B644CA8AF


#ifdef __cplusplus
extern "C" {
#endif


#include <inttypes.h>


/* ------------------------------------------------- [ Window Attributes ] -- */


uint64_t      repo_window_create();
void          repo_window_destroy(uint64_t id);

uint32_t      repo_window_get_width(uint64_t id);
void          repo_window_set_width(uint64_t id, int width);

uint32_t      repo_window_get_height(uint64_t id);
void          repo_window_set_height(uint64_t id, int height);

int           repo_window_get_fullscreen(uint64_t id);
void          repo_window_set_fullscreen(uint64_t id, int fullscreen);

const char *  repo_window_get_title(uint64_t id);
void          repo_window_set_title(uint64_t id, const char *title);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
