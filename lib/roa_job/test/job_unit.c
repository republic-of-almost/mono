//#include <job_queue.h>
//#include <assert.h>
//#include <stdio.h>
//
//
///* --------------------------------------------------------- [ Test Data ] -- */
//
//
//void
//test_func(void *arg) {};
//
//int some_arg = 0;
//
//
///* -------------------------------------------------------------- [ Test ] -- */
//
//
//int
//main()
//{
//  printf("ROA JOB UNIT TEST\n");
//  
//  struct roa_job_queue_ctx ctx;
//  
//  /* create */
//  {
//    roa_job_create(&ctx);
//    assert(roa_job_batch_size(&ctx) == 0);
//    assert(roa_job_desc_size(&ctx) == 0);
//  }
//
//
//  /* add job batches */
//  {
//    for(int j = 0; j < 5; ++j)
//    {
//      struct roa_job_desc desc[10];
//      
//      for(int i = 0; i < 10; ++i)
//      {
//        desc[i].arg = &some_arg;
//        desc[i].job_func = test_func;
//      }
//      
//      int batch = roa_job_add_batch(&ctx, desc, 10);
//      assert(batch);
//      
//      assert(roa_job_batch_size(&ctx) == (j + 1));
//      assert(roa_job_desc_size(&ctx) == 10 * (j + 1));
//    }
//  }
//  
//  /* get next job */
//  {
//    for(int j = 0; j < 5; ++j)
//    {
//      for(int i = 0; i < 10; ++i)
//      {
//        void *func = 0;
//        void *arg = 0;
//        int next = roa_job_next(&ctx, &func, &arg);
//        
//        assert(next);
//        assert(func);
//        assert(arg);
//        
//        int cleared_batch = roa_job_clear(&ctx, next);
//        
//        assert(i < 9 ? cleared_batch == 0 : cleared_batch >= 1);
//      }
//      
//      assert(roa_job_batch_size(&ctx) == 5 - (j + 1));
//    }
//    
//    assert(roa_job_desc_size(&ctx) == 0);
//    assert(roa_job_batch_size(&ctx) == 0);
//  }
//
//  /* destroy */
//  roa_job_create(&ctx);
//
//  return 0;
//}
