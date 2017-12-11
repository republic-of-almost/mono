#include <sedition/sed.h>
#include <internal/array.h>
#include <internal/common.h>
#include <internal/types.h>
#include <stdio.h> /* testing */


/* ------------------------------------------------------------ [ Config ] -- */


#include <stdlib.h>
#define SED_MALLOC(bytes) malloc(bytes)


#include <string.h>
#define SED_MEMCPY(src, dest, bytes) memcpy(src, dest, bytes)


#include <assert.h>
#define SED_ASSERT(expr) assert(expr)


#include <string.h>
#define SED_ZEROMEM(ptr, type) memset(ptr, 0, type)


/* -------------------------------------------------------------- [ Data ] -- */


struct Solution *solutions  = NULL;
struct Project *projects    = NULL;
struct Config *configs      = NULL;


/* ---------------------------------------------------------- [ Solution ] -- */


int
sed_solution_create(const char *name)
{
  /* param check */
  {
    SED_ASSERT(name);
    SED_ASSERT(strlen(name) > 0);
  }

  /* create if first */
  if(solutions == NULL)
  {
    sed_array_create(solutions, 4);
  }

  /* new solution */
  {
    const size_t name_bytes = strlen(name) + 1;
    char *cpy_name = SED_MALLOC(name_bytes);
    SED_MEMCPY(cpy_name, name, name_bytes);

    struct Solution new_solution;
    SED_ZEROMEM(&new_solution, sizeof(new_solution));
    new_solution.name = cpy_name;
    
    sed_array_create(new_solution.projects, 32);
    sed_array_create(new_solution.configs, 4);

    sed_array_push(solutions, new_solution);
  }

  return sed_array_size(solutions);
}


void
sed_solution_add_project(int sol_id, int proj_id)
{
  /* param check */
  {
    SED_ASSERT(sol_id > 0);
    SED_ASSERT(sol_id <= sed_array_size(solutions));
    SED_ASSERT(proj_id > 0);
    SED_ASSERT(proj_id <= sed_array_size(projects));
  }

  /* find solution's projects */
  int *projs = NULL;
  {
    const size_t index = sol_id - 1;
    projs = solutions[index].projects;
  }

  /* scan to see if it is already added */
  {
    int count = sed_array_size(projs);
    int i;

    for(i = 0; i < count; ++i)
    {
      if(projs[i] == proj_id)
      {
        return;
      }
    }
  }

  /* add project */
  {
    sed_array_push(projs, proj_id);
  }
}


void
sed_solution_add_config(int sol_id, int config_id)
{
  /* param check */
  {
    SED_ASSERT(sol_id > 0);
    SED_ASSERT(sol_id <= sed_array_size(solutions));
    SED_ASSERT(config_id > 0);
    SED_ASSERT(config_id <= sed_array_size(configs));
  }

  /* find solution's configs */
  int *confs = NULL;
  {
    const size_t index = sol_id - 1;
    confs = solutions[index].configs;
  }

  /* scan to see if it is already added */
  {
    int count = sed_array_size(confs);
    int i;

    for(i = 0; i < count; ++i)
    {
      if(confs[i] == config_id)
      {
        return;
      }
    }
  }

  /* add config */
  {
    sed_array_push(confs, config_id);
  }
}


/* ------------------------------------------------------------ [ Config ] -- */


int
sed_config_create(const char *name)
{
  /* param check */
  {
    SED_ASSERT(name);
    SED_ASSERT(strlen(name) > 0);
  }

  /* create if first */
  if(configs == NULL)
  {
    sed_array_create(configs, 4);
  }

  /* new config */
  {
    const size_t name_bytes = strlen(name) + 1;
    char *cpy_name = SED_MALLOC(name_bytes);
    SED_MEMCPY(cpy_name, name, name_bytes);

    struct Config new_config;
    SED_ZEROMEM(&new_config, sizeof(new_config));
    new_config.name = cpy_name;

    sed_array_push(configs, new_config);
  }

  sed_array_size(configs);
}


