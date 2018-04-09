#include <roa_ctx/roa_ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_math/math.h>
#include <volt/volt.h>
#include <roa_lib/time.h>
#include <GL/gl3w.h>
#include <scratch/geometry.h>


/* ---------------------------------------------------------- [ Systems ] -- */


roa_ctx_t                 hw_ctx;     /* windowing */
volt_ctx_t                volt_ctx;   /* graphics api */


/* ------------------------------------------------------------ [ Scene ] -- */


volt_program_t    screen_program       = VOLT_NULL;
volt_texture_t    screen_texture       = VOLT_NULL;
volt_vbo_t        screen_triangle      = VOLT_NULL;
volt_input_t      screen_input_format  = VOLT_NULL;
volt_rasterizer_t screen_rasterizer    = VOLT_NULL;

int width = 1200;
int height = 720;

unsigned char *texture_data = ROA_NULL;

/* --------------------------------------------------------- [ GL Stuff ] -- */

int WindowWidth = 1200;
int WindowHeight = 720;


#define GBUFFER_POSITION_TEXTURE_UNIT 0
#define GBUFFER_DIFFUSE_TEXTURE_UNIT  1
#define GBUFFER_NORMAL_TEXTURE_UNIT   2
#define GBUFFER_TEXCOORD_TEXTURE_UNIT 3


enum GBUFFER_TEXTURE_TYPE {
  GBUFFER_TEXTURE_TYPE_POSITION,
  GBUFFER_TEXTURE_TYPE_DIFFUSE,
  GBUFFER_TEXTURE_TYPE_NORMAL,
  GBUFFER_TEXTURE_TYPE_TEXCOORD,
  GBUFFER_NUM_TEXTURES
};

const char *texture_names[] = {
  "gbuffer - position",
  "gbuffer - diffuse",
  "gbuffer - normal",
  "gbuffer - texcoord",
};


GLuint m_fbo, m_depthTexture;
GLuint m_textures[GBUFFER_NUM_TEXTURES];


GLuint m_WVPLocation;
GLuint m_WorldMatrixLocation;
GLuint m_colorTextureUnitLocation;

GLuint m_shaderProg;

GLuint vbo;
GLuint vert_count;
roa_float3 box_positions[5];

roa_float3 cam_position;
float cam_pitch;
float cam_roll;


/* ------------------------------------------------------- [ Applicaton ] -- */


