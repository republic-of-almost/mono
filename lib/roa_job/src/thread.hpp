#ifndef THREAD_INCLUDED_620C4CB9_916F_4A5E_8389_DC5D456E21A4
#define THREAD_INCLUDED_620C4CB9_916F_4A5E_8389_DC5D456E21A4

#ifndef _WIN32
#include <sys/param.h>
#include <sys/sysctl.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#else
#define WIN_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#endif


/* ---------------------------------------------------- [ Thread Fwd Dec ] -- */



struct roa_thread
{
#ifndef _WIN32
  pthread_t thread;
  pthread_attr_t attr;
#else
  HANDLE thread;
  DWORD id;
#endif
};


#ifdef _WIN32
typedef unsigned(__stdcall *thread_func)(void *arg);
#else
typedef void*(*thread_func)(void*);
#endif

/* ------------------------------------------------------ [ Thread Types ] -- */


struct roa_thread_desc
{
  thread_func func;
  void *arg;
  int affinity;
};


/* --------------------------------------------------- [ Thread Lifetime ] -- */
/*
Thread lifetime create and destroy.
*/

void
roa_thread_create(struct roa_thread *th, struct roa_thread_desc *desc);


struct roa_thread
roa_thread_create_this();


void
roa_thread_destroy(struct roa_thread *th);


/* ---------------------------------------------------- [ Thread Actions ] -- */
/*
Various actions
*/


int
roa_thread_find_this(struct roa_thread *th, unsigned count);


void
roa_thread_join(struct roa_thread *th, unsigned count);


int
roa_thread_core_count();


#endif /* inc guard */
