#ifndef ALLOC_INCLUDED_FA713983_DB0B_4D7D_8CC5_00923C16BA74
#define ALLOC_INCLUDED_FA713983_DB0B_4D7D_8CC5_00923C16BA74


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Basic Alloc ] -- */


void*
roa_alloc(unsigned bytes);


void*
roa_zalloc(unsigned bytes);


void
roa_free(void *addr);


/* ------------------------------------------------------- [ Alloc Debug ] -- */
/*
	Must be built with ROALIB_ALLOC_STATS 1 for these to be recorded.
*/


struct roa_alloc_stats
{
	unsigned basic_alloc;
	unsigned basic_free;
};


void
roa_alloc_stats_get(struct roa_alloc_stats *stats);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */