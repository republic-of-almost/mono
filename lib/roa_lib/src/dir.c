#include <roa_lib/dir.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/log.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#endif

const char*
roa_exe_dir()
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
  #elif defined(__linux__)
	static char buffer_path[2048] = "\0";

	if(strlen(buffer_path) == 0)
	{
		char buffer[2048];
		ROA_MEM_ZERO(buffer);

		unsigned int count = readlink("/proc/self/exe", buffer, sizeof(buffer));

		if(count != (unsigned)-1)
		{
			const char *path = dirname(buffer);
			strcpy(buffer_path, path);
			strcat(buffer_path, "/");
		}
		else
		{
			ROA_LOG_ERROR("Failed to get directory");
		}
	}

	return ROA_ARR_DATA(buffer_path);
	#endif
}
