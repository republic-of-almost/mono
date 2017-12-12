#ifndef ENGINE_INCLUDED_EBF8E13C_AC17_4253_B0CA_7AC7C84FDE69
#define ENGINE_INCLUDED_EBF8E13C_AC17_4253_B0CA_7AC7C84FDE69


/* --------------------------------------------------- [ Engine Lifetime ] -- */


struct Repo_engine_ctx;


void    repo_engine_create(struct Repo_engine_ctx **c);
void    repo_engine_tick(struct Repo_engine_ctx *c);
void    repo_engine_destroy(struct Repo_engine_ctx **c);


#endif /* inc guard */

