/*
    GBuffer Fill Fragment Shader
    --
    This will write out the various bits of data to buffers
*/

#version 330

/* ----------------------------------------------------------- [ inputs ] -- */

in vec2 fs_texcoord0;
in vec3 fs_normal0;
in vec3 fs_worldpos0;

/* --------------------------------------------------------- [ uniforms ] -- */

uniform sampler2D uni_map_diffuse;

/* ---------------------------------------------------------- [ outputs ] -- */

layout (location = 0) out vec3 fs_out_worldpos;
layout (location = 1) out vec3 fs_out_diffuse;
layout (location = 2) out vec3 fs_out_normal;
layout (location = 3) out vec3 fs_out_texcoord;

/* ---------------------------------------------------------- [ program ] -- */

void
main()
{
    /* write out to gbuffer */
    fs_out_worldpos = fs_worldpos0;
    fs_out_diffuse  = texture(uni_map_diffuse, fs_texcoord0).xyz;
    fs_out_normal   = normalize(fs_normal0);
    fs_out_texcoord = vec3(fs_texcoord0, 0.0);
}
