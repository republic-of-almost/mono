#include <roa_lib/assert.h>


#ifdef _WIN32
#include <assert.h>
#endif


void
roa_internal_assert(
	const char *expr,
	const char *file,
	unsigned line,
	const char *func)
{
  #ifdef _WIN32
  _wassert(expr, file, line);
  #else
	__assert_fail (expr, file, line, func);
  #endif
}	