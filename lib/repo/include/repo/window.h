#ifndef WINDOW_INCLUDED_2289921C_2090_445D_AE61_6A7B644CA8AF
#define WINDOW_INCLUDED_2289921C_2090_445D_AE61_6A7B644CA8AF


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Window Data ] -- */

struct repo_window_desc
{
  unsigned width;      
  unsigned height;
  unsigned fullscreen;  /* 0 or 1 */
};

/* ------------------------------------------------------- [ Window Desc ] -- */

void
repo_window_get_desc(struct repo_window_desc *out_window);


void
repo_window_set_desc(const struct repo_window_desc *in_desc);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
