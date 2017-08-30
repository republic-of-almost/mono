#include "rov_gl_resources.hpp"
#include "rov_gl.hpp"
#include <lib/assert.hpp>
#include <lib/file.hpp>
#include <lib/string.hpp>
#include <lib/directory.hpp>
#include <lib/logging.hpp>


namespace ROV_Internal {


// -------------------------------------------------------------- [ Texture ] --


uint32_t
ogl_createTexture(
  rovGLData *gl_data,
  uint8_t *data,
  uint32_t width,
  uint32_t height,
  size_t size,
  uint32_t format,
  uintptr_t *out_resource_id)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  GLuint texture;
  glGenTextures(1, &texture);

  if(out_resource_id)
  {
    *out_resource_id = (uintptr_t)texture;
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    format_to_gl_internal(format),
    width,
    height,
    0,
    format_to_gl_format(format),
    format_to_gl_type(format),
    data
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  rovGLTexture tex;
  tex.gl_id  = texture;
  tex.width  = width;
  tex.height = height;

  gl_data->rov_textures.emplace_back(tex);

  return gl_data->rov_textures.size();
}


void
ogl_destroyTextures(rovGLData *gl_data)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  lib::array<GLuint, 64> textures_to_destroy;

  for (ROV_Internal::rovGLTexture tex : gl_data->rov_textures)
  {
    textures_to_destroy.emplace_back(tex.gl_id);
  }

  glDeleteTextures(textures_to_destroy.size(), textures_to_destroy.data());
}


// ----------------------------------------------------------------- [ Mesh ] --


uint32_t
ogl_createMesh(
  rovGLData *gl_data,
  const float *positions,
  const float *normals,
  const float *tex_coords,
  size_t count,
  uintptr_t *out_resource_id)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  constexpr size_t stride = 3 + 3 + 2;

  lib::array<GLfloat> vertices;
  vertices.resize(count * stride);

  for(size_t i = 0; i < count; ++i)
  {
    const size_t index = stride * i;

    vertices[index + 0] = positions[(i * 3) + 0];
    vertices[index + 1] = positions[(i * 3) + 1];
    vertices[index + 2] = positions[(i * 3) + 2];

    vertices[index + 3] = normals[(i * 3) + 0];
    vertices[index + 4] = normals[(i * 3) + 1];
    vertices[index + 5] = normals[(i * 3) + 2];

    vertices[index + 6] = tex_coords[(i * 2) + 0];
    vertices[index + 7] = tex_coords[(i * 2) + 1];
  }

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(GLfloat) * vertices.size(),
    vertices.data(),
    GL_STATIC_DRAW
  );

  const rovGLMesh rov_mesh{vbo, 8, count};
  gl_data->rov_meshes.emplace_back(rov_mesh);

  if(out_resource_id)
  {
    *out_resource_id = vbo;
  }

  return (uint32_t)gl_data->rov_meshes.size();
}


void
ogl_destroyMeshes(rovGLData *gl_data)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  lib::array<GLuint, 64> vbos_to_destroy;

  for (ROV_Internal::rovGLMesh mesh : gl_data->rov_meshes)
  {
    vbos_to_destroy.emplace_back(mesh.gl_id);
  }

  glDeleteVertexArrays(vbos_to_destroy.size(), vbos_to_destroy.data());
}


// ---------------------------------------------------------------- [ Index ] --


uint32_t
ogl_createIndex(
  rovGLData *gl_data,
  const uint32_t *index,
  const size_t count,
  uintptr_t *out_resource_id)
{
  #ifndef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif
  
  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    sizeof(GLuint) * count,
    index,
    GL_STATIC_DRAW
  );
  
  const rovGLIndex rov_index{ibo, count};
  gl_data->rov_indexes.emplace_back(rov_index);
  
  if(out_resource_id)
  {
    *out_resource_id = ibo;
  }
  
  return (uint32_t)gl_data->rov_indexes.size();
}

void
ogl_destroyIndexes(rovGLData *gl_data)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  lib::array<GLuint, 64> ibos_to_destroy;

  for (ROV_Internal::rovGLIndex index : gl_data->rov_indexes)
  {
    ibos_to_destroy.emplace_back(index.gl_id);
  }

  glDeleteVertexArrays(ibos_to_destroy.size(), ibos_to_destroy.data());
}


