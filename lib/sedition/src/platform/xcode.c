#include <platform/xcode.h>
#include <internal/types.h>
#include <internal/common.h>
#include <internal/array.h>
#include <string.h>
#include <stdio.h>


void sed_generate_xcode(
  struct Solution *sols,
  struct Project *projs,
  struct Config *confs)
{
  /* generating xcode */
  int sol_count = sed_array_size(sols);

  int i;
  for(i = 0; i < sol_count; ++i)
  {
    struct Solution *sol = &sols[i];

    /* generate workspace */
    {
      /* project */
      {
        int proj_count = sed_array_size(sol->projects);
        int j;

        for(j = 0; j < proj_count; ++j)
        {
          struct Project *proj = &projs[sol->projects[i -1]];

          char *dir_name = sed_string(proj->name);
          printf("dir: %s/n", dir_name);
          
          const char *ext = ".xcodeproj";
          dir_name = sed_string_append(dir_name, ext);

          sed_mkdir(dir_name);

          printf("dir: %s/n", dir_name);
        }
      }

      /* workspace */
      {
        const char *suf = ".xcworkspace";
        size_t ws_bytes = strlen(sol->name) + strlen(suf) + 1;
        char *ws_name = (char *)sed_malloc(ws_bytes);
        memset(ws_name, 0, ws_bytes);

        strcat(ws_name, sol->name);
        strcat(ws_name, suf); 

        sed_mkdir(ws_name);

        
      }
    } /* generate workspace */
  } /* for solutions */
} /* sed_generate_xcode */
