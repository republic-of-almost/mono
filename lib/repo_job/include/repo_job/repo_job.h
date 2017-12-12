#ifndef REPO_JOB_INCLUDED_5F6978AB_FEE9_439A_8F3C_6759804BD4BE
#define REPO_JOB_INCLUDED_5F6978AB_FEE9_439A_8F3C_6759804BD4BE


#ifdef __cplusplus
extern "C" {
#endif


struct repo_engine;


/* ------------------------------------------------------ [ Entry Points ] -- */


void
repo_api_register(struct repo_engine *engine);


void
repo_api_think();


void
repo_api_destroy();


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */
