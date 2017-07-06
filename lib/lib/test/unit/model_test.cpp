#include <catch/catch.hpp>
#include <lib/utilities.hpp>
#include <math/general/general.hpp>
#include <stdint.h>
#include <string.h>


/*
  Test for both memory corruptions and benchmarking
*/
#define FOR_MODEL_STRESS for(uint32_t i = 0; i < 10000; ++i)


TEST_CASE("Model Test")
{
  SECTION("Load unknown obj")
  {
    // Toggle logging as we know this will output
    lib::logging::set_output(0);
    
    FOR_MODEL_STRESS
    {
      lib::model model = lib::model_import::load_obj_from_file("does_not_exist");
      
      REQUIRE(model.mesh_count == 0);
      REQUIRE(model.material_count == 0);
      REQUIRE(model.vertex_count == nullptr);
    }
    
    lib::logging::set_output(lib::logging::out::console);
  }

  SECTION("Simple obj")
  {
    char file[LIB_MAX_FILE_PATH_SIZE];
    strcat(file, lib::dir::exe_path());
    strcat(file, "assets/lib/plane.obj");
  
    FOR_MODEL_STRESS
    {
      lib::model model = lib::model_import::load_obj_from_file(file);
      
      REQUIRE(model.mesh_count               == 1);
      REQUIRE(model.material_count           == 0);
      REQUIRE(model.mesh_material            == nullptr);
      REQUIRE(model.vertex_count[0]          == 6);
      REQUIRE(strcmp(model.name[0], "Plane") == 0);
      
      // Test Vertices //
      
      REQUIRE(math::is_near(model.verts[0][0], +1.f));
      REQUIRE(math::is_near(model.verts[0][1], +0.f));
      REQUIRE(math::is_near(model.verts[0][2], +1.f));
      
      REQUIRE(math::is_near(model.verts[0][3], +1.f));
      REQUIRE(math::is_near(model.verts[0][4], +0.f));
      REQUIRE(math::is_near(model.verts[0][5], -1.f));
      
      REQUIRE(math::is_near(model.verts[0][6], -1.f));
      REQUIRE(math::is_near(model.verts[0][7], +0.f));
      REQUIRE(math::is_near(model.verts[0][8], -1.f));
      
      // --
      
      REQUIRE(math::is_near(model.verts[0][9],  -1.f));
      REQUIRE(math::is_near(model.verts[0][10], +0.f));
      REQUIRE(math::is_near(model.verts[0][11], +1.f));
      
      REQUIRE(math::is_near(model.verts[0][12], +1.f));
      REQUIRE(math::is_near(model.verts[0][13], +0.f));
      REQUIRE(math::is_near(model.verts[0][14], +1.f));
      
      REQUIRE(math::is_near(model.verts[0][15], -1.f));
      REQUIRE(math::is_near(model.verts[0][16], +0.f));
      REQUIRE(math::is_near(model.verts[0][17], -1.f));
    }
  }
  
  SECTION("Complex obj")
  {
    char file[LIB_MAX_FILE_PATH_SIZE];
    strcat(file, lib::dir::exe_path());
    strcat(file, "assets/lib/complex.obj");
  
    FOR_MODEL_STRESS
    {
      lib::model model = lib::model_import::load_obj_from_file(file);
      
      REQUIRE(model.mesh_count == 3);
      REQUIRE(model.material_count == 1);
      
      REQUIRE(model.vertex_count[0] == 36);
      REQUIRE(model.vertex_count[1] == 36);
      REQUIRE(model.vertex_count[2] == 36);
      REQUIRE(model.mesh_material != nullptr);
      
      REQUIRE(strcmp(model.mesh_material[0].name, "SimpleMat") == 0);
      
      REQUIRE(model.mesh_material[0].map_type[0] == lib::map_type::DIFFUSE);
      REQUIRE(strcmp(model.mesh_material[0].map_path[0], "diff.png") == 0);
    }
  }
}
