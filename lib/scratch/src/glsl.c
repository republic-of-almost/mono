#include <scratch/glsl.h>


/* ------------------------------------------------------ [ Two Textures ] -- */


const char*
glsl_two_textures_vs()
{
  static const char *vert_src = ""
    "#version 400 core\n"

    "layout(location=0) in vec3 vs_in_position;\n"
    "layout(location=2) in vec2 vs_in_texcoord;\n"

    "uniform mat4 uni_wvp_mat;\n"

    "out vec2 fs_in_texcoord;\n"

    "void main() {\n"
    "  fs_in_texcoord = vs_in_texcoord;\n"
    "  gl_Position = uni_wvp_mat * vec4(vs_in_position, 1.0);\n"
    "}\n";

  return vert_src;
}


const char*
glsl_two_textures_fs()
{
  const char* frag_src = ""
    "#version 400 core\n"

    "in vec2 fs_in_texcoord;\n"

    "uniform sampler2D samp_diffuse_01;\n"
    "uniform sampler2D samp_diffuse_02;\n"

    "out vec4 fs_out_fragcolor;\n"

    "void main() {\n"
    "  float x_val = ((fs_in_texcoord.x * 2) - 1.0) * 5.0;\n"
    "  float y_val = ((fs_in_texcoord.y * 2) - 1.0) * 3.0;\n"
    "  float mix_value = 20 * sin(((x_val * x_val) + (y_val * y_val)));\n"
    "  fs_out_fragcolor = mix(texture(samp_diffuse_01, fs_in_texcoord), texture(samp_diffuse_02, fs_in_texcoord), clamp(mix_value, 0, 1));"
    "}\n";

  return frag_src;
}


/* -------------------------------------------------------- [ Fullbright ] -- */


const char*
glsl_fullbright_vs()
{
  static const char *vert_src = ""
    "#version 400 core\n"

    "layout(location=0) in vec3 vs_in_position;\n"

    "uniform mat4 uni_wvp_mat;\n"

    "out vec2 fs_in_texcoord;\n"

    "void main() {\n"
    "  fs_in_texcoord = vs_in_texcoord;\n"
    "  gl_Position = uni_wvp_mat * vec4(vs_in_position, 1.0);\n"
    "}\n";

  return vert_src;
}


const char*
glsl_fullbright_fs()
{
  const char* frag_src = ""
    "#version 400 core\n"

    "in vec2 fs_in_texcoord;\n"

    "uniform sampler2D samp_diffuse_01;\n"

    "out vec4 fs_out_fragcolor;\n"

    "void main() {\n"
    /*"  fs_out_fragcolor = texture(samp_diffuse_01, fs_in_texcoord);"*/
    "  fs_out_fragcolor = vec4(1,0,0,1);\n"
    "}\n";

  return frag_src;
}


/* -------------------------------------------------------- [ Fullscreen ] -- */


const char*
glsl_blit_vs()
{
  static const char *vert_shd =
    "#version 400 core\n"

    "layout(location=0) in vec3 vs_in_position;\n"
    "layout(location=1) in vec2 vs_in_texcoord;\n"

    "out vec2 fs_in_texcoord;\n"

    "void main()\n"
    "{\n"
    "  gl_Position = vec4(vs_in_position, 1.0);\n"
    "  fs_in_texcoord = vs_in_texcoord;\n"
    "}\n";

  return &vert_shd[0];
}


const char*
glsl_blit_fs()
{
  static const char *frag_shd =
    "#version 400 core\n"

    "in vec2 fs_in_texcoord;\n"

    "uniform sampler2D samp_diffuse_01;\n"

    "out vec4 fs_out_fragcolor;\n"

    "void main()\n"
    "{\n"
    " fs_out_fragcolor = texture(samp_diffuse_01, fs_in_texcoord);\n"
    "}\n";

  return &frag_shd[0];
}
