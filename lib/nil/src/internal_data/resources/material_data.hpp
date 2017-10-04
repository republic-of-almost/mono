#ifndef MATERIAL_INCLUDED_B6F417E4_C08D_4638_AFEB_DE336CBEF01B
#define MATERIAL_INCLUDED_B6F417E4_C08D_4638_AFEB_DE336CBEF01B


#include <nil/resource/material.hpp>
#include <lib/array.hpp>


struct Nil_material_data
{
  lib::array<uint32_t, 128> keys{uint32_t{0}};
  lib::array<Nil_material, 128> materials{Nil_material{"", 0xFFFFFFFF}};
};


#endif // inc guard
