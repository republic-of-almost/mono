#ifndef SEDITION_INCLUDED_80449395_87A2_45B9_A22F_236B2037BC20
#define SEDITION_INCLUDED_80449395_87A2_45B9_A22F_236B2037BC20


#ifdef __cplusplus
extern "C" {
#endif


/*
 *  Sedition Interface
 *  --
 *  By default sedition is command line program that has its own main function.
 *  So this interface isn't much use in that setup.
 *
 *  If you build Sedition as a library you should define SED_AS_LIB, then you
 *  can use this interface.
 */


/* ---------------------------------------------------------- [ Solution ] -- */


int       sed_solution_create(const char *name);
void      sed_solution_add_project(int sol_id, int proj_id);
void      sed_solution_add_config(int sol_id, int config_id);


/* ------------------------------------------------------------ [ Config ] -- */


int       sed_config_create(const char *name);
void      sed_config_set_has_debug_symbols(int config_id, int symbols);
void      sed_config_set_optimisation_level(int config_id, int optim);


/* ----------------------------------------------------------- [ Project ] -- */


enum {
  SED_LANG_C89,
  SED_LANG_C99,
  SED_LANG_C11,
  SED_LANG_C,
  SED_LANG_CPP11,
  SED_LANG_CPP14,
  SED_LANG_CPP17,
  SED_LANG_CPP,
};


enum {
  SED_KIND_CONSOLE_APP,
  SED_KIND_WINDOW_APP,
  SED_KIND_STATIC_LIB,
  SED_KIND_DYNAMIC_LIB,
};


int       sed_project_create(const char *name);
void      sed_project_set_kind(int proj_id, int kind);
void      sed_project_set_language(int proj_id, int lang);
void      sed_project_add_file(int proj_id, const char *file);
void      sed_project_add_include_dirs(int proj_id, const char *dir);
void      sed_project_add_library_dirs(int proj_id, const char *dir);


/* ----------------------------------------------------------- [ Execute ] -- */


enum {
  SED_PLAT_XCODE,
  SED_PLAT_VISUAL_STUDIO_2017,
};


void      sed_execute(int platform);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
