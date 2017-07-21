#ifndef KEYBOARD_INCLUDED_A722474A_7FC4_42DF_92CE_0C94E65FB794
#define KEYBOARD_INCLUDED_A722474A_7FC4_42DF_92CE_0C94E65FB794


#include <roa/fundamental.hpp>
#include <roa/keycodes.hpp>


namespace ROA {
namespace Keyboard {


uint32_t      key_state(const uint32_t keycode);
const char*   key_name(const uint32_t keycode);


} // ns
} // ns


#endif // inc guard
