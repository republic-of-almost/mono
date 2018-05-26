/*
    Decal Frag Shader
    --
    Trims off projector fragments that do not intersect with underlying
    geometry.
*/

/* ----------------------------------------------------------- [ config ] -- */

#version 410
//"#extension GL_ARB_texture_rectangle : enable

#define DEBUG_NO_DISCARD 1

/* ----------------------------------------------------------- [ inputs ] -- */

in vec4 fs_position0;
in vec4 fs_position_w0;
in vec2 fs_texcoord0;

/* --------------------------------------------------------- [ uniforms ] -- */

uniform mat4 uni_world;
uniform sampler2D uni_map_diffuse;
uniform sampler2D uni_map_depth;
uniform sampler2D uni_map_worldpos;
uniform mat4 uni_inv_proj_view;
uniform mat4 uni_inv_world;

/* ---------------------------------------------------------- [ outputs ] -- */

layout ( location = 0 ) out vec4 fs_out_diffuse;
//layout ( location = 2 ) out vec4 fs_out_specular;
//layout ( location = 3 ) out vec4 fs_out_glow;

/* ---------------------------------------------------------- [ helpers ] -- */

vec4
reconstruct_pos(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = uni_inv_proj_view * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}
                 
/* ---------------------------------------------------------- [ program ] -- */

void
main()
{
    vec2 screenPosition = fs_position0.xy / fs_position0.w;
 
    vec2 depthUV = screenPosition * 0.5f + 0.5f;
    //depthUV += vec2(0.5f / 1280.0f, 0.5f / 720.0f); //half pixel offset
    float depth = texture(uni_map_depth, depthUV).r;
          
    vec4 worldPos = texture(uni_map_worldpos, depthUV);
    //vec4 worldPos = reconstruct_pos(depth, depthUV);
    //worldPos.w = 1;
    vec4 localPos = worldPos * uni_inv_world;
     
    float dist = 0.5f - abs(localPos.y);
    float dist2 = 0.5f - abs(localPos.x);
    float dist3 = 0.5f - abs(localPos.z);
    
    /* discard projector fragments that are not intersecting */
    if ((depth < 1.0 && dist > 0 && dist2 > 0 && dist3 > 0))
    {
        vec2 uv = vec2(localPos.x, localPos.y) + 0.5f;
        vec4 diffuseColor = texture(uni_map_diffuse, uv);
        fs_out_diffuse = diffuseColor;
        fs_out_diffuse = vec4(0.0, 1.0, 0.0, 1);
    }
    else
    {
        if(DEBUG_NO_DISCARD == 1)
        {
            fs_out_diffuse = vec4(1.0, 1.0, 0.0, 1);
        }
        else
        {
            discard;
        }
    }
}