ROA_BOOL
setup_shader()
{
  m_shaderProg = glCreateProgram();

  GLuint vs_id, fs_id;

  {
    const char vs[] = ""
      "#version 330\n"

      "layout(location = 0) in vec3 Position;\n"
      "layout (location = 1) in vec2 TexCoord; \n"
      "layout(location = 2) in vec3 Normal;\n"

      "uniform mat4 gWVP;\n"
      "uniform mat4 gWorld;\n"

      "out vec2 TexCoord0;\n"
      "out vec3 Normal0;\n"
      "out vec3 WorldPos0;\n"

      "void main()\n"
      "{\n"
        "gl_Position = gWVP * vec4(Position, 1.0);\n"
        "TexCoord0 = TexCoord;\n"
        "Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;\n"
        "WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;\n"
      "}\n";

      const GLchar *vs_src = vs;

      vs_id = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vs_id, 1, &vs_src, ROA_NULL);
      glCompileShader(vs_id);

      /* check for errors */
      GLint status = 0;
      glGetShaderiv(vs_id, GL_COMPILE_STATUS, &status);

      if (status == GL_FALSE)
      {
        GLchar error[1024] = {0};
        GLsizei length = 0;
        glGetShaderInfoLog(
          vs_id,
          ROA_ARR_COUNT(error),
          &length,
          ROA_ARR_DATA(error));

        ROA_ASSERT(0);
      }

      glAttachShader(m_shaderProg, vs_id);
  }

    {
      const char fs[] = ""
        "#version 330\n"

        "in vec2 TexCoord0;   \n"
        "in vec3 Normal0;   \n"
        "in vec3 WorldPos0;  \n"

        "layout (location = 0) out vec3 WorldPosOut;   \n"
        "layout (location = 1) out vec3 DiffuseOut;  \n"
        "layout (location = 2) out vec3 NormalOut;   \n"
        "layout (location = 3) out vec3 TexCoordOut; \n"

        "uniform sampler2D gColorMap;   \n"

        "void main()  \n"
        "{    \n"
        "WorldPosOut     = WorldPos0;    \n"
        "DiffuseOut      = vec3(1, 0, 1); /*texture(gColorMap, TexCoord0).xyz;*/\n"
        "NormalOut       = normalize(Normal0);    \n"
        "TexCoordOut     = vec3(TexCoord0, 0.0);  \n"
        "}\n";

      const GLchar *fs_src = fs;

      vs_id = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(vs_id, 1, &fs_src, ROA_NULL);
      glCompileShader(vs_id);

      /* check for errors */
      GLint status = 0;
      glGetShaderiv(vs_id, GL_COMPILE_STATUS, &status);

      if (status == GL_FALSE)
      {
        GLchar error[1024] = { 0 };
        GLsizei length = 0;
        glGetShaderInfoLog(
          vs_id,
          ROA_ARR_COUNT(error),
          &length,
          ROA_ARR_DATA(error));

        ROA_ASSERT(0);
      }

      glAttachShader(m_shaderProg, vs_id);
    }

    {
      glLinkProgram(m_shaderProg);

      GLint success;
      GLchar ErrorLog[1024] = { 0 };

      glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);

      if (success == 0) {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        ROA_ASSERT(0);
      }

      glValidateProgram(m_shaderProg);
      glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);

      if (!success) {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        ROA_ASSERT(0);
      }

      m_WVPLocation = glGetUniformLocation(m_shaderProg, "gWVP");
      m_WorldMatrixLocation = glGetUniformLocation(m_shaderProg, "gWorld");
      m_colorTextureUnitLocation = glGetUniformLocation(m_shaderProg, "gColorMap");
    }
}


ROA_BOOL
setup_gbuffer()
{
  /* Create the FBO */
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  /* Create the gbuffer textures */
	glGenTextures(ROA_ARR_COUNT(m_textures), m_textures);
  glGenTextures(1, &m_depthTexture);

	unsigned i;

  for (i = 0; i < ROA_ARR_COUNT(m_textures); i++) {
    glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);

    glObjectLabel(GL_TEXTURE, m_textures[i], -1, texture_names[i]);
  }

  /* depth */
  glBindTexture(GL_TEXTURE_2D, m_depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

  GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3 };

  glDrawBuffers(ROA_ARR_COUNT(DrawBuffers), DrawBuffers);

  GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (Status != GL_FRAMEBUFFER_COMPLETE) {
    printf("FB error, status: 0x%x\n", Status);
    return ROA_FALSE;
  }

  /* restore default FBO */
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  return ROA_TRUE;
}


void
setup_geom()
{
  geom_vert_desc desc[3];
  desc[0] = GEOM_VERT_POSITION;
  desc[1] = GEOM_UV;
  desc[2] = GEOM_NORMAL;

  geometry_generate_cube(desc, 3, 1, 1, 1, ROA_NULL, &vert_count);

  float *data = malloc(sizeof(float) * vert_count);

  geometry_generate_cube(desc, 3, 1, 1, 1, data, &vert_count);

  /* create vbo */
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert_count, data, GL_STATIC_DRAW);

  glObjectLabel(GL_BUFFER, vbo, -1, "cube");

  ROA_ASSERT_PEDANTIC(vbo > 0);
}


