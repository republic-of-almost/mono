#include <catch/catch.hpp>
#include <lib/model.hpp>
#include <lib/platform.hpp>
#include <lib/logging.hpp>
#include <lib/directory.hpp>
#include <stdint.h>
#include <string.h>


TEST_CASE("Model Test")
{
  SECTION("Load unknown obj")
  {
    // Toggle logging as we know this will output
    lib::logging::set_output(0);
    lib::model model = lib::model_import::load_obj_from_file("does_not_exist");
    lib::logging::set_output(lib::logging::out::console);
    
    REQUIRE(model.mesh_count == 0);
    REQUIRE(model.material_count == 0);
    REQUIRE(model.vertex_count == nullptr);
  }

  SECTION("Simple obj")
  {
    char file[LIB_MAX_FILE_PATH_SIZE];
    strcat(file, lib::dir::exe_path());
    strcat(file, "assets/lib/plane.obj");
  
    lib::model model = lib::model_import::load_obj_from_file(file);
      
    REQUIRE(model.mesh_count == 1);
    REQUIRE(model.material_count == 0);
    REQUIRE(model.vertex_count[0] == 6);
  }
  
  SECTION("Simple obj")
  {
    
  }
}
