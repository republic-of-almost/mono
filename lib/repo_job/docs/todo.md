# Fiber TODO

- Job Queue in TLS Job Steal
  Lots of contention and cache line sharing, we should have a job queue per
  thread. and have idle threads steal work.
  
- Needs alot more profiling, but want to get other platforms working first.

- Block ID should be set at the same time as blocking thread.

- Stack allocate a counter and pass that instead of block id's
