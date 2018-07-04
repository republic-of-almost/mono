#ifdef ROA_RENDERER_API_GL4
#ifndef OGL_CTX_INCLUDED_5774619E_CC5C_43DF_B796_799C8DDFC489
#define OGL_CTX_INCLUDED_5774619E_CC5C_43DF_B796_799C8DDFC489


#include <GL/gl3w.h>


struct ogl_vertex_input
{
  GLint loc;
  GLboolean normalize;
  GLsizei component_count;
  GLsizei size;
  GLvoid *ptr;
};


struct ogl_gbuffer
{
  GLuint fbo;
  GLuint texture_output[5];
  GLuint texture_depth;
};


struct ogl_gbuffer_dir_light
{
  GLuint program;
};


struct ogl_gbuffer_fill_pass
{
  GLuint program;

  GLuint uni_wvp;
  GLuint uni_world;
  GLuint uni_diffuse;
  GLuint uni_object_id;

  struct ogl_vertex_input input[3];
};


struct ogl_blit_pass
{
  GLuint program;
  GLuint fullscreen_triangle;

  GLuint uni_blit_src;

  struct ogl_vertex_input input[2];
};


struct ogl_decal
{
  GLuint program;
  GLuint vbo;

  GLint uni_world_pos;
  GLint uni_depth;
  GLint uni_diffuse;
  GLint uni_map_object_ids;
  GLint uni_view;
  GLint uni_proj;
  GLint uni_world;
  GLint uni_inv_projview;
  GLint uni_inv_world;
  GLint uni_color;
  GLint uni_object_id;

  struct ogl_vertex_input input[3];
};


struct ogl_mesh
{
  GLuint vbo;
  int vertex_count;

  GLuint ibo;
  int index_count;
  GLenum index_type;
};


struct graphics_api
{
  /* global VAO */
  GLuint vao;

  /* gbuffer */
  struct ogl_gbuffer gbuffer;
  struct ogl_gbuffer_fill_pass gbuffer_fill;
  struct ogl_gbuffer_dir_light dir_light;
  struct ogl_blit_pass blit;
  struct ogl_decal decal;

  uint64_t *mesh_ids;
  struct ogl_mesh *meshes;

  GLuint mesh_program;
  GLuint vbo;
  GLuint tex;
};


#endif /* inc guard */
#endif /* platform guard */
