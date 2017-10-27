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
          
          dir_name = sed_string_append(dir_name, ".xcodeproj");

          sed_mkdir(dir_name);

          sed_string_free(dir_name);
        }
      }

      /* workspace */
      {
        char *dir_name = sed_string(sol->name);
        dir_name = sed_string_append(dir_name, ".xcworkspace");

        sed_mkdir(dir_name);

        char *work_file = sed_string(dir_name);
        work_file = sed_string_append(work_file, "/contents.xcworkspacedata");

        sed_file_create(work_file);
        sed_file_close();

        sed_string_free(work_file);
        sed_string_free(dir_name);
      }
    } /* generate workspace */
  } /* for solutions */
} /* sed_generate_xcode */
