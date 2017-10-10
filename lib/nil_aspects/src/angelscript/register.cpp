#include <angelscript/register.hpp>
#include <angelscript/angelscript.h>
#include <angelscript/add_on/scriptstdstring.h>
#include "object_hooks/object.hpp"
#include "object_hooks/camera.hpp"
#include <assert.h>
#include <string>
#include <stdio.h>


void
PrintString(std::string str)
{
  printf("%s\n", str.c_str());
}


namespace AS_utils {


void
register_hooks(asIScriptEngine *as)
{
  RegisterStdString(as);

  int r = 0;
  
  /* ROA */
  
  r = as->SetDefaultNamespace("ROA"); assert(r >= 0);
  
  /* object types */
  
  const asDWORD object_type_flags = asOBJ_REF |  asOBJ_NOCOUNT;
  
  r = as->RegisterObjectType("Object",      sizeof(Object), object_type_flags); assert(r >= 0);
  r = as->RegisterObjectType("Camera",      sizeof(Object), object_type_flags); assert(r >= 0);
  r = as->RegisterObjectType("Transform",   sizeof(Object), object_type_flags); assert(r >= 0);
  r = as->RegisterObjectType("Renderable",  sizeof(Object), object_type_flags); assert(r >= 0);
  r = as->RegisterObjectType("Light",       sizeof(Object), object_type_flags); assert(r >= 0);

  /* object base */
  
  r = as->RegisterObjectBehaviour("Object", asBEHAVE_FACTORY, "Object@ f()", asFUNCTION(object_new), asCALL_CDECL); assert(r >= 0);
  //r = as->RegisterObjectBehaviour("Object", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(object_ctor), asCALL_CDECL_OBJLAST); assert(r >= 0);
  //r = as->RegisterObjectBehaviour("Object", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(object_dtor), asCALL_CDECL_OBJLAST); assert(r >= 0);
  
  r = as->RegisterObjectMethod("Object", "Camera@ opCast()", asFUNCTION((object_ref_cast<Object,Camera>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Object", "const Camera@ opCast() const", asFUNCTION((object_ref_cast<Object,Camera>)), asCALL_CDECL_OBJLAST); assert(r >= 0);

//  r = as->RegisterObjectMethod("Object", "Transform@ opCast()", asFUNCTION((object_ref_cast<Object,Transform>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
//  r = as->RegisterObjectMethod("Object", "const Transform@ opCast() const", asFUNCTION((object_ref_cast<Object,Transform>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
//
//  r = as->RegisterObjectMethod("Object", "Renderable@ opCast()", asFUNCTION((object_ref_cast<Object,Renderable>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
//  r = as->RegisterObjectMethod("Object", "const Renderable@ opCast() const", asFUNCTION((object_ref_cast<Object,Renderable>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
//
//  r = as->RegisterObjectMethod("Object", "Light@ opCast()", asFUNCTION((object_ref_cast<Object,Light>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
//  r = as->RegisterObjectMethod("Object", "const Light@ opCast() const", asFUNCTION((object_ref_cast<Object,Light>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
  
  /* camera */
  
  r = as->RegisterObjectBehaviour("Camera", asBEHAVE_FACTORY, "Camera@ f()",      asFUNCTION(camera_new), asCALL_CDECL); assert(r >= 0);
//  r = as->RegisterObjectBehaviour("Camera", asBEHAVE_CONSTRUCT, "void f()",       asFUNCTION(camera_ctor), asCALL_CDECL_OBJLAST); assert(r >= 0);
//  r = as->RegisterObjectBehaviour("Camera", asBEHAVE_DESTRUCT, "void f()",        asFUNCTION(camera_dtor), asCALL_CDECL_OBJLAST); assert(r >= 0);

  r = as->RegisterObjectMethod("Camera", "Object@ opImplCast()",                  asFUNCTION((object_ref_cast<Camera,Object>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Camera", "const Object@ opImplCast() const",      asFUNCTION((object_ref_cast<Camera,Object>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
  
  r = as->RegisterObjectMethod("Camera", "void set_fov(float)",                   asFUNCTION(camera_set_fov),           asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Camera", "float get_fov() const",                 asFUNCTION(camera_get_fov),           asCALL_CDECL_OBJLAST); assert(r >= 0);
  
  r = as->RegisterObjectMethod("Camera", "void set_near_plane(float)",            asFUNCTION(camera_set_nearp),         asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Camera", "float get_near_plane() const",          asFUNCTION(camera_get_nearp),         asCALL_CDECL_OBJLAST); assert(r >= 0);

  r = as->RegisterObjectMethod("Camera", "void set_far_plane(float)",             asFUNCTION(camera_set_farp),          asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Camera", "float get_far_plane() const",           asFUNCTION(camera_get_farp),          asCALL_CDECL_OBJLAST); assert(r >= 0);

  r = as->RegisterObjectMethod("Camera", "void set_width(float)",                 asFUNCTION(camera_set_width),         asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Camera", "float get_width() const",               asFUNCTION(camera_get_width),         asCALL_CDECL_OBJLAST); assert(r >= 0);

  r = as->RegisterObjectMethod("Camera", "void set_height(float)",                asFUNCTION(camera_set_height),        asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Camera", "float get_height() const",              asFUNCTION(camera_get_height),        asCALL_CDECL_OBJLAST); assert(r >= 0);
  
  r = as->RegisterObjectMethod("Camera", "void set_clear_color_buffer(bool)",     asFUNCTION(camera_set_color_clear),   asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Camera", "float get_clear_color_buffer() const",  asFUNCTION(camera_get_color_clear),   asCALL_CDECL_OBJLAST); assert(r >= 0);

  r = as->RegisterObjectMethod("Camera", "void set_clear_depth_buffer(bool)",     asFUNCTION(camera_set_depth_clear),   asCALL_CDECL_OBJLAST); assert(r >= 0);
  r = as->RegisterObjectMethod("Camera", "float get_clear_depth_buffer() const",  asFUNCTION(camera_get_depth_clear),   asCALL_CDECL_OBJLAST); assert(r >= 0);
  
  /* Debug Items */
  
  r = as->SetDefaultNamespace("Debug"); assert(r >= 0);
  r = as->RegisterGlobalFunction("void Print(string &in)", asFUNCTION(PrintString), asCALL_CDECL); assert(r >= 0);
}


} // ns
