#include <aspect/angel_script.hpp>
#include <angelscript/angelscript.h>
#include <angelscript/register.hpp>
#include <angelscript/setup.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>
#include <stdio.h>
#include <string>


/* --------------------------------------- [ AngelScript Aspect Lifetime ] -- */


void
angelscript_aspect_startup(Nil_ctx *nil_ctx, void *data)
{
  AScript_data *self = reinterpret_cast<AScript_data*>(data);
  LIB_ASSERT(self);
  
	AS_utils::setup_engine(&self->engine, &self->ctx);
  AS_utils::setup_callbacks(self->engine, self->ctx);
  
  AS_utils::register_hooks(self->engine);
  
	AS_utils::load_script_directory(self->engine, "");
  
  const char *name = "ApplicationMain";
  auto type = self->engine->GetModule(0)->GetTypeInfoByDecl(name);
  asIScriptObject *obj = AS_utils::create_object(self->engine, self->ctx, type, name);
  asIScriptFunction *start_method = AS_utils::get_method(type, "void on_start()");
  
  if(start_method)
  {
    self->methods_start.emplace_back(obj, start_method);
  }
  
  asIScriptFunction *update_method = AS_utils::get_method(type, "void on_update()");
  
  if(update_method)
  {
    self->methods_update.emplace_back(obj, update_method);
  }
}


void
angelscript_aspect_tick(Nil_ctx *ctx, void *data)
{
  AScript_data *self = reinterpret_cast<AScript_data*>(data);
  LIB_ASSERT(self);

  for(auto m : self->methods_start)
  {
    self->ctx->Prepare(m.func);
    self->ctx->SetObject(m.obj);
    self->ctx->Execute();
  }
  
  self->methods_start.clear();
  
  for(auto m : self->methods_update)
  {
    self->ctx->Prepare(m.func);
    self->ctx->SetObject(m.obj);
    self->ctx->Execute();
  }
}


void
angelscript_aspect_shutdown(Nil_ctx *ctx, void *data)
{
  AScript_data *self = reinterpret_cast<AScript_data*>(data);
  LIB_ASSERT(self);


	// We must release the contexts when no longer using them
//	ctx->Release();

	// Shut down the engine
	self->engine->ShutDownAndRelease();
}


/* ------------------------------------------ [ AngelScript Aspect Tasks ] -- */


