#include <roa_lib/thread.h>
#include <roa_lib/fundamental.h>
#include <catch/catch.hpp>
#include <string.h>


/* ------------------------------------------------------- [ Test Config ] -- */


/* Set to 1 if you want some console output */
#define ROALIB_TEST_OUTPUT_SANITY 0


#ifndef ROALIB_TEST_OUTPUT_SANITY
#include <stdio.h>
#endif


/* ---------------------------------------------------- [ Test Functions ] -- */


void*
test(void *arg)
{
  ROA_UNUSED(arg);

	if(ROA_IS_ENABLED(ROALIB_TEST_OUTPUT_SANITY))
	{
		printf("hi from thread \n");
	}

	return ROA_NULL;
}


void*
exit_th(void *arg)
{
  ROA_UNUSED(arg);

	/* this loop should only happen once */
	while(true)
	{
		if(ROA_IS_ENABLED(ROALIB_TEST_OUTPUT_SANITY))
		{
			printf("exiting from thread \n");
		}

		roa_thread_exit_current();
	}

	return ROA_NULL;
}


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Thread Test")
{
  SECTION("Create / Destroy")
  {
  	roa_thread th = roa_thread_create(test, 0, 0, 0);
  	REQUIRE(th != ROA_NULL);

  	roa_thread_destroy(th);
  }

  SECTION("Create from self")
  {
  	roa_thread th = ROA_NULL;
  	th = roa_thread_create_self();

  	REQUIRE(th != ROA_NULL);
  }

  SECTION("Set thread name")
  {
  	roa_thread_set_current_name("roa_test");

    /* can't test this cross platform */
    /* feel free to break point at check */
    REQUIRE(true);
  }

  SECTION("Current ID")
  {
  	roa_thread_id id1 = roa_thread_get_current_id();
  	REQUIRE(id1 != ROA_NULL);

  	roa_thread_id id2 = roa_thread_get_current_id();
  	REQUIRE(id1 == id2);
  }

  SECTION("Exit Thread")
  {
  	/* hard to test - for now be sure it compiles/links and does not crash */
  	/*
			Manual test set ROALIB_TEST_OUTPUT_SANITY to 1
			and you should only see one line of output from exit_th func
  	*/
  	roa_thread th = roa_thread_create(exit_th, 0, 0, 0);
    ROA_UNUSED(th);

  	REQUIRE(true);
  }

  SECTION("Core Count")
  {
  	unsigned core_count = roa_thread_core_count();

  	/* assumes every cpu is at least dual core */
  	REQUIRE(core_count > 1);

  	if(ROA_IS_ENABLED(ROALIB_TEST_OUTPUT_SANITY))
		{
			printf("Core Count: %d\n", core_count);
		}
	}
}


/* ------------------------------------------------------- [ Test Config ] -- */


#undef ROALIB_TEST_OUTPUT_SANITY