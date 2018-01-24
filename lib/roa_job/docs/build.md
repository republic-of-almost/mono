# ROA Job Build

Build information for the ROA Job dispatcher.


## Build Language

C++11(Procedural), should be convertable to C89 with ease, if you have an atomics solution.


## Build Flags

Some combinations or crazy values might yield unexpected results. Use your head here.


### ROA_JOB_SINGLETHREADED

_set to 1 or 0 : default 0_

Executes jobs in a singlethreaded environment. Everytime jobs are submitted it will be executed there and then.


### ROA_DISPATCH_MAX_FIBERS

_default 128_

Set the number of fibers to be constructed on initialization.


### ROA_DISPATCH_MAX_JOBS

_default 1024_

Set the number of jobs that can be submitted on initialization.


### ROA_JOB_BIND_THREAD_TO_CORE

_set to 1 or 0 : default 1_

Where the platform API is available it will bind the thread to the core, or pair of cores. Ignored if ROA_JOB_SINGLETHREADED is 1.


### ROA_JOB_DEBUG_NAME_THREADS

_set to 1 or 0 : default 1_

If the platform supports it, the fiber system will rename threads with debug information. There could be a performance hit from this.