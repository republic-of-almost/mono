#ifndef PLUGIN_INCLUDED_4FC9CED8_9AA3_4E21_B1CF_8ED00CEEAE93
#define PLUGIN_INCLUDED_4FC9CED8_9AA3_4E21_B1CF_8ED00CEEAE93


/* -------------------------------------------------- [ Plugin Interface ] -- */


void
repo_plugins_load(const char **api_names, const void **api_functions, unsigned count);

void
repo_plugins_unload();


#endif /* inc guard */
