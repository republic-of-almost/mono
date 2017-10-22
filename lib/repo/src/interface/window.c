#include <repo/window.h>


/* ------------------------------------------------- [ Window Attributes ] -- */


int
repo_window_get_width()
{
  return data_get_int(data_ctx, REPO_WINDOW_WIDTH);
}


void
repo_window_set_width(int width)
{
  data_set_int(data_ctx, REPO_WINDOW_WIDTH, width);
}


int
repo_window_get_height()
{
  return data_get_int(data_ctx, REPO_WINDOW_HEIGHT);
}


void
repo_window_set_height(int height)
{
  data_set_int(data_ctx, REPO_WINDOW_HEIGHT, height);
}


int
repo_window_get_fullscreen()
{
  return data_get_bool(data_ctx, REPO_WINDOW_FULLSCREEN);
}


void
repo_window_set_fullscreen(int fullscreen)
{
  data_set_bool(data_ctx, REPO_WINDOW_FULLSCREEN, (bool)fullscreen);
}


const char *
repo_window_get_title()
{
  data_get_string(data_ctx, REPO_WINDOW_TITLE);
}


void
repo_window_set_title(const char *title)
{
  data_set_string(data_ctx, REPO_WINDOW_TITLE, title);
}

