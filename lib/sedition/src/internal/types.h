#ifndef TYPES_INCLUDED_07B0AC05_DA3F_4387_B327_CB79998954A7
#define TYPES_INCLUDED_07B0AC05_DA3F_4387_B327_CB79998954A7


#ifdef __cplusplus
extern "C" {
#endif


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
  const char **files;
  const char **inc_dirs;
  const char **lib_dirs;
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


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
