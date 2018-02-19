#include <roa_lib/dir.h>
#include <roa_lib/fundamental.h>

#ifdef _WIN32
#include <windows.h>
#endif

const char*
roa_exe_path()
{
  #ifdef _WIN32
  static char buffer_path[2048] = "\0";

  if (strcmp(buffer_path, "") == 0)
  {
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL)
    {
      GetModuleFileNameA(NULL, buffer_path, ROA_ARR_COUNT(buffer_path));

      size_t path_length = 0;
      for (size_t i = 0; i < ROA_ARR_COUNT(buffer_path); i++)
      {
        if (buffer_path[i] == '\0') {
          path_length = i;
          break;
        }
      }

      size_t last_slash_index = 0;
      for (size_t i = 0; i < path_length; i++)
      {
        size_t r_i = (path_length - 1) - i;
        if (buffer_path[r_i] == '/' || buffer_path[r_i] == '\\') {
          last_slash_index = r_i;
          break;
        }
      }

      buffer_path[last_slash_index + 1] = '\0';
    }
  }
  return buffer_path;
  #endif
}