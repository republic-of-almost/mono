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


enum gltf_primitive_mode_e
{
  GLTF_PRIM_MODE_POINTS = 0,
  GLTF_PRIM_MODE_LINES = 1,
  GLTF_PRIM_MODE_LINE_LOOP = 2,
  GLTF_PRIM_MODE_LINE_STRIP = 3,
  GLTF_PRIM_MODE_TRIANGLES = 4,
  GLTF_PRIM_MODE_TRIANGLE_STRIP = 5,
  GLTF_PRIM_MODE_TRIANGLE_FAN = 6,
};


struct gltf_accessor
{
  int buffer_view;              /* optional */
  int byte_offset;              /* optional */
  int component_type;           /* gltf_component_type_e */
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
  char *uri;
  unsigned char *uri_data;

  int byte_length;
  char *name;
  /* missing - extensions */
  /* missing - extras */
};


struct gltf_attributes
{
  int POSITION;     /* vec3 - Float */
  int NORMAL;       /* vec3 - Float */
  int TANGENT;      /* vec4 - Float */
  int TEXCOORD_0;   /* vec2 - Float, UByte, UShort*/
  int TEXCOORD_1;   /* vec2 - Float Ubyte, UShort */
  int COLOR_0;      /* vec3/vec4 - Float, Ubyte, UShort */
  int JOINTS_0;     /* vec4 - UByte, UShort */
  int WEIGHTS_0;    /* vec4 - Float, UByte, UShort */
};


struct gltf_primitive
{
  struct gltf_attributes attributes;
  int indices;
  int material;
  int mode;

  /* missing - targets */
  /* missing - extensions */
  /* missing - extras */
};


struct gltf_mesh
{
  char *name;
  struct gltf_primitive *primitives;
  int primitive_count;

  /* missing - weights */
  /* missing - extensions */
  /* missing - extras */
};


struct gltf_node
{
  char *name;

  float *rotation;
  int rotation_count;

  float *translation;
  int transloation_count;

  float *scale;
  int scale_count;

  int *children;
  int child_count;

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

  struct gltf_nodes *nodes;
  int node_count;
};


void
gltf_import(const char *file, struct gltf_import *out_scene);


void
gltf_free(struct gltf_import *scene);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
