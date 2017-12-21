#ifndef EDGE_INCLUDED_08B4B0D3_0CE9_4717_A690_1DCA7B859B6D
#define EDGE_INCLUDED_08B4B0D3_0CE9_4717_A690_1DCA7B859B6D


#ifndef __cplusplus
extern "C" {
#endif


#if defined(_MSC_VER) && (_MSC_VER < 1600)
  typedef signed   __int8  int8_t;
  typedef unsigned __int8  uint8_t;
  typedef signed   __int16 int16_t;
  typedef unsigned __int16 uint16_t;
  typedef signed   __int32 int32_t;
  typedef unsigned __int32 uint32_t;
  typedef signed   __int64 int64_t;
  typedef unsigned __int64 uint64_t;
#else
  #include <stdint.h>
#endif


/* -------------------------------------------------------------- [ Edge ] -- */


typedef void(*edge_job_fn)(void *arg);


struct edge_app_desc
{
  edge_job_fn job_entry;
};


int
edge_app_create(
  const struct edge_app_desc            *desc);


int
edge_app_new_frame();


int
edge_app_destroy();


/* -------------------------------------------------------------- [ Edge ] -- */


struct edge_window_desc
{
  unsigned    width;
  unsigned    height;
  const char  *title;
};


int
edge_window_desc_get(
  struct edge_window_desc               *desc);


int
edge_window_desc_set(
  struct edge_window_desc               *desc);


/* --------------------------------------------------------------- [ Job ] -- */


struct edge_job_desc
{
  edge_job_fn   function;
  void          *argument;
};


unsigned
edge_job_submit(
  struct edge_job_desc                  desc[],
  unsigned                              count);


int
edge_job_wait(
  unsigned                              batch_id);


/* ------------------------------------------------------------ [ Object ] -- */


struct edge_object_desc
{
  uint64_t    parent_id;
  const char  *object_name;
};


int
edge_object_create(
  uint64_t                              out_ids[],
  const struct edge_object_desc         desc[],
  unsigned                              count);


int
edge_object_destroy(
  const uint64_t                        ids[],
  unsigned                              count);


int
edge_object_desc_get(
  const uint64_t                        ids[],
  struct edge_object_desc               out_desc[],
  unsigned                              count);


int
edge_object_desc_set(
  const uint64_t                        ids[],
  const struct edge_object_desc         out_desc[],
  unsigned                              count);


/* --------------------------------------------------------- [ Transform ] -- */


struct edge_transform_desc
{
  float position[3];
  float scale[3];
  float rotation[4];
};


int
edge_local_transform_get(
  const uint64_t                        ids[],
  struct edge_transform_desc            desc[],
  unsigned                              count);


int
edge_local_trasnform_set(
  const uint64_t                        ids[],
  const struct edge_transform_desc      desc[],
  unsigned                              count);


int
edge_world_transform_get(
  const uint64_t                        ids[],
  struct edge_transform_desc            desc[],
  unsigned                              count);


int
edge_world_trasnform_set(
  const uint64_t                        ids[],
  const struct edge_transform_desc      desc[],
  unsigned                              count);


/* ------------------------------------------------------ [ Bounding box ] -- */


struct edge_bounding_box_desc
{
  float min[3];
  float max[3];
};


int
edge_axis_aligned_bounding_box_get(
  const uint64_t                        ids[],
  struct edge_bounding_box_desc         desc[],
  unsigned                              count);


/* -------------------------------------------------------------- [ Mesh ] -- */


struct edge_mesh_desc
{
  float *positions;
  float *normals;
  float *texture_coords;
  unsigned vert_count;

  float *index;
  unsigned index_count;
};


int
edge_mesh_create(
  uint64_t                            out_ids[],
  struct edge_mesh_desc               desc[],
  unsigned                            count);


/* ---------------------------------------------------------- [ Material ] -- */


struct edge_material_desc
{
  int shader_id;
};


/* -------------------------------------------------------- [ Renderable ] -- */


struct edge_renderable_desc
{
  const char *mesh;
  const char *material;

  void *renderpass;
};


int
edge_renderable_get(
  const uint64_t                        ids[],
  struct edge_renderable_desc           desc[],
  unsigned                              count);


int
edge_renderable_set(
  const uint64_t                        ids[],
  const struct edge_renderable_desc     desc[],
  unsigned                              count);


/* ------------------------------------------------------------ [ Camera ] -- */


struct edge_camera_desc
{
  float near_plane;
  float far_plane;
  float fov;
  int   projection;
};


int
edge_camera_get(
  const uint64_t                        ids[],
  struct edge_camera_desc               desc[],
  unsigned                              count);


int
edge_camera_set(
  const uint64_t                        ids[],
  const struct edge_camera_desc         desc[],
  unsigned                              count);


/* -------------------------------------------------------- [ Renderpass ] -- */


#ifndef __cplusplus
} /* extern */
#endif

#endif /* inc guard */