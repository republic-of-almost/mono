# Fiber TODO

## Performance

There is high contention in this system. It could be simplified by using TLS more. If the TLS held pending jobs (and maybe pending fibers or just thread locked pending fibers), it should simplify the design.


```c
struct tls
{
  /* array */ jobs *pending_jobs;
  /* array */ jobs *thread_locked_jobs;

  /* array */ fiber *pending_fibers;
  /* array */ fiber *thread_locked_fibers;
};
```


Load balancing could be done through job steals. Where idle threads could search other tls's


Another possible solution would be to continue to have a single array, but try and have a read/write mechanism to pull jobs out without a mutex. Howver this could lead to high false sharing.

## Singlethreaded

Singlethreaded is meant to a testing version / emscripten friendly version of job system. It is still under development.