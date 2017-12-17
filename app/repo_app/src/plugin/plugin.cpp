#include <plugin/plugin.h>
#include <repo/repo.h>
#include <assert.h>
#include <string.h>
#include <tinydir/tinydir.h>
#include <lib/string.hpp>
#include <lib/file.hpp>
#include <vector>
#include <roalib/dlib.h>


const char **api_name_list = nullptr;
const void **api_func_list = nullptr;
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
repo_plugins_load(const char **api_names, const void **api_functions, unsigned count)
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
  
  std::vector<void*> handles;
  
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
    
      void *handle = roa_dlib_open(file.path);
      assert(handle);
      
      repo_module_api_loader_fn reg_api = (repo_module_api_loader_fn)roa_dlib_get_address(handle, "repo_module_api_loader");

      if(reg_api)
      {
        reg_api(repo_api_loader_impl);
      
        handles.emplace_back(handle);
      }
    }
    
    tinydir_next(&dir);
  }
  
  /* start up plugins */
  {
    for(auto &handle : handles)
    {
      repo_module_create_fn create_fn = (repo_module_create_fn)roa_dlib_get_address(handle, "repo_module_create");
      
      if(create_fn)
      {
        create_fn();
      }
    }
  }

  tinydir_close(&dir);
}

void
repo_plugins_unload()
{
}
