#ifndef VS2017_INCLUDED_
#define VS2017_INCLUDED_


#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


struct Solution;
struct Project;
struct Config;


void sed_generate_vs2017(
  struct Solution *sols,
  struct Project *projs,
  struct Config *confs
);


#ifdef __cplusplus
}
#endif


#endif // inc guard