int
main(int argc, char **argv)
{
  /* command line args */
  ROA_UNUSED(argc);
  ROA_UNUSED(argv);

  /* create systems */
  {
    struct roa_ctx_window_desc win_desc;
    win_desc.width = width;
    win_desc.height = height;
    win_desc.title = "Decals";

    roa_ctx_create(&hw_ctx);
    roa_ctx_set_window_desc(hw_ctx, &win_desc);

    volt_ctx_create(&volt_ctx);
  }

  /* setup volt */
  {
    setup_gbuffer();
    setup_shader();
    setup_geom();

    box_positions[0] = roa_float3_set_with_values(0.f, 0.f, 5.f);
    box_positions[1] = roa_float3_set_with_values(6.f, 1.f, 10.f);
    box_positions[2] = roa_float3_set_with_values(-5.f, -1.f, 12.f);
    box_positions[3] = roa_float3_set_with_values(4.f, 4.f, 15.f);
    box_positions[4] = roa_float3_set_with_values(-4.f, 2.f, 20.f);

    /* program */
    {
      volt_shader_stage stages[2];
      stages[0] = VOLT_SHD_VERTEX;
      stages[1] = VOLT_SHD_FRAGMENT;

      const char *vert_shd =
        "#version 150 core\n"
        "in vec2 vs_in_pos;\n"
        "in vec2 vs_in_tex_c;\n"
        "out vec2 ps_in_tex_c;\n"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(vs_in_pos, 0.0, 1.0);\n"
        " ps_in_tex_c = vs_in_tex_c;\n"
        "}\n";

      const char *frag_shd =
        "#version 150 core\n"
        "in vec2 ps_in_tex_c;\n"
        "out vec4 out_color;\n"
        "uniform sampler2D diffuse;\n"
        "void main()\n"
        "{\n"
        " out_color = texture(diffuse, ps_in_tex_c);\n"
        "}\n";

      const char *shaders[2];
      shaders[0] = vert_shd;
      shaders[1] = frag_shd;
      
      struct volt_program_desc screen_program_desc;
      screen_program_desc.stage_count         = ROA_ARR_COUNT(stages);
      screen_program_desc.shader_stages_type  = ROA_ARR_DATA(stages);
      screen_program_desc.shader_stages_src   = ROA_ARR_DATA(shaders);

      volt_program_create(volt_ctx, &screen_program, &screen_program_desc);

      ROA_ASSERT(screen_program != VOLT_NULL);
			volt_ctx_execute(volt_ctx); /* because shader text will go out of scope */
    }

    /* texture */
    {
      texture_data = roa_alloc(width * height * 4);
      unsigned i;

      /* fill with random color for now */
      for (i = 0; i < width * height; ++i)
      {
        unsigned index = i * 4;
        texture_data[index + 0] = rand() % 255;
        texture_data[index + 1] = rand() % 255;
        texture_data[index + 2] = rand() % 255;
        texture_data[index + 3] = 255;
      }

      int tex_width = width;
      int tex_height = height;

      struct volt_texture_desc tex_desc;
      tex_desc.data       = ROA_ARR_DATA(texture_data);
      tex_desc.dimentions = VOLT_TEXTURE_2D;
      tex_desc.width      = tex_width;
      tex_desc.height     = tex_height;
      tex_desc.sampling   = VOLT_SAMPLING_BILINEAR;
      tex_desc.mip_maps   = VOLT_FALSE;
      tex_desc.format     = VOLT_COLOR_RGBA;
      tex_desc.access     = VOLT_STREAM;

      volt_texture_create(volt_ctx, &screen_texture, &tex_desc);

      ROA_ASSERT(screen_texture != VOLT_NULL);
    }

    /* triangle */
    {
      float verts[] = {
        /* x y, s t */
        +3.f, +1.f, 2.f, 0.f,
        -1.f, +1.f, 0.f, 0.f,
        -1.f, -3.f, 0.f, 2.f,
      };

      struct volt_vbo_desc vbo_desc;
      vbo_desc.data  = ROA_ARR_DATA(verts);
      vbo_desc.count = ROA_ARR_COUNT(verts);

      volt_vertex_buffer_create(volt_ctx, &screen_triangle, &vbo_desc);

      ROA_ASSERT(screen_triangle != VOLT_NULL);
			volt_ctx_execute(volt_ctx);
    }

    /* input */
    {
      volt_input_attribute attrs[2];
      attrs[0] = VOLT_INPUT_FLOAT2;
      attrs[1] = VOLT_INPUT_FLOAT2;

      struct volt_input_desc input_desc;
      input_desc.attributes = ROA_ARR_DATA(attrs);
      input_desc.count      = ROA_ARR_COUNT(attrs);

      /*volt_input_t   input_format;*/
      volt_input_create(volt_ctx, &screen_input_format, &input_desc);

      ROA_ASSERT(screen_input_format != VOLT_NULL);
			volt_ctx_execute(volt_ctx); /* attrs will go out of scope */
    }

    /* rasterizer */
    {
      struct volt_rasterizer_desc raster_desc;
      raster_desc.cull_mode = VOLT_CULL_FRONT;
      raster_desc.primitive_type = VOLT_PRIM_TRIANGLES;
      raster_desc.winding_order = VOLT_WIND_CW;

      volt_rasterizer_create(volt_ctx, &screen_rasterizer, &raster_desc);
    }

    volt_ctx_execute(volt_ctx);
  }

  /* app loop */
  while (roa_ctx_new_frame(hw_ctx))
  {
    /* geo pass */
    {
      glUseProgram(m_shaderProg);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

      glDepthMask(GL_TRUE);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);

      /* cam */
      static float time = 0.1f;
      time += 0.03f;
      float radius = 2.5f;

      float x = roa_float_sin(time) * radius;
      float y = 2.f;
      float z = roa_float_cos(time) * radius;

      roa_float3 up = roa_float3_set_with_values(0, 1, 0);
      roa_float3 pos = roa_float3_set_with_values(x, y, z);
      roa_float3 at = roa_float3_zero();

      roa_mat4 view_mat;
      roa_mat4_lookat(&view_mat, pos, at, up);

      roa_mat4 proj_mat;
      roa_mat4_projection(&proj_mat, ROA_TAU * 0.125f, 0.1, 100, (float)width / (float)height);

      glBindBuffer(GL_ARRAY_BUFFER, vbo);

      GLint posAttrib = glGetAttribLocation(m_shaderProg, "Position");
      glEnableVertexAttribArray(posAttrib);
      glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

      GLint texAttrib = glGetAttribLocation(m_shaderProg, "TexCoord");
      glEnableVertexAttribArray(texAttrib);
      glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

      GLint colAttrib = glGetAttribLocation(m_shaderProg, "Normal");
      glEnableVertexAttribArray(colAttrib);
      glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

      int i;
      for(i = 0; i < ROA_ARR_COUNT(box_positions); ++i)
      {
        roa_transform world_trans;
        world_trans.scale = roa_float3_fill_with_value(1.f);
        world_trans.position = box_positions[i];
        world_trans.rotation = roa_quaternion_default();

        roa_mat4 world_mat;
        roa_transform_to_mat4(&world_trans, &world_mat);

        roa_mat4 wvp_mat;
        roa_mat4_multiply_three(&wvp_mat, &world_mat, &view_mat, &proj_mat);

        glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, (GLfloat*)world_mat.data);
        glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, (GLfloat*)wvp_mat.data);

        glDrawArrays(GL_TRIANGLES, 0, vert_count / 8);
      }

      glDepthMask(GL_FALSE);
      glDisable(GL_DEPTH_TEST);
    }

    /* begin light pass */
    {
      glEnable(GL_BLEND);
      glBlendEquation(GL_FUNC_ADD);
      glBlendFunc(GL_ONE, GL_ONE);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      glClear(GL_COLOR_BUFFER_BIT);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
      
    }

    /* light pass */
    {
      GLint HalfWidth = (GLint)(width / 2.0f);
      GLint HalfHeight = (GLint)(height / 2.0f);

      glReadBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_TEXTURE_TYPE_POSITION);
      glBlitFramebuffer(0, 0, width, height, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

      glReadBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_TEXTURE_TYPE_DIFFUSE);
      glBlitFramebuffer(0, 0, width, height, 0, HalfHeight, HalfWidth, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

      glReadBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_TEXTURE_TYPE_NORMAL);
      glBlitFramebuffer(0, 0, width, height, HalfWidth, HalfHeight, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

      glReadBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_TEXTURE_TYPE_TEXCOORD);
      glBlitFramebuffer(0, 0, width, height, HalfWidth, 0, width, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }

    /* volt_ctx_execute(volt_ctx); */
  }

  /* destroy systems */
  {
    volt_ctx_destroy(&volt_ctx);
    roa_ctx_destroy(&hw_ctx);
  }

  return 0;
}
