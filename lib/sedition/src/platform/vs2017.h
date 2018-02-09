#ifndef VS2017_INCLUDED_
#define VS2017_INCLUDED_


#include <stddef.h>
#include <vector>
#include <internal/types.h>


#ifdef __cplusplus
extern "C" {
#endif

	
void sed_generate_vs2017(
	std::vector<Solution> &sols,
	std::vector<Project> &projs,
	std::vector<Config> &confs
);


#ifdef __cplusplus
}
#endif


#endif // inc guard
