#ifndef WINDOW_INCLUDED_2289921C_2090_445D_AE61_6A7B644CA8AF
#define WINDOW_INCLUDED_2289921C_2090_445D_AE61_6A7B644CA8AF


#ifdef __cplusplus
extern "C" {
#endif


#include <inttypes.h>

/* ------------------------------------------------------- [ Window Data ] -- */

struct repo_desc_window
{
  uint32_t  width;
  uint32_t  height;
  bool      fullscreen;
};


/* --------------------------------------------------- [ Window Lifetime ] -- */

uint64_t        repo_window_create(struct repo_desc_window *window);
void            repo_window_destroy(uint64_t win_id);


/* ------------------------------------------------------- [ Window Desc ] -- */

bool            repo_window_get_desc(uint64_t win_id, struct repo_desc_window *window);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
