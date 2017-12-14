#include <plugin/plugin.h>
#include <repo/repo.h>
#include <assert.h>
#include <string.h>
#include <tinydir/tinydir.h>
#include <lib/string.hpp>
#include <lib/file.hpp>

#ifndef _WIN32
#include <dlfcn.h>
#else
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <tchar.h>
#endif


#ifndef _WIN32
using roa_dlib_handle = void*;
#else
using roa_dlib_handle = HINSTANCE;
#endif


roa_dlib_handle
#ifndef _WIN32
roa_dlib_open(const char *name)
#else
roa_dlib_open(const TCHAR *name)
#endif
{
  #ifndef _WIN32
  return dlopen(name, RTLD_LAZY);
  #else
  return LoadLibrary(name);
  #endif
}


void*
roa_dlib_get_address(roa_dlib_handle handle, const char *name)
{
  #ifndef _WIN32
  return dlsym(handle, name);
  #else
  return GetProcAddress(handle, name);
  #endif
}


const char **api_name_list = nullptr;
void **api_func_list = nullptr;
unsigned api_count = 0;


void*
repo_api_loader_impl(const char *api_name)
{
  /* be super careful here */
  for(unsigned i = 0; i < api_count; ++i)
  {
    if(strcmp(api_name, api_name_list[i]) == 0) {
      return (void*)api_func_list[i];
    }
  }
  
  return 0;
}


/* -------------------------------------------------- [ Plugin Interface ] -- */


void
repo_plugins_load(const char **api_names, void **api_functions, unsigned count)
{
  api_name_list = api_names;
  api_func_list = api_functions;
  api_count = count;

  /* load this repo */
  repo_module_api_loader(repo_api_loader_impl);

  /* get dylibs from dir */
  tinydir_dir dir;
  #ifndef _WIN32
  tinydir_open(&dir, "/Users/PhilCK/Desktop/rep_of_a/output/development/");
  #else
  tinydir_open(&dir, "C:/Users/SimStim/Developer/mono/output/development/");
  #endif

  while (dir.has_next)
  {
    tinydir_file file;
    tinydir_readfile(&dir, &file);
    
    #ifdef _WIN32
    const char *ext = "dll";
    #elif defined __APPLE__
    const char *ext = "dylib";
    #else
    const char *ext = "so";
    #endif

    if(!file.is_dir && lib::file::has_extension(file.name, ext))
    {
      printf("file: %s\n", file.name);
    
      roa_dlib_handle handle = roa_dlib_open(file.path);
      assert(handle);
      
      repo_module_api_loader_fn reg_api = (repo_module_api_loader_fn)roa_dlib_get_address(handle, "repo_module_api_loader");      

      if(reg_api)
      {
        reg_api(repo_api_loader_impl);
        repo_module_create_fn create_fn = (repo_module_create_fn)roa_dlib_get_address(handle, "repo_module_create");
        
        if(create_fn)
        {
          create_fn();
        }
      }
    }
    
    tinydir_next(&dir);
  }

  tinydir_close(&dir);
}

void
repo_plugins_unload()
{
}
