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
/*
 *  TODO: Ownership needs to be clearer here.
 */

char *        sed_string(const char *str);
char *        sed_string_append(char *str, const char *append);
void          sed_string_free(char *str);
const char *        sed_string_filename_from_path(const char *path);
const char *        sed_string_ext_from_filename(const char *path);


/* -------------------------------------------------------- [ Filesystem ] -- */


void          sed_mkdir(const char *path);

void          sed_file_create(const char *name);
void          sed_file_append(const char *content);
void          sed_file_close();


#ifdef __cplusplus
}
#endif


#endif // inc guard
