/*
  Fonts disabled
*/
//#include <catch/catch.hpp>
//#include <nil/nil.hpp>
//#include <nil/resource/font.hpp>
//#include <lib/logging.hpp>
//
//
//TEST_CASE("Resource - Font")
//{
//  Nil_ctx *ctx;
//  nil_ctx_initialize(&ctx);
//  
//  SECTION("Initial")
//  {
//    REQUIRE(Nil::Resource::font_count() == 1);
//    REQUIRE(strcmp(Nil::Resource::get_type_name(Nil::Resource::Font{}), "Font") == 0);
//  }
//  
//  SECTION("Load Pass")
//  {
//    Nil::Resource::Font data{};
//    data.name = "my_title_font";
//    
//    const char *filename = "some_filename";
//    data.data = (uintptr_t)filename;
//    data.data_size = strlen(filename) + 1;
//    data.data_type = Nil::Resource::Font::FILENAME;
//    
//    const bool loaded = Nil::Resource::load(data);
//    
//    REQUIRE(loaded == true);
//    REQUIRE(Nil::Resource::font_count() == 2);
//    REQUIRE(data.id > 0);
//  }
//  
//  SECTION("Load fail")
//  {
//    Nil::Resource::Font data{};
//    data.name = "font_to_fail";
//    
//    const char *filename = "some_other_filename";
//    data.data = (uintptr_t)filename;
//    data.data_size = strlen(filename) + 1;
//    data.data_type = Nil::Resource::Font::FILENAME;
//    
//    const bool should_pass = Nil::Resource::load(data);
//    const bool should_fail = Nil::Resource::load(data);
//    
//    REQUIRE(should_pass == true);
//    REQUIRE(should_fail == false);
//  }
//  
//  nil_ctx_destroy(&ctx);
//}
