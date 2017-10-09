#ifndef CALLBACKS_INCLUDED_670AA79A_A368_48C6_BBB1_385D50CEA603
#define CALLBACKS_INCLUDED_670AA79A_A368_48C6_BBB1_385D50CEA603


#include <angelscript/fwd.hpp>
#include <stdint.h>


namespace AS_utils {


void message_callback(const asSMessageInfo *msg, void *param);
void line_callback(asIScriptContext *ctx, uint32_t *time_out);


} // ns


#endif // inc guard
