#ifndef REPO_JOB_INCLUDED_5F6978AB_FEE9_439A_8F3C_6759804BD4BE
#define REPO_JOB_INCLUDED_5F6978AB_FEE9_439A_8F3C_6759804BD4BE


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------ [ Entry Points ] -- */


struct repo_engine;


__declspec(dllexport) void __cdecl
repo_module_create();


void
repo_module_destroy();


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */
