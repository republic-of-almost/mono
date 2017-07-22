/*
  Memory
  --
  Memory pool and tracking.
  
  This memory pool isn't really meant to be super fast, its designed to be general
  so that we can track allocation by tag, and use it to create pools ontop of it.
  
  It serves as a good place to track alloc callstacks and memory corruptions.
  
  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef MEMORY_INCLUDED_F78EEBC6_56DC_4B4A_8BEC_2E616E0F02AD
#define MEMORY_INCLUDED_F78EEBC6_56DC_4B4A_8BEC_2E616E0F02AD


#include "logging.hpp"
#include "assert.hpp"
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


// -------------------------------------------------------- [ Mem Interface ] --


namespace LIB_NS_NAME {
namespace mem {


namespace tag {
enum ENUM {
  FREE,
  GENERAL,
  NUMBER,
  STRING,
  TEXTURE,
  AUDIO,
  MESH,
  FILE,
};
}


struct header
{
  uint32_t mem_tag;
  uint32_t number_of_buckets;
  void *data;
};


size_t
bucket_stride();

size_t
bucket_capacity();

size_t
buckets_in_use();


header*
get_headers();


template<typename T, uint32_t tag = tag::GENERAL, size_t count = 1>
T*
alloc()
{
  constexpr size_t bucket_count = LIB_POOL_BYTE_SIZE / LIB_POOL_BUCKET_SIZE;
  const size_t buckets_needed = ((sizeof(T) * count) / LIB_POOL_BUCKET_SIZE) + 1;

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
    for(size_t j = start_bucket; j < (start_bucket + buckets_needed); ++j)
    {
      headers[j].mem_tag = tag;
      headers[j].number_of_buckets = buckets_needed;
    }
    
    return static_cast<T*>(possible->data);
  }
  
  return nullptr;
}


template<typename T>
void
free(T *free)
{
  header* headers = get_headers();

  const size_t count = bucket_capacity();

  for(size_t i = 0; i < count; ++i)
  {
    if(headers[i].data == free)
    {
      size_t bucket_count = headers[i].number_of_buckets;
    
      for(size_t j = 0; j < bucket_count; ++j)
      {
        headers[i + j].mem_tag = tag::FREE;
        headers[i + j].number_of_buckets = 0;
      }
      
      return;
    }
  }
  
  LIB_ASSERT(false);
  LOG_ERROR("This pointer wasn't in the pool");
}


} // ns
} // ns


#endif // inc guard


// ------------------------------------------------------------- [ Mem Impl ] --


#ifdef LIB_MEM_IMPL
#ifndef LIB_MEM_IMPL_INCLUDED
#define LIB_MEM_IMPL_INCLUDED


namespace LIB_NS_NAME {
namespace mem {


namespace {


header*
init_memory()
{
  static int once = 1;
  static LIB_NS_NAME::mem::header *headers;
  
  if(once)
  {
    constexpr size_t header_count = LIB_POOL_BYTE_SIZE / LIB_POOL_BUCKET_SIZE;
    constexpr size_t header_bytes = header_count * sizeof(header);
    
    headers = (LIB_NS_NAME::mem::header*)malloc(header_bytes);
    memset(headers, 0, sizeof(header_bytes));
    
    uint8_t *mem = (uint8_t*)malloc(LIB_POOL_BYTE_SIZE);
    
    for(size_t i = 0; i < header_count; ++i)
    {
      headers[i].data = (void*)&mem[i * LIB_POOL_BUCKET_SIZE];
    }
    
    once = 0;
  }
  
  return headers;
}


};


size_t
bucket_stride()
{
  return LIB_POOL_BUCKET_SIZE;
}


size_t
bucket_capacity()
{
  return LIB_POOL_BYTE_SIZE / LIB_POOL_BUCKET_SIZE;
}


size_t
buckets_in_use()
{
  header *head = get_headers();
  
  const size_t count = bucket_capacity();
  
  size_t buckets_in_use = 0;
  
  for(size_t i = 0; i < count; ++i)
  {
    if(head[i].mem_tag != tag::FREE)
    {
      ++buckets_in_use;
    }
  }
  
  return buckets_in_use;
}


header*
get_headers()
{
  static header *mem_header = init_memory();
  
  return mem_header;
};


} // ns
} // ns


#endif // impl guard
#endif // impl guard
