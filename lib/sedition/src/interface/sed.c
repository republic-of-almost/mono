#include <sedition/sed.h>


/* -------------------------------------------------------------- [ Data ] -- */


struct Config
{
  const char *name;
  int debug_symbols;
  int optimisation_level;
};


struct Project
{
  const char *name;
  const char *path;
};


struct Solution
{
  const char *name;
  const char *path;
  int *projects;
  int *configs;
};


Solution *solutions = NULL;
Project *projects = NULL;
Config *configs = NULL;


/* ---------------------------------------------------------- [ Solution ] -- */


int
sed_solution_create(const char *name)
{

}


/* ------------------------------------------------------------ [ Config ] -- */


int
sed_config_create(const char *name)
{
}


void
sed_config_set_has_debug_symbols(int symbols)
{
}


void
sed_config_set_optimisation_level(int optim)
{
}


/* ----------------------------------------------------------- [ Project ] -- */

int
sed_project_create(const char *name)
{
}


void
sed_project_set_language(const int lang)
{
}


void
sed_project_add_files(const char *pattern)
{
}


void
sed_project_add_file_excludes(const char *pattern)
{
}


void
sed_project_add_include_dirs(const char *pattern)
{
}


void
sed_project_add_library_dirs(const char *pattern)
{
}


/* ----------------------------------------------------------- [ Execute ] -- */


void
sed_execute()
{
}
