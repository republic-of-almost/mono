#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/resource/resource.hpp>
#include <lib/logging.hpp>


TEST_CASE("Resource - Mesh")
{
  Nil::Engine nil_engine;
  lib::logging::set_output(0);
  
  SECTION("Initial")
  {
    REQUIRE(Nil::Resource::mesh_count() == 0);
  }
  
  SECTION("Load Pass")
  {
    Nil::Resource::Mesh mesh{};
    mesh.name = "mesh_to_pass";
    
    const bool loaded = Nil::Resource::load(mesh);
    
    REQUIRE(loaded == true);
    REQUIRE(Nil::Resource::mesh_count() == 1);
  }
  
  SECTION("Load Fail")
  {
    Nil::Resource::Mesh mesh{};
    mesh.name = "mesh_to_fail";
    
    const bool should_pass = Nil::Resource::load(mesh);
    const bool should_fail = Nil::Resource::load(mesh);
    
    REQUIRE(should_pass == true);
    REQUIRE(should_fail == false);
  }
};
