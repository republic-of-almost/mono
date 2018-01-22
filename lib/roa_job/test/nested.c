/*
  Nested Test
  --
  Tests wait for etc.
*/


#include <fiber/roa_dispatcher.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>


/* ----------------------------------------------------- [ Test Settings ] -- */


#define job_iterations 1
#define job_ticks 400
#define job_spawn_count_inital 50
#define job_spawn_count_nested 100


#define TEST_LOGGING


int job_work_inital[job_spawn_count_inital];
int job_work_nested[job_spawn_count_nested];


/* --------------------------------------------------------- [ Test Jobs ] -- */


void some_other_jawb(struct roa_dispatcher_ctx *fibers, void *arg)
{
  int *int_arg = (int*)arg;
  ++(*int_arg);
  
  #ifdef TEST_LOGGING
//  printf("FINISH FIRST %d\n", *int_arg);
  #endif
}


void some_jawb(struct roa_dispatcher_ctx *fibers, void *arg)
{
  int *int_arg = (int*)arg;
  (*int_arg)++;

  int l;
  for(l = 0; l < job_ticks; ++l)
  {
    struct roa_job_desc job_desc[job_spawn_count_nested];
    int k;
    
    for(k = 0; k < job_spawn_count_nested; ++k)
    {
      job_desc[k].func = some_other_jawb;
      job_desc[k].arg = &job_work_nested[k];
    }
    
    int job_marker = roa_dispatcher_add_jobs(fibers, job_desc, job_spawn_count_nested);
    
    roa_dispatcher_wait_for_counter(fibers, job_marker);
    
    #ifdef TEST_LOGGING
    printf("**TICK** %d / %d\n", l, job_ticks);
    #endif
  }

  /* check for double job calls */
  assert(*int_arg == 1);
  
  #ifdef TEST_LOGGING
//  printf("--== FINISH LAST ==--\n");
  #endif
}


/* -------------------------------------------------------- [ Fiber Test ] -- */


int
main()
{
  int i;

  for(i = 0; i < job_iterations; ++i)
  {
    memset(job_work_inital, 0, sizeof(job_work_inital));
    memset(job_work_nested, 0, sizeof(job_work_nested));

    struct roa_dispatcher_ctx *fibers = 0;
    roa_dispatcher_create(&fibers);

    /* run test */
    {
      struct roa_job_desc job_desc[job_spawn_count_inital];
      int k;

      for(k = 0; k < job_spawn_count_inital; ++k)
      {
        job_desc[k].func = some_jawb;
        job_desc[k].arg = &job_work_inital[k];
      }

      roa_dispatcher_add_jobs(fibers, job_desc, job_spawn_count_inital);
      roa_dispatcher_run(fibers);
    }
  
    #ifdef TEST_LOGGING
    int j;
    for(j = 0; j < job_spawn_count_nested; ++j)
    {
      printf("%d,", job_work_nested[j]);
    }
    printf("\n");
    #endif

    roa_dispatcher_destroy(&fibers);
    
    #ifdef TEST_LOGGING
    printf("-----\n");
    #endif
  }

  return 0;
}
