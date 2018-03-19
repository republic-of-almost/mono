#include <roa_lib/hash.h>


/* https://stackoverflow.com/questions/2535284/how-can-i-hash-a-string-to-an-int-using-c#13487193 */
/* test this, attribute or remove */
uint64_t
roa_hash(const char *name)
{
  uint64_t hash = 5381;

  int c;
  while (c = *name++, c)
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}
