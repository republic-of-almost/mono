/*
  Dir Lit shader
  --
  Mainly for debugging, has a hardcoded directional light.
*/


// VERT_START //

#version 100

highp mat4 transpose(in highp mat4 inMatrix)
{
  highp vec4 i0 = inMatrix[0];
  highp vec4 i1 = inMatrix[1];
  highp vec4 i2 = inMatrix[2];
  highp vec4 i3 = inMatrix[3];

  highp mat4 outMatrix = mat4(
    vec4(i0.x, i1.x, i2.x, i3.x),
    vec4(i0.y, i1.y, i2.y, i3.y),
    vec4(i0.z, i1.z, i2.z, i3.z),
    vec4(i0.w, i1.w, i2.w, i3.w)
  );

  return outMatrix;
}


highp mat4 inverse(mat4 m)
{
  highp float
  a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],
  a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],
  a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],
  a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3],

  b00 = a00 * a11 - a01 * a10,
  b01 = a00 * a12 - a02 * a10,
  b02 = a00 * a13 - a03 * a10,
  b03 = a01 * a12 - a02 * a11,
  b04 = a01 * a13 - a03 * a11,
  b05 = a02 * a13 - a03 * a12,
  b06 = a20 * a31 - a21 * a30,
  b07 = a20 * a32 - a22 * a30,
  b08 = a20 * a33 - a23 * a30,
  b09 = a21 * a32 - a22 * a31,
  b10 = a21 * a33 - a23 * a31,
  b11 = a22 * a33 - a23 * a32,

  det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

  return mat4(
    a11 * b11 - a12 * b10 + a13 * b09,
    a02 * b10 - a01 * b11 - a03 * b09,
    a31 * b05 - a32 * b04 + a33 * b03,
    a22 * b04 - a21 * b05 - a23 * b03,
    a12 * b08 - a10 * b11 - a13 * b07,
    a00 * b11 - a02 * b08 + a03 * b07,
    a32 * b02 - a30 * b05 - a33 * b01,
    a20 * b05 - a22 * b02 + a23 * b01,
    a10 * b10 - a11 * b08 + a13 * b06,
    a01 * b08 - a00 * b10 - a03 * b06,
    a30 * b04 - a31 * b02 + a33 * b00,
    a21 * b02 - a20 * b04 - a23 * b00,
    a11 * b07 - a10 * b09 - a12 * b06,
    a00 * b09 - a01 * b07 + a02 * b06,
    a31 * b01 - a30 * b03 - a32 * b00,
    a20 * b03 - a21 * b01 + a22 * b00
  ) / det;
}


attribute mediump vec3 vs_in_position;
attribute mediump vec3 vs_in_normal;
attribute mediump vec2 vs_in_texture_coords;

uniform mediump mat4 uni_wvp;
uniform mediump mat4 uni_world;

varying mediump vec2 ps_in_texture_coords;
varying mediump vec3 ps_in_normal;
varying mediump vec3 ps_in_fragpos;

void
main()
{
  gl_Position = uni_wvp * vec4(vs_in_position, 1.0);
  ps_in_texture_coords = vs_in_texture_coords;
  ps_in_normal = normalize(mat3(transpose(inverse(uni_world))) * vs_in_normal);
  ps_in_fragpos = vec3(uni_world * vec4(vs_in_position, 1.0));
}

// VERT_END //

// FRAG_START //

#version 100

varying mediump vec2 ps_in_texture_coords;
varying mediump vec3 ps_in_normal;
varying mediump vec3 ps_in_fragpos;

uniform sampler2D uni_map_01;
uniform mediump vec3 uni_eye;
uniform mediump vec4 uni_color;

//      out vec4 ps_out_final_color;

void
main()
{
  // set the specular term to black
  highp vec4 spec = vec4(0.0);
  highp vec3 view_dir = normalize(uni_eye - ps_in_fragpos);
  highp vec3 l_dir = normalize(vec3(-0.35,1.0,-1.25));
  highp vec4 diffuse = texture2D(uni_map_01, ps_in_texture_coords);
  //        highp vec4 diffuse = vec4(uni_color);//
  highp vec4 ambient = vec4(0.1, 0.1, 0.1, 1);
  highp vec4 specular = vec4(0.1, 0.1, 0.1, 1);
  highp float shininess = 32.0;
  // normalize both input vectors
  highp vec3 n = normalize(ps_in_normal);
  highp vec3 e = normalize(view_dir);
  highp float intensity = max(dot(n,l_dir), 0.0);

  // if the vertex is lit compute the specular color
  if (intensity > 0.0)
  {
    // compute the half vector
    highp vec3 h = normalize(l_dir + e);
    // compute the specular term into spec
    highp float intSpec = max(dot(h,n), 0.0);
    spec = specular * pow(intSpec,shininess);
  }
  
  gl_FragColor = max(intensity *  diffuse + spec, ambient);
}


// FRAG_END //
