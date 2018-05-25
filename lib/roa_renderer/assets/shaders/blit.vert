/*
    Blit Vertex Shader
    --
    Transforms a fullscreen triangle.
*/

#version 330

/* ----------------------------------------------------------- [ inputs ] -- */

layout(location = 0) in vec2 vs_position0;
layout(location = 1) in vec2 vs_texcoord0;

/* ---------------------------------------------------------- [ outputs ] -- */

out vec2 fs_texcoord0;

/* ---------------------------------------------------------- [ program ] -- */

void
main()
{
    gl_Position = vec4(vs_position0, 0.0, 1.0);
    fs_texcoord0 = vs_texcoord0;
}
