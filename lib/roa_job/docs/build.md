# ROA Job Build

Build information for the ROA Job dispatcher.


## Build Language

C89


## Build Flags

Some combinations or crazy values might yield unexpected results. Use your head here.


### ROA_JOB_DEBUG_NAME_THREADS

_set to 1 or 0: default 0_

If possible it will rename the thread to help with debugging.

### ROA_JOB_CACHELINE_PADDING

_set to 1 or 0: default 0_

Very minor performance increase, but not throughly tested on many CPUs or Functional tests.

### ROA_JOB_CPU_AFFINITY

_set to 1 or 0: default 1_

If possible bind a thread to a core to help with cache locality. This has only been tested with a simple test case, but showed some minor improvements.

### ROA_JOB_DEBUG_TH_PROCESS_OUTPUT

_set to 1 or 0: default 0_

Pushes out logging information to the console to help with debugging.

### ROA_JOB_STEAL_SIZE

_set to 1 or greater: default 1_

The number of jobs an idle thread will attempt to steal.

This has only been tested with lots of small little jobs. For that _1_ is recommended or it holds up the executing thread up.
