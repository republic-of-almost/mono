
/*
  Mesh Renderer Shader
  --
  
  TODO
  - Remove normal matrix calculation from shader to CPU.
  - Branches are left in until we add spot (or obvious per issue).
  - Material shineness is hardcoded here, do we have to?
  - Material constant amb? Maybe we could leave this up to directional light.
*/


// VERT_START //
// -------------------------------------------------------- [ Vertex Shader ] --
#version 330 core


/*
  Inputs
*/
in vec3 vs_in_position;
in vec3 vs_in_normal;
in vec2 vs_in_texture_coords;


/*
  Uniforms
*/
uniform mat4 uni_world;
uniform mat4 uni_view;
uniform mat4 uni_wvp;
uniform mat4 uni_normal;
uniform mat4 uni_wv;


/*
  Outputs
*/
out vec2 in_ps_texcoord;
out vec3 in_ps_normal;
out vec3 in_ps_world_pos;


// --------------------------------------------------------- [ Vert Program ] --


void
main()
{
  in_ps_texcoord  = vs_in_texture_coords;
  
  /*
    I'm not doing something correct with normal matrix.
  */
  
  in_ps_normal    = normalize(transpose(mat3(inverse(uni_world))) * vs_in_normal);
//  in_ps_normal    = normalize(mat3(uni_normal) * vs_in_normal);

//  mat4 nor_mat = transpose(inverse(uni_wv));
//  in_ps_normal = normalize(nor_mat * vec4(vs_in_normal, 0.0)).xyz;

  in_ps_world_pos = vec3(uni_world * vec4(vs_in_position, 1.0)).xyz;

  gl_Position    = uni_wvp * vec4(vs_in_position, 1.0);
}
// VERT_END //


// FRAG_START //
// ------------------------------------------------------ [ Fragment Shader ] --

#version 330 core


/*
  Defines
*/
#define CONST_AMB 0.02
#define CONST_SHININESS 12.3

#define ShaderMode_Lighting    1
#define ShaderMode_Fullbright  2
#define ShaderMode_Depth       3
#define ShaderMode_Grey        4


/*
  Inputs
*/
in vec2 in_ps_texcoord;
in vec3 in_ps_normal;
in vec3 in_ps_world_pos;

in vec3 in_ps_world_test;


/*
  Uniforms
*/
uniform sampler2D uni_map_01; // Diffuse.
uniform sampler2D uni_map_03; // Specular

uniform vec3      uni_eye;
uniform int       uni_light_count;
uniform sampler1D uni_light_array;

uniform vec4      uni_color;

uniform int       uni_mode = ShaderMode_Fullbright;


/*
  Outputs
*/
out vec4 out_ps_color;


// ---------------------------------------------------------- [ Attenuation ] --


struct Attenuation
{
  float constant;
  float linear;
  float exponential;
};


/*
  Calulates the light drop off.
*/
float
calculate_attenuation(
  vec3 light_pos,
  vec3 frag_pos,
  Attenuation atten)
{
  float distance        = length(light_pos - frag_pos);
  float atten_lin_dist  = atten.linear * distance;
  float atten_exp_dist  = atten.exponential * (distance * distance);
  float attenuation     = 1.0 / (atten.constant + atten_lin_dist + atten_exp_dist);

//  float attenuation = 1.0 / (1.0 + atten.linear * pow(distance, 2));

  return attenuation;
}


// ------------------------------------------------------------- [ Lighting ] --


struct Light
{
	vec3 position;
	vec3 La;          // Ambient intensity
	vec3 Ld;          // Diffuse intensity
	vec3 Ls;          // Specular intensity
  vec3 color;       // Light color
};


struct Material
{
	vec3 Ka;          // Ambient reflect
	vec3 Kd;          // Diffuse reflect
	vec3 Ks;          // Specular reflect
	float shininess;	// Specular shininess factor
};


