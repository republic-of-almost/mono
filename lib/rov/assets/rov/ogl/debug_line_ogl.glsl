/*
  Debug Line Shader
*/

// VERT_START //

#version 150 core

/*
  Output
*/
out int gs_in_vert_id;

/*
  Program
*/
void
main()
{
  gs_in_vert_id = gl_VertexID;
}

// VERT_END //

// GEO_START //

#version 330


#define NUM_LINES 32
#define COMPONENTS_PER_LINE 3


layout (points) in;
layout (line_strip, max_vertices = 2) out;

/*
  Inputs
*/
in int              gs_in_vert_id[];


/*
  Uniforms
*/
uniform mat4        uni_wvp_mat;
uniform vec3        uni_line[NUM_LINES * COMPONENTS_PER_LINE];


/*
  Outputs
*/
out vec3            ps_in_color;


/*
  Program
*/
void
main()
{
  int id = gs_in_vert_id[0] * COMPONENTS_PER_LINE;

  int start = id + 0;
  int end   = id + 1;
  int color = id + 2;

  // Generate the primitive //

  ps_in_color = uni_line[color];
  
  // Small offset to stop z-fight as often used with bounding boxes etc //
  float offset_scale = -0.002;

  // Start Vert //
  {
    vec4 pos = uni_wvp_mat * vec4(uni_line[start], 1);
    vec4 offset = vec4(0,0,offset_scale, 0);
    gl_Position = pos + offset;
    
    EmitVertex();
  }

  // End Vert //
  {
    vec4 pos = uni_wvp_mat * vec4(uni_line[end], 1);
    vec4 offset = vec4(0,0,offset_scale, 0);
    gl_Position = pos + offset;
    
    EmitVertex();
  }

  EndPrimitive();
}

// GEO_END //


// FRAG_START //

#version 150

/*
  Inputs
*/
in vec3 ps_in_color;


/*
  Outputs
*/
out vec4 ps_out_color;


/*
  Program
*/
void
main()
{
  ps_out_color = vec4(ps_in_color, 1.0);
}

// FRAG_END //
