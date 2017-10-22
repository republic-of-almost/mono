#ifndef WINDOW_INCLUDED_2289921C_2090_445D_AE61_6A7B644CA8AF
#define WINDOW_INCLUDED_2289921C_2090_445D_AE61_6A7B644CA8AF


/* ------------------------------------------------- [ Window Attributes ] -- */
/*
 *  A window is created when the the application is initialized. You can change
 *  its attributes here.
 *
 */


int           repo_window_get_width();
void          repo_window_set_width(int width);

int           repo_window_get_height();
void          repo_window_set_height(int height);

int           repo_window_get_fullscreen();
void          repo_window_set_fullscreen(int fullscreen);

const char *  repo_window_get_title();
void          repo_window_set_title(const char *title);


#endif /* inc guard */
