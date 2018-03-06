#ifndef REP_API_INCLUDED_37A05CEF_D8DD_4173_807E_5DC16131C13A
#define REP_API_INCLUDED_37A05CEF_D8DD_4173_807E_5DC16131C13A


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Basic Types ] -- */


#ifndef __cplusplus
#ifdef _MSC_VER
typedef unsigned __int64 uint64_t;
#endif
#else
#include <cstdint>
#endif


typedef void(*rep_task)(void*);


/* ------------------------------------------------------- [ Application ] -- */


struct rep_app_desc
{
  const char *title;
  int width;
  int height;

  rep_task *frame_job;
};


void
rep_app_create(
  struct rep_app_desc * desc);


void
rep_app_get(
  struct rep_app_desc * out_desc);


int
rep_app_new_frame();


void
rep_app_destroy();


/* -------------------------------------------------------------- [ Task ] -- */


unsigned
rep_submit_tasks(
  rep_task * tasks,
  unsigned count);


void
rep_wait_for_tasks(
  unsigned marker);


/* ------------------------------------------------------------ [ Object ] -- */


struct rep_object_desc
{
  const char * name;
  uint64_t object_parent;
};


void
rep_object_create(
  uint64_t * out_object_ids,
  const struct rep_object_desc * desc,
  unsigned count);


void
rep_object_update(
  const uint64_t * object_ids,
  const struct rep_object_desc * desc,
  unsigned count);


void
rep_object_destroy(
  const uint64_t * object_ids,
  unsigned count);


/* --------------------------------------------------------- [ Transform ] -- */


struct rep_transform_desc
{
  float position[3];
  float scale[3];
  float rotation[4];
};


void
rep_transform_set(
  uint64_t * object_ids,
  struct rep_transform_desc * desc,
  unsigned count);


void
rep_transform_get(
  const uint64_t * object_ids,
  struct rep_transform_desc * desc,
  unsigned count);


/* ------------------------------------------------------------ [ Camera ] -- */


struct rep_camera_desc
{
  unsigned width;
  unsigned height;
  float fov;
};


void
rep_camera_set(
  uint64_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count);


void
rep_camera_get(
  const uint64_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count);


/* --------------------------------------------------- [ Mesh Renderable ] -- */


struct rep_mesh_renderable_desc
{
  uint64_t mesh_id;
  uint64_t material_id;
};


void
rep_mesh_renderable_set(
  uint64_t * object_ids,
  struct rep_mesh_renderable_desc * desc,
  unsigned count);


void
rep_mesh_renderable_get(
  const uint64_t * object_ids,
  struct rep_mesh_renderable_desc * desc,
  unsigned count);


/* -------------------------------------------------------------- [ Mesh ] -- */


void
rep_mesh_find(
  uint64_t * out_mesh_ids,
  const char ** mesh_names,
  unsigned mesh_name_count);


/* ---------------------------------------------------------- [ Material ] -- */


void
rep_material_find(
  uint64_t * out_material_ids,
  const char ** material_names,
  unsigned material_name_count);


#ifdef __cplusplus
} /* extern C */
#endif


#endif /* inc guard */