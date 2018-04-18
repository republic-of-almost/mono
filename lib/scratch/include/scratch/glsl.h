#ifndef GLSL_INCLUDED_98768361_051F_49A9_B17C_C533B8C6C088
#define GLSL_INCLUDED_98768361_051F_49A9_B17C_C533B8C6C088


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------ [ Two Textures ] -- */
/*
  Program that blends between two textures.

  Input
  --
  layout(location=0) in vec3 vs_in_position
  layout(location=2) in vec2 vs_in_texcoord

  Uniforms
  --
  uniform mat4 uni_wvp_mat

  Samplers
  --
  uniform sampler2D samp_diffuse_01
  uniform sampler2D samp_diffuse_02

  Output
  --
  out vec4 fs_out_fragcolor
*/

const char*
glsl_two_textures_vs();


const char*
glsl_two_textures_fs();



/* -------------------------------------------------------- [ Fullbright ] -- */
/*
  Program that simply renders mesh with a diffuse texture, no lighting or
  anything fancy.

  Input
  --
  layout(location=0) in vec3 vs_in_position
  layout(location=2) in vec2 vs_in_texcoord

  Uniforms
  --
  uniform mat4 uni_wvp_mat

  Samplers
  --
  uniform sampler2D samp_diffuse_01

  Output
  --
  out vec4 fs_out_fragcolor
*/

const char*
glsl_fullbright_vs();


const char*
glsl_fullbright_fs();


/* ---------------------------------------------------- [ Colored Render ] -- */
/*
  Program that renders a mesh a certain color

  Input
  --
  layout(location=0) in vec3 vs_in_position

  Uniforms
  --
  uniform mat4 uni_wvp_mat
  uniform vec4 uni_color

  Output
  --
  out vec4 fs_out_fragcolor
*/


const char*
glsl_colored_vs();


const char*
glsl_colored_fs();


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
