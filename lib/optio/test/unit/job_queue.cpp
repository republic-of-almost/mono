#include <catch/catch.hpp>
#include <optio/dispatcher.h>
#include <job_queue.hpp>
#include <counter.hpp>


/* -------------------------------------------------------- [ Test Funcs ] -- */


#define ARR_COUNT(arr) sizeof(arr) / sizeof(arr[0])


void
test_job(optio_dispatcher_ctx *, void *)
{
}


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Job Queue")
{
  constexpr unsigned max_jobs = 10;

  optio_job_queue_ctx ctx;
  optio_job_queue_create(&ctx, max_jobs);
  
  SECTION("Empty on start")
  {
    const int has_work = optio_job_queue_has_work(&ctx);
    REQUIRE(has_work == 0);
    
    const int batch_size = optio_job_queue_batch_size(&ctx);
    REQUIRE(batch_size == 0);
    
    const int job_size = optio_job_queue_desc_size(&ctx);
    REQUIRE(job_size == 0);
  }
  
  SECTION("Add single Job/Batch")
  {
    unsigned batch_id = 0;
  
    /* add work */
    {
      optio_job_desc desc[] {
        {test_job, nullptr},
        {test_job, nullptr},
      };
      
      batch_id = optio_job_queue_add_batch(
        &ctx,
        desc,
        ARR_COUNT(desc)
      );
      
      REQUIRE(batch_id > 0);
    }
    
    /* has work */
    {
      const int has_work = optio_job_queue_has_work(&ctx);
      REQUIRE(has_work == 1);
      
      const int batch_size = optio_job_queue_batch_size(&ctx);
      REQUIRE(batch_size == 1);
      
      const int job_size = optio_job_queue_desc_size(&ctx);
      REQUIRE(job_size == 2);
    }
    
    /* do work */
    {
      void *func = nullptr;
      void *arg = nullptr;
      
      const unsigned job_id = optio_job_queue_next(&ctx, &func, &arg);
      REQUIRE(job_id);
      
      const unsigned clear_batch_id = optio_job_queue_clear(&ctx, job_id);
      REQUIRE(clear_batch_id == 0);
    }
    
    /* has work */
    {
      const int has_work = optio_job_queue_has_work(&ctx);
      REQUIRE(has_work == 1);
      
      const int batch_size = optio_job_queue_batch_size(&ctx);
      REQUIRE(batch_size == 1);
      
      const int job_size = optio_job_queue_desc_size(&ctx);
      REQUIRE(job_size == 1);
    }
    
    /* do work and hold then release */
    {
      void *func = nullptr;
      void *arg = nullptr;
      
      const unsigned job_id = optio_job_queue_next(&ctx, &func, &arg);
      REQUIRE(job_id);
      
      /* hold */
      optio_counter *counter = optio_job_queue_batch_block(&ctx, batch_id);
      REQUIRE(counter != nullptr);
      
      const unsigned clear_batch_id = optio_job_queue_clear(&ctx, job_id);
      REQUIRE(clear_batch_id == 0);
      
      /* release */
      optio_job_queue_batch_unblock(&ctx, counter);
    }
  }

  SECTION("Max Jobs")
  {
    for(unsigned i = 0; i < max_jobs * 2; ++i)
    {
      /* add work */
      {
        optio_job_desc desc[]{
          { test_job, nullptr },
        };

        const unsigned batch_id = optio_job_queue_add_batch(
          &ctx,
          desc,
          ARR_COUNT(desc)
        );

        void *func = 0;
        void *arg = 0;

        const unsigned job_id = optio_job_queue_next(&ctx, &func, &arg);
        REQUIRE(job_id);

        const unsigned clear_batch_id = optio_job_queue_clear(&ctx, job_id);
        REQUIRE(clear_batch_id != 0);
      }
    }
  }
  
  optio_job_queue_destroy(&ctx);

}
