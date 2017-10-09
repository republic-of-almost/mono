#ifndef ANGELSCRIPT_INCLUDED_C81F7A87_2399_4CE5_B838_3EE4573B05A9
#define ANGELSCRIPT_INCLUDED_C81F7A87_2399_4CE5_B838_3EE4573B05A9


#include <nil/fwd.hpp>
#include <lib/array.hpp>


/* ----------------------------------------- [ AngelScript Aspect Config ] -- */


class asIScriptEngine;
class asIScriptObject;
class asIScriptFunction;
class asIScriptModule;
class asIScriptContext;


/* ------------------------------------------- [ AngelScript Aspect Data ] -- */

struct obj_method {
  asIScriptObject *obj;
  asIScriptFunction *func;
};


struct update_method {


};


struct AScript_data
{
  asIScriptEngine  *engine;
  asIScriptModule  *module;
  asIScriptContext *ctx;

  lib::array<asIScriptObject*>  objects;
  lib::array<obj_method>        methods_start;
  lib::array<obj_method>        methods_update;
};


/* --------------------------------------- [ AngelScript Aspect Lifetime ] -- */


void
angelscript_aspect_startup(Nil_ctx *ctx, void *data);


void
angelscript_aspect_tick(Nil_ctx *ctx, void *data);


void
angelscript_aspect_shutdown(Nil_ctx *ctx, void *data);


/* ------------------------------------------ [ AngelScript Aspect Tasks ] -- */






#endif // inc guard
