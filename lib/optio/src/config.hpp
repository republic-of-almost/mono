#ifndef CONFIG_INCLUDED_0476F257_2625_481F_B93F_38ADA3FA8C5E
#define CONFIG_INCLUDED_0476F257_2625_481F_B93F_38ADA3FA8C5E


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


/* ----------------------------------------------------------- [ Helpers ] -- */


#define FIBER_ASSERT(expr) assert(expr)
#define FIBER_MALLOC(bytes) malloc(bytes)
#define FIBER_FREE(ptr) free(ptr)
#define FIBER_MEMZERO(ptr, bytes) memset(ptr, 0, bytes)


/* ----------------------------------------------------------- [ Logging ] -- */


#ifndef NFIBER_LOGGING
#define FIBER_LOG(str) printf("%s\n", str)
#define FIBER_LOG_NUMBER(str, num) printf("%s : %d\n", str, num)
#define FIBER_TH_LOG(msg, th_data) printf("%s - FiberID %d Shutdown %d HomeFiber %p\n", msg, th_data->fiber_id, th_data->shutdown, (void*)th_data->home_fiber);
#define FIBER_FI_LOG(msg, fi_data) printf("%s - Fiber %p Func %p Arg %p\n", msg, (void*)fi_data->fiber, (void*)fi_data->desc.func, (void*)fi_data->desc.arg);
#else
#define FIBER_LOG(str)
#define FIBER_LOG_NUMBER(str, num)
#define FIBER_TH_LOG(msg, th_data)
#define FIBER_FI_LOG(msg, fi_data)
#endif


#endif /* inc guard */
