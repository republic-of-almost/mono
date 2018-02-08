# ROA Job Build

Build information for the ROA Job dispatcher.


## Build Language

C++11(Procedural), should be convertable to C89 with ease, if you have an atomics solution.


## Build Flags

Some combinations or crazy values might yield unexpected results. Use your head here.


### ROA_JOB_SINGLETHREADED

_set to 1 or 0 : default 0_

Executes jobs in a singlethreaded environment. Everytime jobs are submitted it will be executed there and then.

WIP - Intended for testing and cases where single threaded may be prefered.


### ROA_DISPATCH_MAX_FIBERS

_default 128_

Set the number of fibers to be constructed, this is a fixed size.


### ROA_DISPATCH_MAX_JOBS

_default 1024_

Set the number of jobs that can be submitted, this is a fixed size.


### ROA_JOB_DEBUG_NAME_THREADS

_set to 1 or 0 : default 1_

If the platform supports it, the fiber system will rename threads with debug information.
Its not recomended to be too granular with this, there can be a performance hit.