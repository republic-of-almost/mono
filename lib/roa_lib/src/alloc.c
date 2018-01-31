#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <stdlib.h>


/* ------------------------------------------------------ [ Alloc Config ] -- */


#ifndef ROALIB_ALLOC_STATS
#define ROALIB_ALLOC_STATS 1
#endif


#if ROALIB_ALLOC_STATS
/*#include <roa_lib/atomic.h>*/

unsigned g_alloc_counter = 0;
unsigned g_free_counter = 0;
#endif


/* ------------------------------------------------------- [ Basic Alloc ] -- */


void*
roa_alloc(unsigned bytes)
{
	if(ROA_IS_ENABLED(ROALIB_ALLOC_STATS))
	{
		g_alloc_counter += 1;
	}
	
	return malloc(bytes);	
}


void*
roa_zalloc(unsigned bytes)
{
  if(ROA_IS_ENABLED(ROALIB_ALLOC_STATS))
  {
    g_alloc_counter += 1;
  }

  return calloc(1, bytes);
}


void
roa_free(void *addr)
{
	if(ROA_IS_ENABLED(ROALIB_ALLOC_STATS))
	{
		g_free_counter += 1;
	}
	
	free(addr);
}


/* ------------------------------------------------------ [ Alloc Config ] -- */


void
roa_alloc_stats_get(struct roa_alloc_stats *stats)
{
	stats->basic_alloc 	= g_alloc_counter;
	stats->basic_free 	= g_free_counter;
}


#undef ROALIB_ALLOC_STATS