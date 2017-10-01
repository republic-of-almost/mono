#include <catch/catch.hpp>
#include <roa/material.hpp>
#include <roa/resource_status.hpp>
#include <roa/color.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA::Material")
{
  lib::logging::set_output(0);


  // --------------------------------------------------- [ Resource Factory ] --


  SECTION("Create Defaults")
  {
    ROA::Material rsrc = ROA::Resource::create<ROA::Material>("Basic");
    
    REQUIRE(rsrc.is_valid() == true);
    REQUIRE(!!rsrc == true);
    REQUIRE(rsrc.get_id() > 0);
    REQUIRE(rsrc.get_resource_type_id() > 0);
    
    const char *type_name = rsrc.get_resource_type_name();
    const char *expected_type_name = "Material";
    
    REQUIRE(strcmp(type_name, expected_type_name) == 0);
    REQUIRE(rsrc.get_status() == ROA::Resource_status::WAITING);
    
    const char *name = rsrc.get_instance_name();
    const char *expected_name = "Basic";
    
    REQUIRE(strcmp(name, expected_name) == 0);
  }
  
  
  SECTION("Create Invalid")
  {
    ROA::Material invalid = ROA::Resource::create<ROA::Material>("");
    
    REQUIRE(invalid.is_valid() == false);
    REQUIRE(!!invalid == false);
    REQUIRE(invalid.get_id() == 0);
  }
  
  
  SECTION("Load New")
  {
    // This is unsupported anyway
    ROA::Material new_rsrc = ROA::Resource::load<ROA::Material>("filepath");
    
    REQUIRE(new_rsrc.is_valid() == false);
  }
  
  
  SECTION("Load Waiting")
  {
    ROA::Material rsrc_load = ROA::Resource::create<ROA::Material>("Make Pending");
    
    REQUIRE(rsrc_load.get_status() == ROA::Resource_status::WAITING);
    
    rsrc_load.load();
    
    REQUIRE(rsrc_load.get_status() == ROA::Resource_status::PENDING);
  }
  
  
  SECTION("Find")
  {
    ROA::Material mat = ROA::Resource::find<ROA::Material>("Basic");
    
    REQUIRE(mat.is_valid() == true);
  }
  
  
  SECTION("Find Invalid")
  {
    ROA::Material mat = ROA::Resource::find<ROA::Material>("Unknown");
    
    REQUIRE(mat.is_valid() == false);
  }
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  SECTION("Default Attrs")
  {
    ROA::Material mat = ROA::Resource::create<ROA::Material>("New Mat");
    
    const ROA::Color default_col = mat.get_color();
    const ROA::Color expected_col = ROA::Color(0xFFFFFFFF);
    
    REQUIRE(default_col == expected_col);
  }
  
  
  SECTION("Color")
  {
    ROA::Material mat = ROA::Resource::create<ROA::Material>("Color");
    
    const ROA::Color new_color(0xFF00FF00);
    
    mat.set_color(new_color);
    
    const ROA::Color get_color(mat.get_color());
    
    REQUIRE(new_color == get_color);
  }
  
  
  SECTION("Shader")
  {
    /*
      Pending shader has a) a default, and b) allows sinking of new shaders.
    */
  }
  
  
  // ---------------------------------------------------------- [ Inherited ] --

  
  SECTION("Operators")
  {
    ROA::Material rsrc_1 = ROA::Resource::create<ROA::Material>("cmp1");
    ROA::Material rsrc_2 = ROA::Resource::create<ROA::Material>("cmp2");
    ROA::Material rsrc_3 = ROA::Resource::find<ROA::Material>("cmp1");
    ROA::Material rsrc_4 = ROA::Resource::find<ROA::Material>("doesn't exist");
    
    REQUIRE(rsrc_1 != rsrc_2);
    REQUIRE(rsrc_1 == rsrc_3);
    REQUIRE(!!rsrc_4 == false);
  }
}
