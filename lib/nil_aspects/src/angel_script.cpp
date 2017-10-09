#include <aspect/angel_script.hpp>
#include <angelscript/angelscript.h>
#include <angelscript/add_on/scriptstdstring.h>
#include <angelscript/setup.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>
#include <stdio.h>
#include <string>


/* ------------------------------------------------ [ AngelScript Config ] -- */


void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}


void PrintString(std::string &str)
{
	printf("%s\n", str.c_str());
}


uint32_t timeGetTime()
{
	return 123;
}


int CompileScript(asIScriptEngine *engine)
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
}

void LineCallback(asIScriptContext *ctx, uint32_t *timeOut)
{
	// If the time out is reached we abort the script
//	if( *timeOut < timeGetTime() )
//		ctx->Abort();

	// It would also be possible to only suspend the script,
	// instead of aborting it. That would allow the application
	// to resume the execution where it left of at a later 
	// time, by simply calling Execute() again.
}


/* --------------------------------------- [ AngelScript Aspect Lifetime ] -- */


void
angelscript_aspect_startup(Nil_ctx *nil_ctx, void *data)
{
  AScript_data *self = reinterpret_cast<AScript_data*>(data);
  LIB_ASSERT(self);
  
  int r = 0;

	AS_utils::setup_engine(&self->engine, &self->ctx);
  AS_utils::setup_callbacks(self->engine, self->ctx);
  
  RegisterStdString(self->engine);

	if( !strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") )
	{
		// Register the functions that the scripts will be allowed to use.
		// Note how the return code is validated with an assert(). This helps
		// us discover where a problem occurs, and doesn't pollute the code
		// with a lot of if's. If an error occurs in release mode it will
		// be caught when a script is being built, so it is not necessary
		// to do the verification here as well.
		r = self->engine->RegisterGlobalFunction("void Print(string &in)", asFUNCTION(PrintString), asCALL_CDECL); assert( r >= 0 );
		r = self->engine->RegisterGlobalFunction("uint GetSystemTime()", asFUNCTION(timeGetTime), asCALL_STDCALL); assert( r >= 0 );
	}
  
	AS_utils::load_script_directory(self->engine, "");
  

	// We don't want to allow the script to hang the application, e.g. with an
	// infinite loop, so we'll use the line callback function to set a timeout
	// that will abort the script after a certain time. Before executing the 
	// script the timeOut variable will be set to the time when the script must 
	// stop executing. 
	uint32_t timeOut;
//	r = self->ctx->SetLineCallback(asFUNCTION(LineCallback), &timeOut, asCALL_CDECL);
//	if( r < 0 )
//	{
//		printf("Failed to set the line callback function.\n");
//		self->ctx->Release();
//		self->engine->Release();
//		return;
//	}
  
	// Find the function for the function we want to execute.
	asIScriptFunction *func = self->engine->GetModule(0)->GetFunctionByDecl("float calc(float, float)");
  auto type = self->engine->GetModule(0)->GetTypeInfoByDecl("GameManager");
  
  // Get the factory function from the object type
  asIScriptFunction *factory = type->GetFactoryByDecl("GameManager @GameManager()");
  // Prepare the context to call the factory function
  r = self->ctx->Prepare(factory);
  // Execute the call
  r = self->ctx->Execute();
  // Get the object that was created
  asIScriptObject *obj = *(asIScriptObject**)self->ctx->GetAddressOfReturnValue();
  
  obj->AddRef();
  
  asIScriptFunction *method = type->GetMethodByDecl("void on_start()");
  // Prepare the context for calling the method
  self->ctx->Prepare(method);
  // Set the object pointer
  self->ctx->SetObject(obj);
  // Execute the call
  self->ctx->Execute();

  
	if( func == 0 )
	{
		printf("The function 'float calc(float, float)' was not found.\n");
		self->ctx->Release();
		self->engine->Release();
		return;
	}

	// Prepare the script context with the function we wish to execute. Prepare()
	// must be called on the context before each new script function that will be
	// executed. Note, that if you intend to execute the same function several 
	// times, it might be a good idea to store the function returned by 
	// GetFunctionByDecl(), so that this relatively slow call can be skipped.
	r = self->ctx->Prepare(func);
	if( r < 0 ) 
	{
		printf("Failed to prepare the context.\n");
		self->ctx->Release();
		self->engine->Release();
		return;
	}

	// Now we need to pass the parameters to the script function. 
	self->ctx->SetArgFloat(0, 3.14159265359f);
	self->ctx->SetArgFloat(1, 2.71828182846f);

	// Set the timeout before executing the function. Give the function 1 sec
	// to return before we'll abort it.
	timeOut = timeGetTime() + 1000;

	// Execute the function
	printf("Executing the script.\n");
	printf("---\n");
	r = self->ctx->Execute();
	printf("---\n");
	if( r != asEXECUTION_FINISHED )
	{
		// The execution didn't finish as we had planned. Determine why.
		if( r == asEXECUTION_ABORTED )
			printf("The script was aborted before it could finish. Probably it timed out.\n");
		else if( r == asEXECUTION_EXCEPTION )
		{
			printf("The script ended with an exception.\n");

			// Write some information about the script exception
			asIScriptFunction *func = self->ctx->GetExceptionFunction();
//			cout << "func: " << func->GetDeclaration() << endl;
//			cout << "modl: " << func->GetModuleName() << endl;
//			cout << "sect: " << func->GetScriptSectionName() << endl;
//			cout << "line: " << ctx->GetExceptionLineNumber() << endl;
//			cout << "desc: " << ctx->GetExceptionString() << endl;
		}
		else
    {
//			cout << "The script ended for some unforeseen reason (" << r << ")." << endl;
    }
	}
	else
	{
		// Retrieve the return value from the context
		float returnValue = self->ctx->GetReturnFloat();
//		cout << "The script function returned: " << returnValue << endl;
	}

	// We must release the contexts when no longer using them
//	ctx->Release();  
}


void
angelscript_aspect_tick(Nil_ctx *ctx, void *data)
{
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


