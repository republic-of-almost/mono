#include <roa_lib/thread.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>
#include <stdlib.h>
#include <stdio.h>



#ifndef _WIN32
#define SYSCTL_CORE_COUNT   "machdep.cpu.core_count"
//#define SYSCTL_CORE_COUNT "hw.ncpu"
//#define SYSCTL_CORE_COUNT "machdep.cpu.thread_count"


typedef struct cpu_set {
  uint32_t    count;
} cpu_set_t;


static void
CPU_ZERO(cpu_set_t *cs) { cs->count = 0; }

static void
CPU_SET(int num, cpu_set_t *cs) { cs->count |= (1 << num); }

#define CPU_SETSIZE 1024

static int
CPU_ISSET(int num, cpu_set_t *cs) { return (cs->count & (1 << num)); }


int sched_getaffinity(pid_t pid, size_t cpu_size, cpu_set_t *cpu_set)
{
  int32_t core_count = 0;
  size_t  len = sizeof(core_count);
  int ret = sysctlbyname(SYSCTL_CORE_COUNT, &core_count, &len, 0, 0);
  if (ret) {
    printf("error while get core count %d\n", ret);
    return -1;
  }
  cpu_set->count = 0;
  for (int i = 0; i < core_count; i++) {
    cpu_set->count |= (1 << i);
  }

  return 0;
}

static char *cpuset_to_cstr(cpu_set_t *mask, char *str)
{
  char *ptr = str;
  int i, j, entry_made = 0;
  for (i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, mask)) {
      int run = 0;
      entry_made = 1;
      for (j = i + 1; j < CPU_SETSIZE; j++) {
        if (CPU_ISSET(j, mask)) run++;
        else break;
      }
      if (!run)
        sprintf(ptr, "%d,", i);
      else if (run == 1) {
        sprintf(ptr, "%d,%d,", i, i + 1);
        i++;
      }
      else {
        sprintf(ptr, "%d-%d,", i, i + run);
        i += run;
      }
      while (*ptr != 0) ptr++;
    }
  }
  ptr -= entry_made;
  *ptr = 0;
  return(str);
}

int pthread_setaffinity_np(pthread_t thread, size_t cpu_size,
  cpu_set_t *cpu_set)
{
  thread_port_t mach_thread;
  int core = 0;

  for (core = 0; core < 8 * cpu_size; core++) {
    if (CPU_ISSET(core, cpu_set)) break;
  }
  printf("binding to core %d\n", core);
  thread_affinity_policy_data_t policy = { core };
  mach_thread = pthread_mach_thread_np(thread);
  thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
    (thread_policy_t)&policy, 1);
  return 0;
}

#else

#endif


/* --------------------------------------------------- [ Thread Lifetime ] -- */


struct roa_thread
roa_thread_create(struct roa_thread_desc *desc)
{
  /* param check */
  ROA_ASSERT(desc);

  struct roa_thread new_th;

#ifndef _WIN32
  /* create pthread */
  pthread_attr_init(&new_th->attr);
  pthread_attr_setdetachstate(&new_th->attr, PTHREAD_CREATE_JOINABLE);

  pthread_create(&new_th->thread, NULL, desc->func, desc->arg);

  /* affinity OSX */
  //  int thread;
  cpu_set_t coremask;
  CPU_ZERO(&coremask);
  //  char clbuf[7 * CPU_SETSIZE], hnbuf[64];
  //  char prefix[200];

  //  memset(clbuf, 0, sizeof(clbuf));
  //  memset(hnbuf, 0, sizeof(hnbuf));
  //  (void)gethostname(hnbuf, sizeof(hnbuf));

  //  strcpy(prefix,"Hello world from");

  //  sched_getaffinity(0, sizeof(coremask), &coremask);

  //  cpuset_to_cstr(&coremask, clbuf);
  //  printf("%s thread %d, on %s. (core affinity = %s)\n", prefix, thread, hnbuf, clbuf);
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);

  //  cpu_set_t cpu_set;
  CPU_SET(desc->affinity, &coremask);

  pthread_setaffinity_np(new_th->thread, sizeof(cpu_set_t), &coremask);

  CPU_ZERO(&cpuset);
  sched_getaffinity(0, sizeof(coremask), &coremask);

#else

  HANDLE handle = (HANDLE)_beginthreadex(0, 524288, desc->func, desc->arg, 0, 0);

  new_th.thread = handle;
  new_th.id = GetThreadId(handle);

  return new_th;

#endif

}


struct roa_thread
roa_thread_create_this()
{
  struct roa_thread new_th;

#ifndef _WIN32  

  new_th->thread = pthread_self();

  cpu_set_t cpu_set;
  CPU_SET(0, &cpu_set);

  pthread_setaffinity_np(new_th->thread, sizeof(cpu_set_t), &cpu_set);

#else

  new_th.thread = GetCurrentThread();
  new_th.id = GetThreadId(new_th.thread);

#endif

  return new_th;
}


void
roa_thread_destroy(struct roa_thread *th)
{
  /* param check */
  ROA_ASSERT(th);

  /* todo - why?? */
  //#ifndef _WIN32
  roa_thread_join(th, 1);
  //#endif
}


/* ---------------------------------------------------- [ Thread Actions ] -- */


int
roa_thread_find_this(const struct roa_thread *th, unsigned count)
{
#ifndef _WIN32
  pthread_t self = pthread_self();

  for (unsigned i = 0; i < count; ++i)
  {
    int eq = pthread_equal(self, th[i]->thread);

    if (eq)
    {
      return i;
    }
  }

#else

  DWORD this_id = GetCurrentThreadId();

  char buffer[256];
  memset(buffer, 0, sizeof(buffer));

  sprintf(&buffer[strlen(buffer)], "[");
  sprintf(&buffer[strlen(buffer)], "(%d)", this_id);

  for (unsigned i = 0; i < count; ++i)
  {
    DWORD th_id = GetThreadId(th[i].thread);
    
    sprintf(&buffer[strlen(buffer)], "%d,", (int)th_id);
  }
  sprintf(&buffer[strlen(buffer)], "]");
  printf("%s \n", buffer);

  for (unsigned i = 0; i < count; ++i)
  {
    DWORD th_id = GetThreadId(th[i].thread);

    ROA_ASSERT(th_id == th[i].id);

    if (th[i].id == this_id)
    {
      //printf("%d, %d, %d\n", th_id, this_id, i);
      return i;
    }
  }

#endif

  /* failed to find a thread */
  ROA_ASSERT(0);
  return -1;
}


void
roa_thread_join(struct roa_thread *th, unsigned count)
{
#ifndef _WIN32
  for (unsigned i = 0; i < count; ++i)
  {
    pthread_join(th[i]->thread, NULL);
  }
#else
  for (unsigned i = 0; i < count; ++i)
  {
    WaitForSingleObject(th[i].thread, INFINITE);
    CloseHandle((HANDLE)th[i].thread);
  }
#endif
}


int
roa_thread_core_count()
{
#ifdef __APPLE__
  int nm[2];
  size_t len = 4;
  int count;

  nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
  sysctl(nm, 2, &count, &len, NULL, 0);

  if (count < 1) {
    nm[1] = HW_NCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);
    if (count < 1) { count = 1; }
  }

#elif defined _WIN32
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  const int count = sysinfo.dwNumberOfProcessors;

#endif

  return count;
}
