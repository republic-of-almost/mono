#ifndef MESH_INCLUDED_DE6D03E1_2A27_49AC_962F_7442413F5396
#define MESH_INCLUDED_DE6D03E1_2A27_49AC_962F_7442413F5396


#include <nil/resource/mesh.hpp>
#include <lib/array.hpp>


struct Nil_mesh_data
{
  lib::array<uint32_t, 128> keys{uint32_t{0}};
  lib::array<Nil_mesh, 128> meshes{Nil_mesh{}};
};


#endif // inc guard
