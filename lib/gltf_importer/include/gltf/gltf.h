#ifndef GLTF_IMPORTER_INCLUDED_3B803CF0_C0D5_45D1_BEE0_144BA51B5741
#define GLTF_IMPORTER_INCLUDED_3B803CF0_C0D5_45D1_BEE0_144BA51B5741


#ifdef __cplusplus
extern "C" {
#endif


enum gltf_component_type_e
{
  GLTF_COMPONENT_TYPE_BYTE = 5120,
  GLTF_COMPONENT_TYPE_UNSIGNED_BYTE = 5121,
  GLTF_COMPONENT_TYPE_SHORT = 5122,
  GLTF_COMPONENT_TYPE_UNSIGNED_SHORT = 5123,
  GLTF_COMPONENT_TYPE_UNSIGNED_INT = 5125,
  GLTF_COMPONENT_TYPE_FLOAT = 5126,
};


enum gltf_type_e
{
  GLTF_TYPE_SCALAR,
  GLTF_TYPE_VEC2,
  GLTF_TYPE_VEC3,
  GLTF_TYPE_VEC4,
  GLTF_TYPE_MAT2,
  GLTF_TYPE_MAT3,
  GLTF_TYPE_MAT4,
};


enum gltf_target_e
{
  GLTF_TARGET_NONE,
  GLTF_TARGET_ARRAY_BUFFER = 34962,
  GLTF_TARGET_ELEMENT_ARRAY_BUFFER = 34963,
};


struct gltf_accessor
{
  int buffer_view;              /* optional */
  int byte_offset;              /* optional */
  int component_type;           /* gltf_type_e */
  int normalized;               /* 0 or 1 */
  int count;                    /* number of attributes */
  int type;                     /* gltf_type_e */
  float *max;                   /* optional */
  int max_count;

  float *min;                   /* optional */
  int min_count;
  /* missing - sparse */
  char *name;                   /* optional */
  /* missing - extensions */
  /* missing - extras */
};


struct gltf_buffer_view
{
  int buffer;
  int byte_offset;
  int byte_length;
  int byte_stride;
  int target;
  char *name;
  /* missing - extensions */
  /* missing - extras */
};


struct gltf_buffer
{
  unsigned char *uri;
  int byte_length;
  char *name;
  /* missing - extensions */
  /* missing - extras */
};


struct gltf_attributes
{
  int NORMAL;
  int POSITION;
  int TANGENT;
  int TEXCOORD_0;
};


struct gltf_primitives
{
  struct gltf_attributes attributes;
  int indices;
};


struct gltf_mesh
{
  char name[64];
  struct gltf_primitives primitives;
};


struct gltf_node
{
  char name[64];
  float rotation[4];
  float translation[3];
  float scale[3];

  int mesh;
};


struct gltf_import
{
  struct gltf_accessor *accessors;
  int accessor_count;

  struct gltf_buffer_view *buffer_views;
  int buffer_view_count;

  struct gltf_buffer *buffers;
  int buffer_count;

  struct gltf_mesh *meshes;
  int mesh_count;
};


void
gltf_import(const char *file, struct gltf_import *out_scene);


void
gltf_free(struct gltf_import *scene);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
