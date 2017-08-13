#include <catch/catch.hpp>
#include <lib/string.hpp>


TEST_CASE("string")
{
  SECTION("Compare")
  {
    REQUIRE(lib::str::compare("foo", "foo") == true);
    REQUIRE(lib::str::compare("foo", "FOO") == false);
    REQUIRE(lib::str::compare("foo", "bar") == false);
  }

  SECTION("Append")
  {
    char buff[16]{};
    
    lib::str::append(buff, "foo", sizeof(buff));
    REQUIRE(lib::str::compare("foo", buff) == true);
    
    lib::str::append(buff, "bar", sizeof(buff));
    REQUIRE(lib::str::compare("foobar", buff) == true);
  }
  
  SECTION("Length")
  {
    REQUIRE(lib::str::length("foo") == 3);
    
    char buff[1024]{};
    lib::str::append(buff, "foo", sizeof(buff));
    REQUIRE(lib::str::length(buff) == 3);
  }
  
  SECTION("Copy")
  {
    const char src[] = "foobarbaz";
  
    char big_buff[1024]{};
    lib::str::copy(big_buff, src, sizeof(big_buff));
    REQUIRE(lib::str::compare(src, big_buff) == true);
    
    char small_buff[7]{};
    lib::str::copy(small_buff, src, sizeof(small_buff));
    REQUIRE(lib::str::compare(small_buff, "foobar"));
  }
}