// ------------------------------------------------------------- [ Lighting ] --


uint32_t
ogl_createLights(
  rovGLData *gl_data,
  rovLight *lights,
  size_t count,
  uintptr_t *out_resource_id)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  GLuint light_buffer;

  glGenTextures(1, &light_buffer);
  glBindTexture(GL_TEXTURE_1D, light_buffer);

  glTexImage1D(
    GL_TEXTURE_1D,
    0,
    GL_RGBA32F,
    512,
    0,
    GL_RGBA,
    GL_FLOAT,
    0
  );

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexSubImage1D(
    GL_TEXTURE_1D,
    0,
    0,
    count * 3,
    GL_RGBA,
    GL_FLOAT,
    lights
  );

  gl_data->light_buffers.emplace_back(
    light_buffer,
    count
  );

  return gl_data->light_buffers.size();
}


bool
ogl_updateLights(
  rovGLData *gl_data,
  uint32_t id,
  rovLight *lights,
  size_t count)
{
  // -- Param Check -- //
  LIB_ASSERT(gl_data);
  LIB_ASSERT(id);

  rovGLLightPack &buffer = gl_data->light_buffers[id - 1];
  buffer.count = count;

  glBindTexture(GL_TEXTURE_1D, buffer.gl_id);

  glTexSubImage1D(
    GL_TEXTURE_1D,
    0,
    0,
    count * 3,
    GL_RGBA,
    GL_FLOAT,
    lights
  );

  return true;
}


void
ogl_destroyLights(rovGLData *gl_data)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  lib::array<GLuint, 64> buffer_to_destroy;

  for (ROV_Internal::rovGLLightPack buf : gl_data->light_buffers)
  {
    buffer_to_destroy.emplace_back(buf.gl_id);
  }

  glDeleteTextures(buffer_to_destroy.size(), buffer_to_destroy.data());
}


// ------------------------------------------------------------- [ Lighting ] --


uint32_t
ogl_createFramebuffer(
  rovGLData *gl_data,
  uint32_t width,
  uint32_t height,
  uint32_t format,
  uintptr_t *out_platform_resource)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  GLuint fbo;

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  GLuint depth;
  
  glGenRenderbuffers(1, &depth);
  glBindRenderbuffer(GL_RENDERBUFFER, depth);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

  GLuint tex;

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, width, height, true);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);

  GLuint resolve_fbo;

  glGenFramebuffers(1, &resolve_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, resolve_fbo);

  GLuint resolve_tex;

  glGenTextures(1, &resolve_tex);
  glBindTexture(GL_TEXTURE_2D, resolve_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, resolve_tex, 0);

  // check FBO status
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    LOG_ERROR("Failed to create FBO");
    return 0;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  rovGLFramebuffer rov_fbo { fbo, depth, tex, resolve_fbo, resolve_tex, width, height };
  gl_data->rov_frame_buffers.emplace_back(rov_fbo);

  if (out_platform_resource)
  {
    *out_platform_resource = resolve_tex;
  }

  const uint32_t id = (uint32_t)gl_data->rov_frame_buffers.size();

  return id;
}


// ------------------------------------------------------------- [ Programs ] --


