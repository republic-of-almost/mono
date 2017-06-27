/*
  Memory
  --
  Memory pool and tracking.
  
  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef MEMORY_INCLUDED_F78EEBC6_56DC_4B4A_8BEC_2E616E0F02AD
#define MEMORY_INCLUDED_F78EEBC6_56DC_4B4A_8BEC_2E616E0F02AD


#include <stdint.h>
#include <stddef.h>


// ----------------------------------------------------------- [ Mem Config ] --


#ifndef LIB_NS_NAME
#define LIB_NS_NAME lib
#endif


#ifndef LIB_POOL_BYTE_SIZE
#define LIB_POOL_BYTE_SIZE 536870912
#endif


#ifndef LIB_POOL_BUCKET_SIZE
#define LIB_POOL_BUCKET_SIZE 32
#endif


#ifndef LIB_POOL_ALIGN
#define LIB_POOL_ALIGN 16
#endif


// -------------------------------------------------------- [ Mem Interface ] --


namespace LIB_NS_NAME {
namespace mem {


namespace tag {
enum ENUM {
  FREE,
  UNKNOWN,
  NUMBER,
  STRING,
  TEXTURE,
  AUDIO,
  MESH,
  SUB_POOL,
};
}


struct header
{
  uint32_t mem_tag;
  uint32_t number_of_buckets;
  void *data;
};


header*
get_headers();


template<typename T, uint32_t tag = tag::UNKNOWN, size_t count = 1>
T*
alloc()
{
  constexpr size_t bucket_count = LIB_POOL_BYTE_SIZE / LIB_POOL_BUCKET_SIZE;
  const size_t buckets_needed = (sizeof(T) * count) / LIB_POOL_BUCKET_SIZE;

  header* headers = get_headers();
  
  // Search for number of consecutive buckets
  header *possible = nullptr;
  size_t bucket_counter = 0;
  size_t start_bucket = 0;
  
  for(size_t i = 0; i < bucket_count; ++i)
  {
    if(!possible)
    {
      if(headers[i].mem_tag != tag::FREE)
      {
        continue;
      }
      else
      {
        possible = &headers[i];
        start_bucket = i;
        continue;
      }
    }
    
    if(possible && headers[i].mem_tag == tag::FREE)
    {
      ++bucket_counter;
    }
    else
    {
      bucket_counter = 0;
      start_bucket = 0;
      possible = nullptr;
      continue;
    }
    
    if(bucket_counter >= buckets_needed)
    {
      break;
    }
  }
  
  if(possible)
  {
    for(size_t i = start_bucket; start_bucket < start_bucket + buckets_needed; ++i)
    {
      headers[i].mem_tag = tag;
      headers[i].number_of_buckets = buckets_needed;
    }
  }
  
  return static_cast<T*>(possible->data);
}


template<typename T>
void
free(T*)
{
  // Find header, mark as free or merge.
}


} // ns
} // ns


#endif // inc guard


// ------------------------------------------------------------- [ Mem Impl ] --


#ifdef LIB_MEM_IMPL
#ifndef LIB_FILE_IMPL_INCLUDED
#define LIB_FILE_IMPL_INCLUDED


namespace LIB_NS_NAME {
namespace mem {


namespace {


header*
init_memory()
{
  static int once = 1;
  static header *headers;
  
  if(once)
  {
    constexpr size_t header_count = LIB_POOL_BYTE_SIZE / LIB_POOL_BUCKET_SIZE;
    constexpr size_t header_bytes = header_count * sizeof(header);
    
    header = (header*)malloc(header_bytes);
    memset(header, 0, sizeof(header_bytes));
    
    uint8_t *mem = (uint8_t*)malloc(LIB_POOL_BYTE_SIZE);
    
    for(size_t i = 0; i < header_count; ++i)
    {
      header[i].data = mem[i * LIB_POOL_BUCK_SIZE];
    }
    
    once = 0;
  }
  
  return &smem_header;
}


};


header*
get_headers()
{
  static header *mem_header = init_memory();
};


} // ns
} // ns


#endif // impl guard
#endif // impl guard
