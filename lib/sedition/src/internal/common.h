#ifndef COMMON_INCLUDED_C97567C7_0EFD_45CE_B14C_0BBE822C4A11
#define COMMON_INCLUDED_C97567C7_0EFD_45CE_B14C_0BBE822C4A11


#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------ [ Memory ] -- */


void *        sed_malloc(size_t bytes);
void          sed_free(void *ptr);


/* ------------------------------------------------------------ [ String ] -- */


char *        sed_string(const char *str);
char *        sed_string_append(char *str, const char *append);


/* -------------------------------------------------------- [ Filesystem ] -- */


void          sed_mkdir(const char *path);

void          sed_file_create(const char *name);
void          sed_file_append(const char *content);
void          sed_file_close();


#ifdef __cplusplus
}
#endif


#endif // inc guard
