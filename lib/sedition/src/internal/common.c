#include <internal/common.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* ------------------------------------------------------------ [ Memory ] -- */


void*
sed_malloc(size_t bytes)
{
  return malloc(bytes);
}


void
sed_free(void *ptr)
{
  if(ptr)
  {
    free(ptr);
  }
}


void*
sed_realloc(void *ptr, size_t new_size)
{
  return realloc(ptr, new_size);
}


/* ------------------------------------------------------------ [ String ] -- */


char *
sed_string(const char *name)
{
  char *str = (char *)sed_malloc(strlen(name) + 1);
  memcpy(str, name, strlen(name) + 1);

  return str;
}


char *
sed_string_append(char *str, const char *append)
{
  const size_t old_size = strlen(str);
  char *new_str = sed_realloc((void*)str, strlen(str) + strlen(append) + 1);
  memcpy(new_str + old_size, append, strlen(append) + 1);

  return new_str;
}


void
sed_string_free(char *str)
{
  free(str);
}


const char *
sed_string_filename_from_path(const char *path)
{
  const char *ssc;
  int l = 0;

  ssc = strstr(path, "/");
  
  while(ssc)
  {
    l = strlen(ssc) + 1;
    path = &path[strlen(path)-l+2];
    ssc = strstr(path, "/");
  };

  return ssc ? ssc : path;
}


const char *
sed_string_ext_from_filename(const char *filename)
{
  const char *dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";

  return dot + 1;
}


/* -------------------------------------------------------- [ Filesystem ] -- */


void
sed_mkdir(const char *path)
{
  int err = 0;

  #ifdef _WIN32

  #else
  mode_t mode = 0733;
  err = mkdir(path, mode);
  #endif
}


FILE *curr_file = NULL;


void
sed_file_create(const char *name)
{
  if(curr_file)
  {
    fclose(curr_file);
    curr_file = NULL;
  }

  curr_file = fopen(name, "w+");
}


void
sed_file_append(const char *str)
{
  if(curr_file)
  {
    fputs(str, curr_file);
  }
}


void
sed_file_close()
{
  fclose(curr_file);
  curr_file = NULL;
}

