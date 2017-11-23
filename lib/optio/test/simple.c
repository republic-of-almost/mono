///*
//  Simple Test
//  --
//  Basic functionality test.
//*/
//
//
//#include <fiber/dispatcher.h>
//#include <atomic_counter.h>
//#include <stdio.h>
//#include <pthread.h>
//#include <string.h>
//#include <assert.h>
//
//
///* ----------------------------------------------------- [ Test Settings ] -- */
//
//
//#define job_iterations 50
//#define job_spawn_count 100
//
//#define TEST_LOGGING
//
//int job_work[job_spawn_count];
//
//
///* --------------------------------------------------------- [ Test Jobs ] -- */
//
//
//void some_jawb(struct optio_dispatcher_ctx *fibers, void *arg)
//{
//  int *int_arg = (int*)arg;
//  (*int_arg)++;
//
//  /* check for double job calls */
//  assert(*int_arg == 1);
//  
//  #ifdef TEST_LOGGING
//  printf("WORKING\n");
//  #endif
//}
//
//
///* -------------------------------------------------------- [ Fiber Test ] -- */
//
//
//int
//main()
//{
//  int i;
//
//  for(i = 0; i < job_iterations; ++i)
//  {
//    memset(job_work, 0, sizeof(job_work));
//
//    struct optio_dispatcher_ctx *fibers = 0;
//    optio_dispatcher_create(&fibers);
//
//    /* run test */
//    {
//
//      struct optio_job_desc job_desc[job_spawn_count];
//      int k;
//
//      for(k = 0; k < job_spawn_count; ++k)
//      {
//        job_desc[k].func = some_jawb;
//        job_desc[k].arg = &job_work[k];
//      }
//
//      int marker = optio_dispatcher_add_jobs(fibers, job_desc, job_spawn_count);
//      optio_dispatcher_run(fibers);
//    }
//  
//    #ifdef TEST_LOGGING
//    int j;
//    for(j = 0; j < job_spawn_count; ++j)
//    {
//      printf("%d,", job_work[j]);
//    }
//    printf("\n");
//    #endif
//
//    optio_dispatcher_destroy(&fibers);
//    
//    #ifdef TEST_LOGGING
//    printf("-----\n");
//    #endif
//  }
//
//  return 0;
//}
