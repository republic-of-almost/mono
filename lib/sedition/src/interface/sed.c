#include <sedition/sed.h>
#include <internal/array.h>


/* ------------------------------------------------------------ [ Config ] -- */


#include <stdlib.h>
#define SED_MALLOC(bytes) malloc(bytes)


#include <string.h>
#define SED_MEMCPY(src, dest, bytes) memcpy(src, dest, bytes)


#include <assert.h>
#define SED_ASSERT(expr) assert(expr)


/* -------------------------------------------------------------- [ Data ] -- */


struct Config
{
  const char *name;
  int symbols;
  int optim_level;
};


struct Project
{
  const char *name;
  const char *path;
  int lang;
  int kind;
};


struct Solution
{
  const char *name;
  const char *path;
  int *projects;
  int *configs;
};


struct Solution *solutions = NULL;
struct Project *projects = NULL;
struct Config *configs = NULL;


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
    new_proj.name = cpy_name;
    
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
}


void
sed_project_add_include_dirs(int proj_id, const char *dir)
{
}


void
sed_project_add_library_dirs(int proj_id, const char *dir)
{
}


/* ----------------------------------------------------------- [ Execute ] -- */


void
sed_execute()
{
}


/* -------------------------------------------------------------- [ Main ] -- */


int
main()
{
  /* testing */
  int solution = sed_solution_create("FooSol");
  int config = sed_config_create("DebugConfig");
  int proj = sed_project_create("BarProj");

 

  return 0;
}
