/*
    GBuffer Fill Vertex Shader
*/

#version 330

/* ----------------------------------------------------------- [ inputs ] -- */

layout(location = 0) in vec3 vs_position0;
layout(location = 1) in vec2 vs_texcoord0;
layout(location = 2) in vec3 vs_normal0;

/* --------------------------------------------------------- [ uniforms ] -- */

uniform mat4 uni_wvp;
uniform mat4 uni_world;

/* ---------------------------------------------------------- [ outputs ] -- */

out vec2 fs_texcoord0;
out vec3 fs_normal0;
out vec3 fs_worldpos0;

/* ---------------------------------------------------------- [ program ] -- */

void
main()
{
    gl_Position  = uni_wvp * vec4(vs_position0, 1.0);
    fs_texcoord0 = vs_texcoord0;
    fs_normal0   = (uni_world * vec4(vs_normal0, 0.0)).xyz;
    fs_worldpos0 = (uni_world * vec4(vs_position0, 1.0)).xyz;
}
