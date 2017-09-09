#include <catch/catch.hpp>
#include <lib/logging.hpp>


TEST_CASE("Logging")
{
  SECTION("Simple")
  {
    LOG_INFO("foo");
    LOG_WARNING("bar");
    LOG_ERROR("boo");
    LOG_FATAL("doo");
  }

  SECTION("Variables")
  {
    LOG_INFO("foo %s", "hi");
    LOG_WARNING("bar %d", 123);
    LOG_ERROR("boo %f", 1.23f);
    LOG_FATAL("doo, %p", nullptr);
  }
}
