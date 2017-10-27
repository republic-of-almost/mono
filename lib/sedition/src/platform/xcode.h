#ifndef XCODE_INCLUDED_2AE9499F_43D1_4BE8_86B7_B55CD13F2485
#define XCODE_INCLUDED_2AE9499F_43D1_4BE8_86B7_B55CD13F2485


#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


struct Solution;
struct Project;
struct Config;


void sed_generate_xcode(
  struct Solution *sols,
  struct Project *projs,
  struct Config *confs
);


#ifdef __cplusplus
}
#endif


#endif // inc guard
