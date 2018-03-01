#include <roa_lib/alloc.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Alloc")
{
  SECTION("Basic Alloc")
  {
  	/* a better test is possible :) */
  	
  	void *bytes = roa_alloc(128);
  	roa_free(bytes);

  	REQUIRE(true);
  }

  SECTION("allocator")
  {
    roa_tagged_allocator_init();

    roa_tagged_allocator allocator;
    roa_tagged_allocator_create(&allocator, 1);

    void *alloc = roa_tagged_allocator_alloc(&allocator, 123);
    REQUIRE(alloc);

    roa_tagged_allocator_destroy();
  }
}
