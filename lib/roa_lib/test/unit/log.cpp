#include <roa_lib/log.h>
#include <catch/catch.hpp>


TEST_CASE("log")
{
	roa_logging_set_output(0); /* stops logging flooding unit test output */

  /* just a link time test to see if I can call these things */
  ROA_LOG_INFO("some info %s", "info");
  ROA_LOG_WARNING("warning %d", 123);
  ROA_LOG_ERROR("error %f", 1.234f);
  ROA_LOG_FATAL("fatal");
}