void
sed_config_set_has_debug_symbols(int config_id, int symbols)
{
  /* param check */
  {
    SED_ASSERT(config_id > 0);
    SED_ASSERT(config_id <= sed_array_size(configs));
  }

  /* set symbols */
  {
    const size_t index = config_id - 1;
    configs[index].symbols = symbols;
  }
}


void
sed_config_set_optimisation_level(int config_id, int optim)
{
  /* param check */
  {
    SED_ASSERT(config_id > 0);
    SED_ASSERT(config_id <= sed_array_size(configs));
  }

  /* set optim level */
  {
    const size_t index = config_id - 1;
    configs[index].optim_level = optim;
  }
}


/* ----------------------------------------------------------- [ Project ] -- */


int
sed_project_create(const char *name)
{
  /* param check */
  {
    SED_ASSERT(name);
    SED_ASSERT(strlen(name) > 0);
  }

  /* new project */
  {
    if(projects == NULL)
    {
      sed_array_create(projects, 32);
    }

    const size_t name_bytes = strlen(name) + 1;
    char *cpy_name = SED_MALLOC(name_bytes);
    SED_MEMCPY(cpy_name, name, name_bytes);

    struct Project new_proj;
    SED_ZEROMEM(&new_proj, sizeof(new_proj));
    new_proj.name = cpy_name;
    
    sed_array_create(new_proj.files, 256);
    sed_array_create(new_proj.inc_dirs, 32);
    sed_array_create(new_proj.lib_dirs, 32);

    sed_array_push(projects, new_proj);
  }
}


void
sed_project_set_kind(int proj_id, int kind)
{
  /* param check */
  {
    SED_ASSERT(proj_id > 0);
    SED_ASSERT(proj_id <= sed_array_size(projects));
    SED_ASSERT(kind >= 0);
    SED_ASSERT(kind <= SED_KIND_DYNAMIC_LIB);
  }

  /* set project kind */
  {
    const size_t index = proj_id - 1;
    projects[index].kind = kind;
  }
}


void
sed_project_set_language(int proj_id, int lang)
{
  /* param check */
  {
    SED_ASSERT(proj_id > 0);
    SED_ASSERT(proj_id <= sed_array_size(projects));
    SED_ASSERT(lang >= 0);
    SED_ASSERT(lang <= SED_LANG_CPP);
  }

  /* set project language */
  {
    const size_t index = proj_id - 1;
    projects[index].lang = lang;
  }
}


void
sed_project_add_file(int proj_id, const char *file)
{
  /* param check */
  {
    SED_ASSERT(proj_id > 0);
    SED_ASSERT(proj_id <= sed_array_size(projects));
    SED_ASSERT(file);
    SED_ASSERT(strlen(file));
  }

  /* add file */
  {
    struct File new_file;
    memset(&new_file, 0, sizeof(new_file));

    const char *filename = sed_string_filename_from_path(file);
    const char *ext = sed_string_ext_from_filename(filename);
    
    const char *file_extensions[] = {
      "h",
      "H",
      "hh",
      "HH",
      "hpp",
      "HPP",

      "c",
      "C",
      "cpp",
      "CPP",
      "cc",
      "CC",
      "cxx",
      "CXX",
      "cp",
      "CP",
      "c++",
      "C++",
    };

    const int file_type[] = {
      SED_FILE_TYPE_HEADER,
      SED_FILE_TYPE_HEADER,
      SED_FILE_TYPE_HEADER,
      SED_FILE_TYPE_HEADER,
      SED_FILE_TYPE_HEADER,
      SED_FILE_TYPE_HEADER,

      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
      SED_FILE_TYPE_SOURCE,
    };

    new_file.name = sed_string(filename);
    new_file.type = SED_FILE_TYPE_TEXT;

    int ext_count = sizeof(file_extensions) / sizeof(file_extensions[0]);

    int i;

    for(i = 0; i < ext_count; ++i)
    {
      if(strcmp(file_extensions[i], ext) == 0)
      {
        new_file.type = file_type[i];
      }
    }

    const size_t index = proj_id - 1;
    sed_array_push(projects[index].files, new_file);
  } 
}


