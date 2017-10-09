#ifndef SETUP_INCLUDED_DD08FB71_3F76_4F32_A6C7_9E5FFE2522A1
#define SETUP_INCLUDED_DD08FB71_3F76_4F32_A6C7_9E5FFE2522A1


#include <angelscript/fwd.hpp>


namespace AS_utils {


bool
setup_engine(asIScriptEngine **engine, asIScriptContext **ctx);


bool
setup_callbacks(asIScriptEngine *engine, asIScriptContext *ctx);


bool
load_script_directory(asIScriptEngine *engine, const char *dir);


asIScriptObject*
create_object(asIScriptEngine *engine, asIScriptContext *ctx, asITypeInfo *type_info, const char *obj_name);


asIScriptFunction*
get_method(asITypeInfo *type_info, const char *decl);


}


#endif // inc guard
