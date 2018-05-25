/*
    Decal Vert Shader
    --
    Transforms projection geometry.
*/

#version 410

/* ----------------------------------------------------------- [ inputs ] -- */

layout ( location = 0 ) in vec4 vs_position0;
layout ( location = 1 ) in vec2 vs_texcoord0;
layout ( location = 2 ) in vec3 vs_normal0;

/* --------------------------------------------------------- [ uniforms ] -- */

uniform mat4 uni_view;
uniform mat4 uni_proj;
uniform mat4 uni_world;

/* ---------------------------------------------------------- [ outputs ] -- */

out vec4 fs_position0;
out vec4 fs_position_w0;
out vec2 fs_texcoord0;

/* ---------------------------------------------------------- [ program ] -- */

void
main()
{
    fs_position_w0 =  uni_world * vec4(vs_position0.xyz * 1, vs_position0.w);
    fs_position0 = uni_proj * uni_view * fs_position_w0;
    fs_texcoord0 = vs_texcoord0;
    gl_Position = fs_position0;
}
