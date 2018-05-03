#include <catch/catch.hpp>
#include <importer/gltf.h>
#include <roa_lib/dir.h>
#include <cstring>


TEST_CASE("GLTF Importer")
{
  struct gltf_import import_scene{};

  char path[2048]{};
  strcat(path, roa_exe_dir());
  strcat(path, "assets/cube.gltf");

  gltf_import(path, &import_scene);

  SECTION("accessors")
  {
    REQUIRE(import_scene.accessor_count == 5);

    REQUIRE(import_scene.accessors[0].count == 36);
    REQUIRE(import_scene.accessors[1].count == 24);
    REQUIRE(import_scene.accessors[2].count == 24);
    REQUIRE(import_scene.accessors[3].count == 24);
    REQUIRE(import_scene.accessors[4].count == 24);
  }

  gltf_free(&import_scene);
}
