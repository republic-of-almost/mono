#include <nau/renderers/opengl3.h>
#include <GL/gl3w.h>


// ----------------------------------------------------------------- [ Data ] --


namespace {


struct nau_gl3_renderer
{
  GLuint vao;
  GLuint vs_shader;
  GLuint fs_shader;
  GLuint program;
  GLuint vbo;
};


inline nau_gl3_renderer&
get_nau_gl3()
{
  static nau_gl3_renderer gl3;
  return gl3;
};


const GLchar* vs_src = R"GLSL(
  #version 150 core
  in vec2 position;
//  in vec4 vs_in_color;
//  in vec2 vs_in_tex_coord;
  
//  uniform vec3 uni_viewport;
  
//  out vec4 fs_in_color;
//  out vec2 fs_in_tex_coord;

  void
  main()
  {
    vec2 foo = vec2(800, 600);
  
    gl_Position    = vec4(position / foo, 0.0, 1.0);
//    gl_Position = vec4(position, 0.0, 1.0);
//    fs_in_color     = vs_in_color;
//    fs_in_tex_coord = vs_in_tex_coord;

  }

)GLSL";


const GLchar* fs_src = R"GLSL(
  #version 150 core
//  in vec4 fs_in_color;
//  in vec2 fs_in_tex_coord;

  out vec4 fs_out_color;
  
  void
  main()
  {
    return vec4(1,1,1,1);
  }

)GLSL";


const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    out vec4 outColor;
    void main()
    {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)glsl";


} // ns



// ------------------------------------------------------------- [ Lifetime ] --


void
nau_gl3_init()
{
  // Load shader
  glGenVertexArrays(1, &get_nau_gl3().vao);
  glBindVertexArray(get_nau_gl3().vao);

  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &get_nau_gl3().vbo);

  GLfloat vertices[]
  {
    -100.5f, -100.5f,
     100.5f, -100.5f,
    -100.5f,  100.5f,
    
     100.5f, -100.5f,
     100.5f,  100.5f,
    -100.5f,  100.5f,
  };

  glBindBuffer(GL_ARRAY_BUFFER, get_nau_gl3().vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  get_nau_gl3().vs_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(get_nau_gl3().vs_shader, 1, &vs_src, NULL);
  glCompileShader(get_nau_gl3().vs_shader);

  // Create and compile the fragment shader
  get_nau_gl3().fs_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(get_nau_gl3().fs_shader, 1, &fragmentSource, NULL);
  glCompileShader(get_nau_gl3().fs_shader);
  
  // Link the vertex and fragment shader into a shader program
  get_nau_gl3().program = glCreateProgram();
  glAttachShader(get_nau_gl3().program, get_nau_gl3().vs_shader);
  glAttachShader(get_nau_gl3().program, get_nau_gl3().fs_shader);
  glBindFragDataLocation(get_nau_gl3().program, 0, "outColor");
  glLinkProgram(get_nau_gl3().program);
  glUseProgram(get_nau_gl3().program);
  
  // Specify the layout of the vertex data
  GLint pos = glGetAttribLocation(get_nau_gl3().program, "position");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void
nau_gl3_render(Nau_draw_cmd *renderables, int count)
{
  glClear(GL_DEPTH_BUFFER_BIT);
  
  // Render
  glBindVertexArray(get_nau_gl3().vao);
  glUseProgram(get_nau_gl3().program);
  
  glBindFragDataLocation(get_nau_gl3().program, 0, "outColor");
  glBindBuffer(GL_ARRAY_BUFFER, get_nau_gl3().vbo);
  
  for(size_t i = 0; i < count; ++i)
  {
    
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}


void
nau_gl3_destroy()
{
  glDeleteProgram(get_nau_gl3().program);
  glDeleteShader(get_nau_gl3().fs_shader);
  glDeleteShader(get_nau_gl3().vs_shader);

  glDeleteBuffers(1, &get_nau_gl3().vbo);

  glDeleteVertexArrays(1, &get_nau_gl3().vao);
}