void
sed_project_add_include_dir(int proj_id, const char *dir)
{
  /* param check */
  {
    SED_ASSERT(proj_id > 0);
    SED_ASSERT(proj_id <= sed_array_size(projects));
    SED_ASSERT(dir);
    SED_ASSERT(strlen(dir));
  }

  /* add inc dir */
  {
    const size_t bytes = strlen(dir) + 1;
    char *cpy_dir = (char*)SED_MALLOC(bytes);
    SED_MEMCPY(cpy_dir, dir, bytes);

    const size_t index = proj_id - 1;
    sed_array_push(projects[index].inc_dirs, cpy_dir);
  }
}


void
sed_project_add_library_dir(int proj_id, const char *dir)
{
  /* param check */
  {
    SED_ASSERT(proj_id > 0);
    SED_ASSERT(proj_id <= sed_array_size(projects));
    SED_ASSERT(dir);
    SED_ASSERT(strlen(dir));
  }

  /* add lib dir */
  {
    const size_t bytes = strlen(dir) + 1;
    char *cpy_dir = (char*)SED_MALLOC(bytes);
    SED_MEMCPY(cpy_dir, dir, bytes);

    const size_t index = proj_id - 1;
    sed_array_push(projects[index].lib_dirs, cpy_dir);
  }
}


/* ----------------------------------------------------------- [ Execute ] -- */


#include <platform/xcode.h>


void
sed_execute(int platform)
{
  /* testing */
  int sol_count = sed_array_size(solutions);
  int i,j,k;
 
  /* loop through solutions */ 
  for(i = 0; i < sol_count; ++i)
  {
    struct Solution *sol = &solutions[i];
    int *proj_ids = sol->projects;
    int *conf_ids = sol->configs;

    /* loop through projects */
    int proj_count = sed_array_size(sol->projects);

    int conf_count = sed_array_size(sol->configs);

    printf("Solution: %s projs: %d confs: %d\n", sol->name, proj_count, conf_count);

    for(j = 0; j < proj_count; ++j)
    {
      struct Project *proj = &projects[proj_ids[j] - 1];

      printf("  Project: %s\n", proj->name);

      /* loop through files */
      int file_count = sed_array_size(proj->files);

      for(k = 0; k < file_count; ++k)
      {
        printf("    file: %s\n", proj->files[k].name);
        printf("    file type: %d\n", proj->files[k].type);
      }

      /* loop through inc dirs */
      int inc_dir_count = sed_array_size(proj->inc_dirs);

      for(k = 0; k < inc_dir_count; ++k)
      {
        printf("    inc dir: %s\n", proj->inc_dirs[k]);
      }

      /* loop through lib dirs */
      int lib_dir_count = sed_array_size(proj->lib_dirs);

      for(k = 0; k < lib_dir_count; ++k)
      {
        printf("    lib dir: %s\n", proj->lib_dirs[k]);
      }
    }

    /* loop through configs */

    for(k = 0; k < conf_count; ++k)
    {
      struct Config *conf = &configs[conf_ids[k] - 1];

      printf("  Config: %s\n", conf->name);
    }
  }

  /* generate platform solution */
  sed_generate_xcode(solutions, projects, configs); 
}


/* -------------------------------------------------------------- [ Main ] -- */


#ifndef SED_AS_LIB
int
main()
{
  /* testing */

  int solution = sed_solution_create("FooSol");
  int config = sed_config_create("DebugConfig");
  int proj = sed_project_create("BarProj");
  sed_project_set_language(proj, SED_LANG_C89);
  sed_project_set_kind(proj, SED_KIND_CONSOLE_APP);
  sed_project_add_file(proj, "../../src/foo.c");
  sed_project_add_file(proj, "../../include/foo.h");
  sed_project_add_include_dir(proj, "./include");
  sed_project_add_library_dir(proj, "/usr/local");

  sed_solution_add_config(solution, config);
  sed_solution_add_project(solution, proj);

  sed_execute(SED_PLAT_OUTPUT_ONLY);

  return 0;
}
#endif