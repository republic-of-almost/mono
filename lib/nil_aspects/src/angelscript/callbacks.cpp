#include <angelscript/callbacks.hpp>
#include <angelscript/angelscript.h>
#include <stdio.h>

namespace AS_utils {


void
message_callback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}


void
line_callback(asIScriptContext *ctx, uint32_t *time_out)
{
	// If the time out is reached we abort the script
//	if( *time_out < timeGetTime() )
//		ctx->Abort();

	// It would also be possible to only suspend the script,
	// instead of aborting it. That would allow the application
	// to resume the execution where it left of at a later 
	// time, by simply calling Execute() again.
}


} // ns
