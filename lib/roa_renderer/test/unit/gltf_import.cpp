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

    REQUIRE(import_scene.accessors[0].buffer_view == 0);
    REQUIRE(import_scene.accessors[0].count == 36);
    REQUIRE(import_scene.accessors[0].type == GLTF_TYPE_SCALAR);
    REQUIRE(import_scene.accessors[0].component_type == GLTF_COMPONENT_TYPE_UNSIGNED_BYTE);

    REQUIRE(import_scene.accessors[1].buffer_view == 1);
    REQUIRE(import_scene.accessors[1].count == 24);
    REQUIRE(import_scene.accessors[1].type == GLTF_TYPE_VEC3);
    REQUIRE(import_scene.accessors[1].component_type == GLTF_COMPONENT_TYPE_FLOAT);

    REQUIRE(import_scene.accessors[2].buffer_view == 2);
    REQUIRE(import_scene.accessors[2].count == 24);
    REQUIRE(import_scene.accessors[2].type == GLTF_TYPE_VEC3);
    REQUIRE(import_scene.accessors[2].component_type == GLTF_COMPONENT_TYPE_FLOAT);

    REQUIRE(import_scene.accessors[3].buffer_view == 3);
    REQUIRE(import_scene.accessors[3].count == 24);
    REQUIRE(import_scene.accessors[3].type == GLTF_TYPE_VEC4);
    REQUIRE(import_scene.accessors[3].component_type == GLTF_COMPONENT_TYPE_FLOAT);

    REQUIRE(import_scene.accessors[4].buffer_view == 4);
    REQUIRE(import_scene.accessors[4].count == 24);
    REQUIRE(import_scene.accessors[4].type == GLTF_TYPE_VEC2);
    REQUIRE(import_scene.accessors[4].component_type == GLTF_COMPONENT_TYPE_FLOAT);
  }

  gltf_free(&import_scene);
}
