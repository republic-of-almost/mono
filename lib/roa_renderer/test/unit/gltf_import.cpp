#include <catch/catch.hpp>
#include <importer/gltf.h>
#include <roa_lib/dir.h>
#include <cstring>


TEST_CASE("GLTF Importer")
{
  struct gltf_import import{};

  char path[2048]{};
  strcat(path, roa_exe_dir());
  strcat(path, "assets/cube.gltf");

  gltf_import(path, &import);

  SECTION("Import Cube")
  {
    
  }

  gltf_free(&import);
}
