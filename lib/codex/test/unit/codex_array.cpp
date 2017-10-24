#include <catch/catch.hpp>
#include <common/array.h>


TEST_CASE("Codex Array")
{
  SECTION("Int Array")
  {
    int *test_arr = NULL;
    codex_array_create(test_arr, 123);
    
    for(uint32_t i = 0; i < 1000; ++i)
    {
      codex_array_push(test_arr, i);
    }
    
    for(uint32_t i = 0; i < 1000; ++i)
    {
      REQUIRE(test_arr[i] == i);
    }
    
    codex_array_destroy(test_arr);
  }

  SECTION("Struct Array")
  {
    struct Foo {
      int a;
      char *b;
      float *c;
    };
    
    struct Foo *test_arr = NULL;
    codex_array_create(test_arr, 123);
    
    for(uint32_t i = 0; i < 1000; ++i)
    {
      struct Foo foo;
      foo.a = i;
      codex_array_push(test_arr, foo);
    }
    
    for(uint32_t i = 0; i < 1000; ++i)
    {
      REQUIRE(test_arr[i].a == i);
    }
    
    codex_array_destroy(test_arr);
  }
}
