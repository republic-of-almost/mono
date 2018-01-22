#include <stdio.h>
#include <fiber.h>
#include <string.h>
#include <assert.h>


/* ---------------------------------------------------------- [ Settings ] -- */


const int fiber_count = 8; /* if you change this update the test str */

const char expected_str[] = "12345678";
char str[fiber_count + 1];


/* --------------------------------------------------------- [ Test Vars ] -- */


struct roa_fiber *local_fiber;
struct roa_fiber *fibers[fiber_count];
int last_fiber = 0;


/* ---------------------------------------------------- [ Fiber Function ] -- */


void
fiber_function(void *arg)
{
  printf("last fiber index %d\n", last_fiber);

  char c = 48 + (int)arg;
  str[last_fiber] = c;

  printf("arg %p", arg);

  last_fiber++;

  if(last_fiber < fiber_count)
  {
    printf("fiber switch\n");
    roa_fiber_switch(fibers[last_fiber - 1], fibers[last_fiber]);
  }
  else
  {
    printf("fiber return\n");
    roa_fiber_switch(fibers[fiber_count - 1], local_fiber);
  }

  /* you should never get here - the fibers should switch before this point */
  /* this function has no return */
  assert(0);
}


/* -------------------------------------------------------------- [ Test ] -- */


int
main()
{
  memset(str, 0, sizeof(str));

  /* create fibers */
  roa_fiber_create(&local_fiber, 0, (void*)0);

  for(int i = 0; i < fiber_count; ++i)
  {
    /* produces a warning - as expected */
    int int_arg = i + 1;
    void *arg = (void*)int_arg;

    roa_fiber_create(&fibers[i], fiber_function, arg);
  }

  /* switch to fiber */
  printf("start\n");
  roa_fiber_switch(local_fiber, fibers[0]);
  printf("end\n");

  /* clean up */
  roa_fiber_destroy(local_fiber);

  for(int i = 0; i < fiber_count; ++i) {
    roa_fiber_destroy(fibers[i]);
  }

  printf("str: %s\n", str);

  assert(strcmp(str, expected_str) == 0);

  return 0;
}
