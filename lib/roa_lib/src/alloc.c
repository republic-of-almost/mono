#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
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


/* --------------------------------------------------------- [ Allocator ] -- */


struct roa_internal_frame_allocator
{
  /* array */ uint64_t *block_tag_frame_key;
  /* array */ uint64_t *block_tag_frame_count;

  /* array */ uint64_t *block_tag_keys;
  /* array */ void **blocks;
} internal_allocator;


void
roa_tagged_allocator_init()
{
  ROA_MEM_ZERO(internal_allocator);

  roa_array_create(internal_allocator.block_tag_frame_key, 32);
  roa_array_resize(internal_allocator.block_tag_frame_key, 32);

  roa_array_create(internal_allocator.block_tag_frame_count, 32);
  roa_array_resize(internal_allocator.block_tag_frame_count, 32);

  roa_array_create(internal_allocator.block_tag_keys, 32);
  roa_array_resize(internal_allocator.block_tag_keys, 32);

  roa_array_create(internal_allocator.blocks, 32);
  roa_array_resize(internal_allocator.blocks, 32);

  /* create blocks */
  unsigned count = roa_array_size(internal_allocator.blocks);
  int i;

  for (i = 0; i < count; ++i)
  {
    internal_allocator.block_tag_keys[i] = 0;
    internal_allocator.blocks[i] = roa_alloc(ROA_MB_TO_BYTES(2));
  }
}


void
roa_tagged_allocator_destroy()
{
  roa_array_destroy(internal_allocator.block_tag_frame_key);
  roa_array_destroy(internal_allocator.block_tag_frame_count);

  roa_array_destroy(internal_allocator.block_tag_keys);
  roa_array_destroy(internal_allocator.blocks);
}


void
roa_tagged_allocator_create(
  struct roa_tagged_allocator *allocator,
  uint64_t tag)
{
  ROA_ASSERT(tag);

  allocator->allocator_tag = tag;
}


void*
roa_tagged_allocator_alloc(
  struct roa_tagged_allocator *allocator,
  unsigned bytes)
{
  unsigned remaining = allocator->block_end - allocator->block_begin;

  if (remaining < bytes)
  {
    uint64_t *tags = internal_allocator.block_tag_keys;
    unsigned count = roa_array_size(internal_allocator.block_tag_keys);
    unsigned i;

    for (i = 0; i < count; ++i)
    {
      if (tags[i] == 0)
      {
        void **blocks = internal_allocator.blocks;

        tags[i] = allocator->allocator_tag;
        allocator->block_begin = internal_allocator.blocks[i];
        allocator->block_end = allocator->block_begin + ROA_MB_TO_BYTES(2);

        break;
      }
    }
  }

  void *alloc = allocator->block_begin;
  allocator->block_begin = &allocator->block_begin[bytes];

  return alloc;
}


void
roa_tagged_allocator_free(uint64_t tag)
{
  
}



/* ------------------------------------------------------ [ Alloc Config ] -- */


#undef ROALIB_ALLOC_STATS