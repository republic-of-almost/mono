#include <roa_lib/file.h>
#include <roa_lib/assert.h>
#include <stdio.h>
#include <stdlib.h>


int
roa_copy_file(const char *filename, char **out_file, unsigned *out_len)
{
  /* param check */
  ROA_ASSERT(filename);
  ROA_ASSERT(strlen(filename) > 0);
  ROA_ASSERT(out_file);


  const char *fs = 0;
  FILE *fs_file = fopen(filename, "rb");
  unsigned length = 0;

  if (fs_file)
  {
    fseek(fs_file, 0, SEEK_END);
    length = ftell(fs_file);
    fseek(fs_file, 0, SEEK_SET);
    fs = calloc(1, length + 1);

    if (out_file)
    {
      *out_file = fs;
    }

    if (out_len)
    {
      *out_len = length;
    }

    if (fs_file)
    {
      fread(fs, 1, length, fs_file);
    }
    fclose(fs_file);
  }
}