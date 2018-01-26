#include <roa_lib/assert.h>


void
roa_internal_assert(
	const char *expr,
	const char *file,
	unsigned line,
	const char *func)
{
	__assert_fail (expr, file, line, func);
}	