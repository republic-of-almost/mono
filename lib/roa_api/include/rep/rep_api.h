#ifndef REP_API_INCLUDED_37A05CEF_D8DD_4173_807E_5DC16131C13A
#define REP_API_INCLUDED_37A05CEF_D8DD_4173_807E_5DC16131C13A


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Basic Types ] -- */


#ifndef __cplusplus
#ifdef _MSC_VER
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
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

  rep_task frame_job;
  void *frame_arg;
};


void
rep_app_create(
  const struct rep_app_desc * desc);


void
rep_app_get(
  struct rep_app_desc * out_desc);


void
rep_app_set(
  const struct rep_app_desc * desc);


void
rep_app_destroy();


/* -------------------------------------------------------------- [ Task ] -- */


struct rep_task_desc
{
  rep_task function;
  void *arg;
};


uint64_t
rep_task_submit(
  struct rep_task_desc * tasks,
  unsigned count);


void
rep_task_wait(
  uint64_t marker);


/* ------------------------------------------------------------ [ Object ] -- */


struct rep_object_desc
{
	uint32_t object_parent;
};


void
rep_object_create(
  uint32_t * out_object_ids,
  const struct rep_object_desc * desc,
  unsigned count);


void
rep_object_update(
  const uint32_t * object_ids,
  const struct rep_object_desc * desc,
  unsigned count);


void
rep_object_destroy(
  const uint32_t * object_ids,
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
  uint32_t * object_ids,
  struct rep_transform_desc * desc,
  unsigned count);


void
rep_transform_get(
  const uint32_t * object_ids,
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
  uint32_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count);


void
rep_camera_get(
  const uint32_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count);


/* --------------------------------------------------- [ Mesh Renderable ] -- */


struct rep_mesh_renderable_desc
{
  uint64_t material_id;
  uint64_t mesh_id;
};


void
rep_mesh_renderable_set(
  uint32_t * object_ids,
  struct rep_mesh_renderable_desc * desc,
  unsigned count);


void
rep_mesh_renderable_get(
  const uint32_t * object_ids,
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
