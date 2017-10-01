#include <catch/catch.hpp>
#include <roa/shader.hpp>
#include <roa/resource_status.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA::Shader")
{
  lib::logging::set_output(0);
//  lib::logging::set_output(lib::logging::out::console);


  // --------------------------------------------------- [ Resource Factory ] --


  SECTION("Create Defaults")
  {
    ROA::Shader rsrc = ROA::Resource::create<ROA::Shader>("Fullbright");
    
    REQUIRE(rsrc.is_valid() == true);
    REQUIRE(!!rsrc == true);
    REQUIRE(rsrc.get_id() > 0);
    REQUIRE(rsrc.get_resource_type_id() > 0);
    
    const char *type_name = rsrc.get_resource_type_name();
    const char *expected_type_name = "Shader";
    
    REQUIRE(strcmp(type_name, expected_type_name) == 0);
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
    const ROA::Resource_status before_load = rsrc_load.get_status();
    
    REQUIRE(before_load == ROA::Resource_status::WAITING);
    
    rsrc_load.load();
    
    const ROA::Resource_status after_load = rsrc_load.get_status();
    
    REQUIRE(after_load == ROA::Resource_status::PENDING);
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


  SECTION("Load code")
  {
    ROA::Shader rsrc_load = ROA::Resource::create<ROA::Shader>("Load Code");
    
    const char *vs_code = "vs_code";
    const bool update_vs_pre_load = rsrc_load.set_vertex_shader_code(vs_code);
    REQUIRE(update_vs_pre_load == true);
    const char *got_vs_code = rsrc_load.get_vertex_shader_code();
    REQUIRE(strcmp(got_vs_code, vs_code) == 0);
    
    const char *gs_code = "gs_code";
    REQUIRE(rsrc_load.set_geometry_shader_code(gs_code) == true);
    const char *got_gs_code = rsrc_load.get_geometry_shader_code();
    REQUIRE(strcmp(got_gs_code, gs_code) == 0);
    
    const char *fs_code = "fs_code";
    REQUIRE(rsrc_load.set_fragment_shader_code(fs_code) == true);
    const char *got_fs_code = rsrc_load.get_fragment_shader_code();
    REQUIRE(strcmp(got_fs_code, fs_code) == 0);
    
    // Shouldn't be able to update code after heap //
    rsrc_load.load();
    
    const char *new_vs_code = "new_vs_code";
    REQUIRE(rsrc_load.set_vertex_shader_code(new_vs_code) == false);
    const char *got_old_vs_code = rsrc_load.get_vertex_shader_code();
    REQUIRE(strcmp(got_old_vs_code, vs_code) == 0);
    
    const char *new_gs_code = "new_gs_code";
    REQUIRE(rsrc_load.set_geometry_shader_code(new_gs_code) == false);
    const char *got_old_gs_code = rsrc_load.get_geometry_shader_code();
    REQUIRE(strcmp(got_old_gs_code, gs_code) == 0);
    
    const char *new_fs_code = "new_fs_code";
    REQUIRE(rsrc_load.set_fragment_shader_code(new_fs_code) == false);
    const char *got_old_fs_code = rsrc_load.get_fragment_shader_code();
    REQUIRE(strcmp(got_old_fs_code, fs_code) == 0);
  }
  
  
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
