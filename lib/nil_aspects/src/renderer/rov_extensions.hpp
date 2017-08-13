#ifndef ROV_EXTENSIONS_INCLUDED_D901B900_7537_40F4_934B_75746A4E3AA9
#define ROV_EXTENSIONS_INCLUDED_D901B900_7537_40F4_934B_75746A4E3AA9


#include <nil/fwd.hpp>
#include <stddef.h>
#include <stdint.h>


namespace Nil_ext {


void
rov_render_camera_cross(Nil::Data::Camera cam, uint32_t viewport[2]);


void
rov_render_bounding_box(Nil::Data::Bounding_box bbs[], size_t count);


} // ns


#endif // inc guard
