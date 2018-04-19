#ifndef OGL_VOLT_INCLUDED_
#define OGL_VOLT_INCLUDED_


#include <roa_lib/assert.h>
#include <volt/volt.h>
#include <GL/gl3w.h>


/* -------------------------------------------------------- [ gl to volt ] -- */


GLenum
volt_primitive_to_gl(volt_primitive_type prim)
{
  switch(prim)
  {
    case(VOLT_PRIM_TRIANGLES): return GL_TRIANGLES;
    case(VOLT_PRIM_POINTS): return GL_POINTS;
  }

  ROA_ASSERT(false);
  return GL_TRIANGLES;
}


GLenum
volt_cull_mode_to_gl(volt_cull_mode cull)
{
  switch(cull)
  {
    case(VOLT_CULL_BACK): return GL_BACK;
    case(VOLT_CULL_FRONT):  return GL_FRONT;
  }

  ROA_ASSERT(false);
  return GL_BACK;
}


GLenum
volt_winding_order_to_gl(volt_winding_order order)
{
  switch(order)
  {
    case(VOLT_WIND_CCW): return GL_CCW;
    case(VOLT_WIND_CW): return GL_CW;
  }

  ROA_ASSERT(false);
  return GL_CCW;
};


GLenum
volt_blend_eq_to_gl(volt_blend_equation eq)
{
  switch(eq)
  {
    case(VOLT_BLEND_EQ_NONE): return GL_NONE;
    case(VOLT_BLEND_EQ_ADD): return GL_FUNC_ADD;
  }

  ROA_ASSERT(false);
  return GL_NONE;
}


GLenum
volt_blend_flag_to_gl(volt_blend_flag fl)
{
  switch(fl)
  {
    case(VOLT_BLEND_FLAG_ONE): return GL_ONE;
  }

  ROA_ASSERT(false);
  return GL_ONE;
}


GLenum
volt_attribute_to_gl_type(volt_input_attribute attr)
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
volt_attribute_to_gl_normalized(volt_input_attribute attr)
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
volt_boolean_to_gl(volt_bool boolean)
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
gl_boolean_to_volt(GLboolean boolean)
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
gl_format_to_gl_attachment(GLenum fmt)
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
volt_format_to_gl_type(volt_pixel_format fmt)
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
volt_format_to_gl_format(volt_pixel_format fmt)
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
volt_format_to_gl_internal_format(volt_pixel_format fmt)
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
gl_format_to_volt(GLenum fmt)
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
volt_tex_dimention_to_gl_target(volt_texture_dimentions di)
{
  switch (di)
  {
    case(VOLT_TEXTURE_2D): return GL_TEXTURE_2D;
  }

  ROA_ASSERT(false);
  return GL_TEXTURE_2D;
}


volt_texture_dimentions
gl_dimention_to_volt(GLenum di)
{
  switch (di)
  {
    case(GL_TEXTURE_2D): return VOLT_TEXTURE_2D;
  }

  ROA_ASSERT(false);
  return VOLT_TEXTURE_2D;
}


GLbitfield
volt_clear_to_gl_flag(unsigned flags)
{
  GLbitfield mask = 0;

  if (flags & VOLT_CLEAR_COLOR)   { mask |= GL_COLOR_BUFFER_BIT;    }
  if (flags & VOLT_CLEAR_DEPTH)   { mask |= GL_DEPTH_BUFFER_BIT;    }
  if (flags & VOLT_CLEAR_STENCIL) { mask |= GL_STENCIL_BUFFER_BIT;  }

  return mask;
}


#endif /* inc guard  */