bool
ogl_createProgram(
  const char *filename,
  GLuint *out_program)
{
  auto shd_compiled = [](const GLuint shd_id, const char *name) -> bool
  {
    GLint is_compiled = 0;

    glGetShaderiv(shd_id, GL_COMPILE_STATUS, &is_compiled);
    if(is_compiled == GL_FALSE)
    {
      GLint max_length = 0;
      glGetShaderiv(shd_id, GL_INFO_LOG_LENGTH, &max_length);

      // The maxLength includes the NULL character
      lib::array<GLchar, 1024> error_log;
      error_log.resize(max_length);

      glGetShaderInfoLog(shd_id, max_length, &max_length, &error_log[0]);

      // Provide the infolog in whatever manor you deem best.
      // Exit with failure.
      glDeleteShader(shd_id); // Don't leak the shader.

      printf("SHD ERR: %s %s\n", name, error_log.data());

      return false;
    }

    return true;
  };

  // -- Load File -- //

  char name[1024]{};
  lib::string::filename_from_path(filename, name);

  char path[2048]{};
  const char * exe_path = lib::dir::exe_path();
  strcat(path, exe_path);
  strcat(path, filename);

  char program[1 << 13]{};
  lib::file::get_contents(path, program, sizeof(program));

  if(strlen(program) == 0)
  {
    LOG_ERROR("Failed to load shader")
    return false;
  }

  char vert_src[1 << 13]{};
  lib::string::get_text_between_tags(
    "// VERT_START //",
    "// VERT_END //",
    program,
    vert_src,
    sizeof(vert_src)
  );

  char geo_src[1 << 13]{};
  lib::string::get_text_between_tags(
    "// GEO_START //",
    "// GEO_END //",
    program,
    geo_src,
    sizeof(geo_src)
  );

  char frag_src[1 << 13]{};
  lib::string::get_text_between_tags(
    "// FRAG_START //",
    "// FRAG_END //",
    program,
    frag_src,
    sizeof(frag_src)
  );

  // -- Create Shader Stages -- //

  const GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
  const char *vs_src = &vert_src[0];
  glShaderSource(vert_shd, 1, &vs_src, NULL);
  glCompileShader(vert_shd);

  if(!shd_compiled(vert_shd, name))
  {
    LIB_ASSERT(false);
    return false;
  }

  #ifdef GL_HAS_GEO_SHD
  GLuint geo_shd = 0;
  if(strlen(geo_src))
  {
    geo_shd = glCreateShader(GL_GEOMETRY_SHADER);
    const char *gs_src = &geo_src[0];
    glShaderSource(geo_shd, 1, &gs_src, NULL);
    glCompileShader(geo_shd);

    if(!shd_compiled(geo_shd, name))
    {
      LIB_ASSERT(false);
      return false;
    }
  }
  #endif

  const GLuint frag_shd = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fs_src = &frag_src[0];
  glShaderSource(frag_shd, 1, &fs_src, NULL);
  glCompileShader(frag_shd);
  if(!shd_compiled(frag_shd, name))
  {
    LIB_ASSERT(false);
    return false;
  }

    {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }

  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert_shd);

  if(strlen(geo_src))
  {
    glAttachShader(prog, geo_shd);
  }

    {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }

  glAttachShader(prog, frag_shd);
  glLinkProgram(prog);

  GLint is_linked = 0;

  glGetProgramiv(prog, GL_LINK_STATUS, &is_linked);

  if(is_linked == GL_FALSE)
  {
    GLint max_length = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &max_length);

    // The maxLength includes the NULL character
    lib::array<GLchar, 1024> error_log;
    error_log.resize(max_length);

    glGetProgramInfoLog(prog, max_length, &max_length, &error_log[0]);

    // Provide the infolog in whatever manor you deem best.
    // Exit with failure.

    printf("PROG ERR: %s\n%s\n", name, error_log.data());

    return false;
  }

  // -- Detach and Delete Shaders -- 
  
  if (vert_shd) { glDetachShader(prog, vert_shd); }
  if (geo_shd)  { glDetachShader(prog, geo_shd);  }
  if (frag_shd) { glDetachShader(prog, frag_shd); }

  if (vert_shd) { glDeleteShader(vert_shd); }
  if (geo_shd) { glDeleteShader(geo_shd); }
  if (frag_shd) { glDeleteShader(frag_shd); }

  // -- Return Values -- //

  if(out_program) { *out_program = prog; }

  return true;
}


void
ogl_destroyPrograms(rovGLData *gl_data)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif

  glUseProgram(0);

  for (ROV_Internal::rovGLMeshProgram &prog : gl_data->rov_mesh_programs)
  {
    GLsizei count = 0;
    GLuint shaders[3]{};
    glGetAttachedShaders(prog.program, 3, &count, shaders);

    for (GLuint sh : shaders)
    {
      LIB_ASSERT(sh == 0);
    }

    glDeleteProgram(prog.program);
  }

  for (ROV_Internal::rovGLLineProgram &prog : gl_data->rov_line_programs)
  {
    GLsizei count = 0;
    GLuint shaders[3]{};
    glGetAttachedShaders(prog.program, 3, &count, shaders);

    for (GLuint sh : shaders)
    {
      LIB_ASSERT(sh == 0);
    }

    glDeleteProgram(prog.program);
  }
}

} // ns
