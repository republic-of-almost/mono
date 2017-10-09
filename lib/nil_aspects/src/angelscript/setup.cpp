#include <angelscript/setup.hpp>
#include <angelscript/callbacks.hpp>
#include <angelscript/angelscript.h>
#include <lib/logging.hpp>
#include <stdio.h>
#include <string>


namespace AS_utils {


bool
setup_engine(asIScriptEngine **engine, asIScriptContext **ctx)
{
  *engine = asCreateScriptEngine();
  
  if(*engine == 0)
  {
    LOG_ERROR("Failed to create script engine");
    return false;
  }
  
  *ctx = (*engine)->CreateContext();
  
  if(*ctx == 0)
  {
    (*engine)->Release();
    
    LOG_ERROR("Failed to create script context");
    return false;
  }
  
  return true;
}


bool
setup_callbacks(asIScriptEngine *engine, asIScriptContext *ctx)
{
  int set_msg_cb = engine->SetMessageCallback(asFUNCTION(message_callback), 0, asCALL_CDECL);
  
  static uint32_t time_out{};
  int set_line_cb = ctx->SetLineCallback(asFUNCTION(line_callback), &time_out, asCALL_CDECL);
  
  return set_msg_cb & set_line_cb;
}


bool
load_script_directory(asIScriptEngine *engine, const char *dir)
{
	int r;

	// We will load the script from a file on the disk.
	FILE *f = fopen("/Users/PhilCK/Desktop/rep_of_a/assets/blank/scripts/game_manager.as", "rb");
	if( f == 0 )
	{
		printf("Failed to open the script file 'script.as'.\n");
		return -1;
	}

	// Determine the size of the file	
	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	fseek(f, 0, SEEK_SET);

	// On Win32 it is possible to do the following instead
	// int len = _filelength(_fileno(f));

	// Read the entire file
	std::string script;
	script.resize(len);
	size_t c = fread(&script[0], len, 1, f);
	fclose(f);

	if( c == 0 ) 
	{
		printf("Failed to load script file.\n");
		return -1;
	}
  
	// Add the script sections that will be compiled into executable code.
	// If we want to combine more than one file into the same script, then 
	// we can call AddScriptSection() several times for the same module and
	// the script engine will treat them all as if they were one. The script
	// section name, will allow us to localize any errors in the script code.
	asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
	r = mod->AddScriptSection("script", &script[0], len);
	if( r < 0 ) 
	{
		printf("AddScriptSection() failed\n");
		return -1;
	}
	
	// Compile the script. If there are any compiler messages they will
	// be written to the message stream that we set right after creating the 
	// script engine. If there are no errors, and no warnings, nothing will
	// be written to the stream.
	r = mod->Build();
	if( r < 0 )
	{
		printf("Build() failed\n");
		return -1;
	}

	// The engine doesn't keep a copy of the script sections after Build() has
	// returned. So if the script needs to be recompiled, then all the script
	// sections must be added again.

	// If we want to have several scripts executing at different times but 
	// that have no direct relation with each other, then we can compile them
	// into separate script modules. Each module use their own namespace and 
	// scope, so function names, and global variables will not conflict with
	// each other.

	return 0;
  
  
  return true;
}


asIScriptObject*
create_object(asIScriptEngine *engine, asIScriptContext *ctx, asITypeInfo *type, const char *obj_name)
{
  int r = 0;
  
  char name[2048]{};
  strcat(name, obj_name);
  strcat(name, " @");
  strcat(name, obj_name);
  strcat(name, "()");

  asIScriptFunction *factory = type->GetFactoryByDecl(name);
  
  r = ctx->Prepare(factory);
  r = ctx->Execute();
  
  asIScriptObject *obj = *(asIScriptObject**)ctx->GetAddressOfReturnValue();
  
  if(obj)
  {
    obj->AddRef();
  }

  return obj;
}


asIScriptFunction *
get_method(asITypeInfo *type, const char *decl)
{
  asIScriptFunction *method = type->GetMethodByDecl(decl);
  
  return method;
}


} // ns
