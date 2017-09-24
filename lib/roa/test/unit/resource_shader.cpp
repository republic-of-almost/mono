#include <catch/catch.hpp>
#include <roa/shader.hpp>
#include <roa/resource_status.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA::Shader")
{
  lib::logging::set_output(0);


  // --------------------------------------------------- [ Resource Factory ] --


  SECTION("Create Defaults")
  {
    ROA::Shader rsrc = ROA::Resource::create<ROA::Shader>("Fullbright");
    
    REQUIRE(rsrc.is_valid() == true);
    REQUIRE(!!rsrc == true);
    REQUIRE(rsrc.get_id() > 0);
    REQUIRE(rsrc.get_resource_type_id() > 0);
    REQUIRE(rsrc.get_resource_type_name() > 0);
    REQUIRE(rsrc.get_status() == ROA::Resource_status::WAITING);
    
    const char *name = rsrc.get_instance_name();
    const char *expected_name = "Fullbright";
    
    REQUIRE(strcmp(name, expected_name) == 0);
  }
  
  
  SECTION("Create Invalid")
  {
    ROA::Shader invalid = ROA::Resource::load<ROA::Shader>("");
    
    REQUIRE(invalid.is_valid() == false);
    REQUIRE(!!invalid == false);
    REQUIRE(invalid.get_id() == 0);
  }
  
  
  SECTION("Load New")
  {
    // This is unsupported anyway
    ROA::Shader new_rsrc = ROA::Resource::load<ROA::Shader>("filepath");
    
    REQUIRE(new_rsrc.is_valid() == false);
  }
  
  
  SECTION("Load Waiting")
  {
    ROA::Shader rsrc_load = ROA::Resource::create<ROA::Shader>("Make Pending");
    
    REQUIRE(rsrc_load.get_status() == ROA::Resource_status::WAITING);
    
    rsrc_load.load();
    
    REQUIRE(rsrc_load.get_status() == ROA::Resource_status::PENDING);
  }
  
  
  SECTION("Find")
  {
    ROA::Shader rsrc_find = ROA::Resource::find<ROA::Shader>("Fullbright");
    
    REQUIRE(rsrc_find.is_valid() == true);
  }
  
  
  SECTION("Find Invalid")
  {
    ROA::Shader rsrc_find = ROA::Resource::find<ROA::Shader>("Unknown");
    
    REQUIRE(rsrc_find.is_valid() == false);
  }
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  /*
    None yet
  */
  
  // ---------------------------------------------------------- [ Inherited ] --
  
  
  SECTION("Operators")
  {
    ROA::Shader rsrc_1 = ROA::Resource::create<ROA::Shader>("cmp1");
    ROA::Shader rsrc_2 = ROA::Resource::create<ROA::Shader>("cmp2");
    ROA::Shader rsrc_3 = ROA::Resource::find<ROA::Shader>("cmp1");
    ROA::Shader rsrc_4 = ROA::Resource::find<ROA::Shader>("doesn't exist");
    
    REQUIRE(rsrc_1 != rsrc_2);
    REQUIRE(rsrc_1 == rsrc_3);
    REQUIRE(!!rsrc_4 == false);
  }
}
