/*
    Blit Fragment Shader
    --
    Copies the contents from one target buffer onto another.
    This is intended to copy a FBO onto the backbuffer.
*/

#version 330

/* ------------------------------------------------------------ [ inputs ] -- */

in vec2 fs_texcoord0;

/* ---------------------------------------------------------- [ uniforms ] -- */

uniform sampler2D uni_map_color;

/* ----------------------------------------------------------- [ outputs ] -- */

layout (location = 0) out vec4 fs_out_color;

/* ----------------------------------------------------------- [ program ] -- */

void
main()
{
    fs_out_color  = texture(uni_map_color, fs_texcoord0);
}
