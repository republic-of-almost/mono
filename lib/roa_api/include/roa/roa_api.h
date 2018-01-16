#ifndef ROA_API_INCLUDED_37A05CEF_D8DD_4173_807E_5DC16131C13A
#define ROA_API_INCLUDED_37A05CEF_D8DD_4173_807E_5DC16131C13A


#ifdef __cplusplus
extern "C" {
#endif


/* ----------------------------------------------------- [ ROA API Types ] -- */


/* boolean */
typedef int ROA_BOOL;

#define ROA_TRUE 1
#define ROA_FALSE 0


/* callbacks */
typedef void(*roa_job_func)(void *unused, void *argument);


/* other */
#define ROA_NULL 0
#define ROA_NULL_HANDLE 0


/* ----------------------------------------------- [ Application Handles ] -- */




/* --------------------------------------------------- [ Application API ] -- */


struct roa_application_desc
{
  const char    *title;
  unsigned      width;
  unsigned      height;
  ROA_BOOL      fullscreen;
  roa_job_func  application_job;
  void*         application_arg;
};


ROA_BOOL
roa_application_create(
  const struct roa_application_desc *desc);


ROA_BOOL
roa_application_destroy();


/* ----------------------------------------------- [ Application Job API ] -- */


#define ROA_JOB(job_name, arg_type)           \
void job_wrap_##job_name##(arg_type arg);     \
                                              \
void                                          \
job_name(void *unused, void *void_arg)        \
{                                             \
  (void)unused;                               \
  job_wrap_##job_name##((arg_type)void_arg);  \
}                                             \
                                              \
void job_wrap_##job_name##(arg_type arg)


struct roa_job_desc
{
  roa_job_func  function;
  void          *argument;
  ROA_BOOL      lock_to_thread;
};


typedef unsigned roa_job_batch;

roa_job_batch
roa_job_batch_submit(
  struct roa_job_desc *desc,
  unsigned count);


void
roa_job_batch_wait(
  roa_job_batch batch);


/* ---------------------------------------------------- [ Object Handles ] -- */


typedef struct roa_object*                roa_object;
typedef struct roa_object_transform*      roa_object_transform;
typedef struct roa_object_mesh_instance*  roa_object_mesh_instance;


/* -------------------------------------------------------- [ Object API ] -- */


struct roa_object_desc
{
  roa_object parent_object;
  const char *name;
};


ROA_BOOL
roa_object_create(
  roa_object *out_objects,
  const struct roa_object_desc *in_desc,
  unsigned count);


void
roa_object_destroy(
  const roa_object *in_objects,
  unsigned count);


void
roa_object_set(
  const roa_object *in_objects,
  const struct roa_object_desc *in_desc,
  unsigned count);


/* -------------------------------------------------- [ Resource handles ] -- */


typedef struct roa_resource_mesh*         roa_resource_mesh;
typedef struct roa_resource_material*     roa_resource_material;


/* ------------------------------------------------------ [ Resource API ] -- */


#ifdef __cplusplus
} /* extern C */
#endif


#endif /* inc guard */