#include <monocle/monocle.h>
#include <stdio.h>


void
ct_module_register()
{
  printf("Reg Module\n");
}


void
ct_module_start()
{
  printf("Start module\n");
}


void
ct_module_destroy()
{
  printf("Reg Destroy Module\n");
}


void
ct_module_tick()
{
  printf("Tick Module\n");
}
