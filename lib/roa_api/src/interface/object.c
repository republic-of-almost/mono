#include <roa/roa_api.h>
#include <roa_lib/assert.h>

/*------------------------------------------------------- [ Object Jobs ] -- */


struct codex_transform
{
  float position[3];
  float scale[3];
  float rotation[4];
};


struct codex_object_writer
{
  struct codex_transform local_transform;
  struct codex_transform world_transform;
};


struct roa_obj_create_data {
  const struct roa_object_desc *desc;
};


ROA_JOB(job_obj_new, struct roa_obj_create_data*)
{
  ROA_ASSERT(arg != ROA_NULL);

  struct codex_transform default_transform;

  default_transform.position[0] = 0.f;
  default_transform.position[1] = 0.f;
  default_transform.position[1] = 0.f;

  default_transform.scale[0] = 1.f;
  default_transform.scale[1] = 1.f;
  default_transform.scale[1] = 1.f;

  default_transform.rotation[0] = 0.f;
  default_transform.rotation[1] = 0.f;
  default_transform.rotation[2] = 0.f;
  default_transform.rotation[3] = 1.f;

  struct codex_object_writer *writer;
  writer->local_transform = default_transform;
  writer->world_transform = default_transform;

  codex_object_write(writer);
}


/* -------------------------------------------------------- [ Object API ] -- */


struct roa_object
{
  unsigned i;
};


ROA_BOOL
roa_object_create(
  roa_object *out_objects,
  const struct roa_object_desc *in_desc,
  unsigned count)
{
  ROA_ASSERT(out_objects);
  ROA_ASSERT(in_desc);
  ROA_ASSERT(count);

  if (count)
  {
    /* create arg spin off jobs */
  }
}


void
roa_object_destroy(
  const roa_object *in_objects,
  unsigned count)
{
  ROA_ASSERT(in_objects);
  ROA_ASSERT(count);

  if (count)
  {

  }
}


void
roa_object_set(
  const roa_object *in_objects,
  const struct roa_object_desc *in_desc,
  unsigned count)
{
  ROA_ASSERT(in_objects);
  ROA_ASSERT(in_desc);
  ROA_ASSERT(count);

  if (count)
  {

  }
}