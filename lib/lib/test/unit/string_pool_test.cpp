#include <catch/catch.hpp>
#include <lib/string_pool.hpp>
#include <stdint.h>
#include <string.h>


TEST_CASE("String Pool")
{
  lib::string_pool::reset();
  
  SECTION("None")
  {
    REQUIRE(lib::string_pool::size() == 0);
  }

  SECTION("Add string")
  {
    const uint32_t str_id = lib::string_pool::add("foo");
    
    REQUIRE(str_id > 0);
    REQUIRE(lib::string_pool::size() == 1);
  }
  
  SECTION("Find string")
  {
    const uint32_t add_id = lib::string_pool::add("bar");
    const uint32_t find_id = lib::string_pool::find("bar");
    
    REQUIRE(add_id == find_id);
    REQUIRE(lib::string_pool::size() == 1);
  }
  
  SECTION("Get string")
  {
    const uint32_t add_id = lib::string_pool::add("baz");
    const char *get = lib::string_pool::get(add_id);
    
    REQUIRE(strcmp(get, "baz") == 0);
  }
}
