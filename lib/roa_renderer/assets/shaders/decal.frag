/*
    Decal Frag Shader
    --
    Trims off projector fragments that do not intersect with underlying
    geometry.
*/

/* ------------------------------------------------------------ [ config ] -- */

#version 410
//"#extension GL_ARB_texture_rectangle : enable

/* 1 will not cull any fragments, helps with debuging if you suspect the
    projector is indside geometry */
#define DEBUG_NO_DISCARD 0

/* ------------------------------------------------------------ [ inputs ] -- */

in vec4 fs_position0;
in vec4 fs_position_w0;
in vec2 fs_texcoord0;

/* ---------------------------------------------------------- [ uniforms ] -- */

uniform sampler2D uni_map_diffuse;
uniform sampler2D uni_map_depth;
uniform sampler2D uni_map_worldpos;
uniform sampler2D uni_map_object_ids;
uniform vec3 uni_color;
uniform mat4 uni_inv_world;
uniform float uni_object_id;

/* ----------------------------------------------------------- [ outputs ] -- */

layout ( location = 0 ) out vec4 fs_out_diffuse;

/* ----------------------------------------------------------- [ program ] -- */

void
main()
{
    vec2 screen_pos = fs_position0.xy / fs_position0.w;
    vec2 texcoord = screen_pos * 0.5f + 0.5f;

    float object_id = texture(uni_map_object_ids, texcoord).r;

    /* ensure we don't render decal on another object */
    if(abs(object_id - uni_object_id) > 0.001) {
        discard;
    }

    float depth = texture(uni_map_depth, texcoord).r;
    vec4 worldpos = texture(uni_map_worldpos, texcoord);

    worldpos.w = 1;

    vec4 localpos = worldpos * uni_inv_world;

    vec3 dist = 0.5 - abs(localpos).xyz;

    if ((depth < 1.0 && dist.x > 0 && dist.y > 0 && dist.z > 0))
    {
        fs_out_diffuse = vec4(uni_color, 1);
    }
    else
    {
        if(DEBUG_NO_DISCARD == 1)
        {
            fs_out_diffuse = vec4(1.0, 1.0, 0, 1);
        }
        else
        {
            discard;
        }
    }
}
