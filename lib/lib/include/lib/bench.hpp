/*
  Bench
  --
  Utilitiy for benchmarking.
  
  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef BENCHMARK_INCLUDED_022A1DD1_7E69_4862_ADD5_BCC7E9C4090A
#define BENCHMARK_INCLUDED_022A1DD1_7E69_4862_ADD5_BCC7E9C4090A


// --------------------------------------------------------------- [ Config ] --


#ifndef NBENCHMARK
#define LIB_BENCH_USE_REMOTERY
#else
#define LIB_BENCH_NONE
#endif


// ------------------------------------------------------------ [ Interface ] --


#ifdef LIB_BENCH_USE_REMOTERY

#include <remotery/Remotery.h>

#define BENCH_INIT_CPU   static Remotery* rmt; rmt_CreateGlobalInstance(&rmt);
#define BENCH_INIT_GPU   rmt_BindOpenGL();

#define BENCH_SCOPED_CPU(name) rmt_ScopedCPUSample(name, 0);
#define BENCH_PUSH_CPU(name) rmt_BeginCPUSample(name, 0);
#define BENCH_POP_CPU rmt_EndCPUSample();

#define BENCH_SCOPED_GPU(name) rmt_ScopedOpenGLSample(name);
#define BENCH_PUSH_GPU(name) rmt_BeginOpenGLSample(name);
#define BENCH_POP_GPU rmt_EndOpenGLSample();

#else

#define BENCH_INIT_CPU
#define BENCH_INIT_GPU

#define BENCH_SCOPED_CPU(name)
#define BENCH_PUSH_CPU(name)
#define BENCH_POP_CPU

#define BENCH_SCOPED_GPU(name)
#define BENCH_PUSH_GPU(name)
#define BENCH_POP_GPU

#endif



#endif // inc guard
