#include <roa_lib/assert.h>
#include <roa_lib/fundamental.h>

#include <assert.h>
#ifdef _WIN32
#include <assert.h>
#include <windows.h>
#endif

#include <stdlib.h>


void
roa_internal_assert(
	const char *expr,
	const char *file,
	unsigned line,
	const char *func)
{
  #ifdef _WIN32
  ROA_UNUSED(func);

  wchar_t wmsg[2024];
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, expr, -1, wmsg, ROA_ARR_COUNT(wmsg));

  wchar_t wfile[2024];
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, file, -1, wfile, ROA_ARR_COUNT(wfile));

  _wassert(wmsg, wfile, line);
  #else
	ROA_UNUSED(expr);
	ROA_UNUSED(file);
	ROA_UNUSED(line);
	ROA_UNUSED(func);
	/*__assert_fail (expr, file, line, func);*/
  abort();
  #endif
}	
