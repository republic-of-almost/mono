#ifndef OGL_VOLT_CONVERT_INCLUDED_
#define OGL_VOLT_CONVERT_INCLUDED_


#include <roa_lib/assert.h>
#include <volt/volt.h>
#include <GL/gl3w.h>


/* -------------------------------------------------------- [ gl to volt ] -- */


GLenum
convert_volt_attribute_to_gl_type(volt_input_attribute attr)
{
  switch (attr)
  {
  case(VOLT_INPUT_FLOAT4): return GL_FLOAT;
  case(VOLT_INPUT_FLOAT3): return GL_FLOAT;
  case(VOLT_INPUT_FLOAT2): return GL_FLOAT;
  case(VOLT_INPUT_FLOAT):  return GL_FLOAT;
  }

  ROA_ASSERT(false);
  return GL_NONE;
}


GLboolean
convert_volt_attribute_to_gl_normalized(volt_input_attribute attr)
{
  switch (attr)
  {
  case(VOLT_INPUT_FLOAT4): return GL_FALSE;
  case(VOLT_INPUT_FLOAT3): return GL_FALSE;
  case(VOLT_INPUT_FLOAT2): return GL_FALSE;
  case(VOLT_INPUT_FLOAT):  return GL_FALSE;
  }

  ROA_ASSERT(false);
  return GL_FALSE;
}


GLboolean
convert_volt_boolean_to_gl(volt_bool boolean)
{
  switch (boolean)
  {
  case(VOLT_TRUE):  return GL_TRUE;
  case(VOLT_FALSE): return GL_FALSE;
  }

  ROA_ASSERT(false);
  return GL_FALSE;
}


volt_bool
convert_gl_boolean_to_volt(GLboolean boolean)
{
  switch (boolean)
  {
  case(GL_TRUE):  return VOLT_TRUE;
  case(GL_FALSE): return VOLT_FALSE;
  }

  ROA_ASSERT(false);
  return VOLT_FALSE;
}

GLenum
convert_gl_format_to_gl_attachment(GLenum fmt)
{
  switch (fmt)
  {
  case(GL_DEPTH32F_STENCIL8): return GL_DEPTH_STENCIL_ATTACHMENT;
  case(GL_DEPTH_COMPONENT):   return GL_DEPTH_ATTACHMENT;
  }

  ROA_ASSERT(false);
  return GL_NONE;
}

GLenum
convert_volt_format_to_gl_type(volt_pixel_format fmt)
{
  switch (fmt)
  {
  case(VOLT_PIXEL_FORMAT_RGB):                return GL_UNSIGNED_BYTE;
  case(VOLT_PIXEL_FORMAT_RGBA):               return GL_UNSIGNED_BYTE;
  case(VOLT_PIXEL_FORMAT_RGBA32F):            return GL_FLOAT;
  case(VOLT_PIXEL_FORMAT_DEPTH32F):           return GL_FLOAT;
  case(VOLT_PIXEL_FORMAT_DEPTH32F_STENCIL8):  return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
  }

  ROA_ASSERT(false);
  return GL_NONE;
}

GLenum
convert_volt_format_to_gl_format(volt_pixel_format fmt)
{
  switch (fmt)
  {
  case(VOLT_PIXEL_FORMAT_RGB):                return GL_RGB;
  case(VOLT_PIXEL_FORMAT_RGBA):               return GL_RGBA;
  case(VOLT_PIXEL_FORMAT_RGBA32F):            return GL_RGBA;
  case(VOLT_PIXEL_FORMAT_DEPTH32F):           return GL_DEPTH_COMPONENT;
  case(VOLT_PIXEL_FORMAT_DEPTH32F_STENCIL8):  return GL_DEPTH32F_STENCIL8;
  }

  ROA_ASSERT(false);
  return GL_NONE;
};

GLint
convert_volt_format_to_gl_internal_format(volt_pixel_format fmt)
{
  switch (fmt)
  {
  case(VOLT_PIXEL_FORMAT_RGB):                return GL_RGB;
  case(VOLT_PIXEL_FORMAT_RGBA):               return GL_RGBA;
  case(VOLT_PIXEL_FORMAT_RGBA32F):            return GL_RGBA32F;
  case(VOLT_PIXEL_FORMAT_DEPTH32F):           return GL_DEPTH_COMPONENT32F;
  case(VOLT_PIXEL_FORMAT_DEPTH32F_STENCIL8):  return GL_DEPTH32F_STENCIL8;
  }

  ROA_ASSERT(false);
  return GL_NONE;
};


volt_pixel_format
convert_gl_format_to_volt(GLenum fmt)
{
  switch (fmt)
  {
  case(GL_RGB):                 return VOLT_PIXEL_FORMAT_RGB;
  case(GL_RGBA):                return VOLT_PIXEL_FORMAT_RGBA;
  case(GL_RGBA32F):             return VOLT_PIXEL_FORMAT_RGBA32F;
  case(GL_DEPTH_COMPONENT32F):  return VOLT_PIXEL_FORMAT_DEPTH32F;
  }

  ROA_ASSERT(false);
  return VOLT_PIXEL_FORMAT_RGBA;
}


GLenum
convert_volt_tex_dimention_to_gl_target(volt_texture_dimentions di)
{
  switch (di)
  {
  case(VOLT_TEXTURE_2D): return GL_TEXTURE_2D;
  }

  ROA_ASSERT(false);
  return GL_TEXTURE_2D;
}


volt_texture_dimentions
convert_gl_dimention_to_volt(GLenum di)
{
  switch (di)
  {
  case(GL_TEXTURE_2D): return VOLT_TEXTURE_2D;
  }

  ROA_ASSERT(false);
  return VOLT_TEXTURE_2D;
}


GLbitfield
convert_volt_clear_to_gl_flag(unsigned flags)
{
  GLbitfield mask = 0;

  if (flags & VOLT_CLEAR_COLOR) { mask |= GL_COLOR_BUFFER_BIT; }
  if (flags & VOLT_CLEAR_DEPTH) { mask |= GL_DEPTH_BUFFER_BIT; }
  if (flags & VOLT_CLEAR_STENCIL) { mask |= GL_STENCIL_BUFFER_BIT; }

  return mask;
}


#endif /* inc guard  */
