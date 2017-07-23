#include <nil/resource/directory.hpp>
#include <lib/directory.hpp>
#include <lib/platform.hpp>
#include <string.h>


namespace Nil {
namespace Resource {


const char *
directory(const char *file)
{
  static char path[LIB_MAX_FILE_PATH_SIZE];
  memset(path, 0, sizeof(path));
  
  const size_t length = LIB_MAX_FILE_PATH_SIZE;
  
  strlcat(path, lib::dir::exe_path(), length - strlen(path));
  
  #ifdef LIB_PLATFORM_MAC
  
  strlcat(path, "../Resources/assets/", length - strlen(path));
  
  #endif
  
  strlcat(path, file, length - strlen(path));
  
  return path;
}


} // n
} // ns