vec3
calculate_light(Light light, Material mat, vec3 L, vec3 V, vec3 N)
{
  // Amb //
  vec3 amb = mat.Ka * light.La;
  
  // Diffuse //
  float diffuse_factor = clamp(dot(N, L), 0, 1);
  vec3 diffuse = mat.Kd * light.Ld * diffuse_factor;
  
  // Specular //
  float specular_factor = 0;
  float n_dot_l = dot(N, L);
  
  if(n_dot_l > 0)
  {
    vec3 H = normalize(L + V);
    float n_dot_h = max(dot(N, H), 0.0);
    specular_factor = pow(n_dot_h, mat.shininess);
  }
  
  vec3 specular = mat.Ks * light.Ls * specular_factor;
  
  // Combine //
  // Is this the correct way to add color?
//  return (amb + diffuse + specular) * light.color;
  return (
  amb + (n_dot_l * diffuse) + specular) * light.color;
//  return specular * light.color;
}


// --------------------------------------------------------- [ Frag Program ] --


void
main()
{
  /*
    Common to all modes
  */
  vec4 diffuse_map = texture(uni_map_01, in_ps_texcoord);
  vec3 color;
  float alpha = 1.0;

  /*
    If we have lights calculate color with them.
  */
  if(uni_mode == ShaderMode_Lighting)
  {
    vec4 diffuse_color = mix(uni_color, diffuse_map, diffuse_map.a);
    vec4 specular_map = texture(uni_map_03, in_ps_texcoord);

    Material mat;
    mat.Ka = vec3(CONST_AMB);
    mat.Kd = diffuse_color.rgb;
    mat.Ks = vec3(0.5);//specular_map.rgb;
    mat.shininess = CONST_SHININESS;

    vec3 accum_color = vec3(0,0,0);

    /*
      For all the lights, we should tile this if we can.
    */
    for(int i = 0; i < uni_light_count; ++i)
    {
      // Data
      int index = i * 3;

      vec4 pos_data   = texelFetch(uni_light_array, index + 0, 0);
      vec4 color_data = texelFetch(uni_light_array, index + 1, 0);
      vec4 atten_data = texelFetch(uni_light_array, index + 2, 0);
      
      // Light
      Light light;
      light.position = pos_data.xyz;

      // We just pass the color here
      light.La = color_data.rgb;
      light.Ld = color_data.rgb;
      light.Ls = color_data.rgb;
      light.color = color_data.rgb;

      vec3 L = normalize(light.position - in_ps_world_pos);
      vec3 V = normalize(uni_eye - in_ps_world_pos);
      vec3 N = normalize(in_ps_normal);
      
      // Is directional?
      if(pos_data.w > 0.f)
      {
        L = pos_data.xyz;
      }
      
      vec3 final_light  = calculate_light(light, mat, L, V, N);
      
      // Attenuation
      Attenuation atten;
      atten.constant    = atten_data.x;
      atten.linear      = atten_data.y;
      atten.exponential = atten_data.z;
      
      float final_atten = calculate_attenuation(light.position, in_ps_world_pos, atten);
      
      // Is directional?
      if(pos_data.w > 0.f)
      {
        final_atten = 1.0;
      }
    
      // Final
      accum_color += (final_light * final_atten);
    }

    color = mat.Kd * accum_color;

    // Gamma correction.
    vec3 gamma = vec3(1.0/2.2);
    vec3 final_color = pow(color, gamma);

    // Output Result //
    color = final_color;
  }
  
  /*
    Fullbright
  */
  else if(uni_mode == ShaderMode_Fullbright)
  {
    vec4 diffuse_map   = texture(uni_map_01, in_ps_texcoord);
    vec4 diffuse_color = mix(uni_color, diffuse_map, diffuse_map.a);
    
    color = diffuse_color.rgb;
  }
  
  /*
    Depth mode
  */
  else if(uni_mode == ShaderMode_Depth)
  {
    float factor = 5.0; // so we can see it better
    color = vec3(gl_FragCoord.w * factor);
  }
  
  /*
    Grey mode
  */
  else if(uni_mode == ShaderMode_Grey)
  {
    color = vec3(0.25);
    alpha = 1;
  }
  
  /*
    Err
  */
  else
  {
    color = vec3(1.0, 0.0, 1.0);
  }
  
  out_ps_color = vec4(color.rgb, alpha);
}
// FRAG_END //
