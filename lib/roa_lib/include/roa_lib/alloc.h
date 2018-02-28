#ifndef ALLOC_INCLUDED_FA713983_DB0B_4D7D_8CC5_00923C16BA74
#define ALLOC_INCLUDED_FA713983_DB0B_4D7D_8CC5_00923C16BA74


#include <roa_lib/fundamental.h>


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


/* --------------------------------------------------------- [ Allocator ] -- */


struct roa_allocator
{
  uint64_t allocator_tag;

  unsigned block_frame_counter;
  uint8_t *block_begin;
  uint8_t *block_end;
};


void
roa_allocator_init();


void
roa_allocator_destroy();


void
roa_allocator_create(
  struct roa_allocator *allocator,
  uint64_t tag);


void*
roa_allocator_alloc(
  struct roa_allocator *allocator,
  unsigned bytes);


void
roa_allocator_free(uint64_t tag);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */