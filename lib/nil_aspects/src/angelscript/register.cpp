#include <angelscript/register.hpp>
#include <angelscript/angelscript.h>
#include <angelscript/add_on/scriptstdstring.h>
#include <assert.h>
#include <string>
#include <stdio.h>


void
PrintString(std::string str)
{
  printf("%s\n", str.c_str());
}


struct Object
{
  int id;
};


void Constructor(void *memory)
{
  // Initialize the pre-allocated memory by calling the
  // object constructor with the placement-new operator
  new(memory) Object();
}
void Destructor(void *memory)
{
  // Uninitialize the memory by calling the object destructor
  ((Object*)memory)->~Object();
}


namespace AS_utils {


void
register_hooks(asIScriptEngine *as)
{
  RegisterStdString(as);

  int r = 0;
  
  /* ROA */
  as->SetDefaultNamespace("ROA");
  as->RegisterObjectType("Camera", sizeof(Object), asOBJ_VALUE);
  as->RegisterObjectBehaviour("Camera", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Constructor), asCALL_CDECL_OBJLAST);
  as->RegisterObjectBehaviour("Camera", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Destructor), asCALL_CDECL_OBJLAST);
  
//  as->RegisterObjectType("Transform", sizeof(Object), asOBJ_VALUE);
  
  /* Debug Items */
  as->SetDefaultNamespace("Debug");
  r = as->RegisterGlobalFunction("void Print(string &in)", asFUNCTION(PrintString), asCALL_CDECL); assert( r >= 0 );
}


} // ns
