#include <nil/resource/directory.hpp>
#include <lib/directory.hpp>
#include <lib/platform.hpp>
#include <lib/string.hpp>
#include <string.h>


namespace Nil {
namespace Resource {


const char *
directory(const char *file)
{
  static char path[LIB_MAX_FILE_PATH_SIZE];
  memset(path, 0, sizeof(path));

  strcat(path, lib::dir::exe_path());//, length - strlen(path));
  strcat(path, asset_path());

  strcat(path, file);//, length - strlen(path));

  return path;
}


const char *
asset_path()
{
  static char path[LIB_MAX_FILE_PATH_SIZE];
  memset(path, 0, sizeof(path));

  #ifdef LIB_PLATFORM_MAC
  
  const size_t length = LIB_MAX_FILE_PATH_SIZE;
  strlcat(path, "../Resources/assets/", length - strlen(path));

  #else

  strcat(path, "assets/");

  #endif

  return path;
}


} // n
} // ns
