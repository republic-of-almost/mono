#ifndef MOUSE_INCLUDED_6C9B02A6_2E62_4821_9295_5A6EAB3DEB22
#define MOUSE_INCLUDED_6C9B02A6_2E62_4821_9295_5A6EAB3DEB22


#include <roa/fundamental.hpp>


namespace ROA {
namespace Mouse {


Point     get_delta();
Point     get_position();

void      set_captured(const bool set);
bool      is_captured();


} // ns
} // ns


#endif // inc guard
