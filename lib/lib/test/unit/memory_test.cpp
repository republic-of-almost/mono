#include <catch/catch.hpp>
#include <lib/utilities.hpp>
#include <cstring>


TEST_CASE("Memory Pool")
{
  SECTION("Init")
  {
    REQUIRE(lib::mem::buckets_in_use() == 0);
  }

  SECTION("Simple Alloc Free")
  {
    REQUIRE(sizeof(int) < lib::mem::bucket_stride()); // Other wise the test is invalid.
  
    int *foo = nullptr;
    foo = lib::mem::alloc<int>();
    
    REQUIRE(foo != nullptr);
    REQUIRE(lib::mem::buckets_in_use() == 1);
    
    lib::mem::free(foo);
    REQUIRE(lib::mem::buckets_in_use() == 0);
  }
  
  SECTION("Allocate few")
  {
    int *foo = nullptr;
    foo = lib::mem::alloc<int>();
    
    int *bar = nullptr;
    bar = lib::mem::alloc<int>();
    
    REQUIRE(foo != bar);
    
    REQUIRE(lib::mem::buckets_in_use() == 2);
    
    lib::mem::free(foo);
    lib::mem::free(bar);
    
    REQUIRE(lib::mem::buckets_in_use() == 0);
  }
  
  SECTION("Allocate Array")
  {
    int *foo = nullptr;
    foo = lib::mem::alloc<int, lib::mem::tag::GENERAL, 256>();
    
    size_t buckets_in_use = (sizeof(int) * 256) / lib::mem::bucket_stride() + 1;
    REQUIRE(buckets_in_use == lib::mem::buckets_in_use());
    
    lib::mem::free(foo);
    
    REQUIRE(lib::mem::buckets_in_use() == 0);
  }  
}
