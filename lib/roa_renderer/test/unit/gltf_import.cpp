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
    REQUIRE(import_scene.accessors[0].max[0] == 23.f);
    REQUIRE(import_scene.accessors[0].min[0] == 0.f);

    REQUIRE(import_scene.accessors[1].buffer_view == 1);
    REQUIRE(import_scene.accessors[1].count == 24);
    REQUIRE(import_scene.accessors[1].type == GLTF_TYPE_VEC3);
    REQUIRE(import_scene.accessors[1].component_type == GLTF_COMPONENT_TYPE_FLOAT);
    REQUIRE(import_scene.accessors[1].max[0] == 0.5000001788139343f);
    REQUIRE(import_scene.accessors[1].max[1] == 0.5f);
    REQUIRE(import_scene.accessors[1].max[2] == 0.5000003576278687f);
    REQUIRE(import_scene.accessors[1].min[0] == -0.5000002384185791f);
    REQUIRE(import_scene.accessors[1].min[1] == -0.5f);
    REQUIRE(import_scene.accessors[1].min[2] == -0.5000001192092896f);

    REQUIRE(import_scene.accessors[2].buffer_view == 2);
    REQUIRE(import_scene.accessors[2].count == 24);
    REQUIRE(import_scene.accessors[2].type == GLTF_TYPE_VEC3);
    REQUIRE(import_scene.accessors[2].component_type == GLTF_COMPONENT_TYPE_FLOAT);
    REQUIRE(import_scene.accessors[2].max[0] == 1.0f);
    REQUIRE(import_scene.accessors[2].max[1] == 1.0f);
    REQUIRE(import_scene.accessors[2].max[2] == 1.0f);
    REQUIRE(import_scene.accessors[2].min[0] == -1.0f);
    REQUIRE(import_scene.accessors[2].min[1] == -1.0f);
    REQUIRE(import_scene.accessors[2].min[2] == -1.0f);

    REQUIRE(import_scene.accessors[3].buffer_view == 3);
    REQUIRE(import_scene.accessors[3].count == 24);
    REQUIRE(import_scene.accessors[3].type == GLTF_TYPE_VEC4);
    REQUIRE(import_scene.accessors[3].component_type == GLTF_COMPONENT_TYPE_FLOAT);
    REQUIRE(import_scene.accessors[3].max[0] == 1.0f);
    REQUIRE(import_scene.accessors[3].max[1] == 1.1923313536499336e-07f);
    REQUIRE(import_scene.accessors[3].max[2] == 2.980232238769531e-07f);
    REQUIRE(import_scene.accessors[3].max[3] == 1.0f);
    REQUIRE(import_scene.accessors[3].min[0] == 2.980232238769531e-07f);
    REQUIRE(import_scene.accessors[3].min[1] == -1.3411180077582685e-07f);
    REQUIRE(import_scene.accessors[3].min[2] == -1.0f);
    REQUIRE(import_scene.accessors[3].min[3] == 1.0f);

    REQUIRE(import_scene.accessors[4].buffer_view == 4);
    REQUIRE(import_scene.accessors[4].count == 24);
    REQUIRE(import_scene.accessors[4].type == GLTF_TYPE_VEC2);
    REQUIRE(import_scene.accessors[4].component_type == GLTF_COMPONENT_TYPE_FLOAT);
    REQUIRE(import_scene.accessors[4].max[0] == 0.9999000430107117f);
    REQUIRE(import_scene.accessors[4].max[1] == 0.9999000200114097f);
    REQUIRE(import_scene.accessors[4].min[0] == 9.997998859034851e-05f);
    REQUIRE(import_scene.accessors[4].min[1] == 9.995698928833008e-05f);
  }

  gltf_free(&import_scene);
}
