/*
  Fullbright shader
  --
  Has no lighting information will just output the color or texture.
*/

// VERT_START //

#version 100

attribute mediump vec3 vs_in_position;
attribute mediump vec3 vs_in_normal;
attribute mediump vec2 vs_in_texture_coords;

uniform mediump mat4 uni_wvp;

varying mediump vec2 ps_in_texture_coords;
varying mediump vec3 ps_in_color;

void
main()
{
  gl_Position = uni_wvp * vec4(vs_in_position, 1.0);
  ps_in_texture_coords = vs_in_texture_coords;
  ps_in_color = vs_in_normal;
}

// VERT_END //

// FRAG_START //

#version 100

varying mediump vec2 ps_in_texture_coords;
varying mediump vec3 ps_in_color;

uniform mediump vec4 uni_color;

void
main()
{
  gl_FragColor = uni_color;
}

// FRAG_END //
