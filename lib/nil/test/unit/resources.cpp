#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/resource/resource.hpp>


TEST_CASE("Resources")
{
  Nil::Engine nil_engine;
  
  SECTION("Materials")
  {
    // load
    {
      Nil::Resource::Material mat{};
      mat.color = 0xFF0000FF;
      
      Nil::Resource::load("Foo", mat);
      
      REQUIRE(mat.id > 0);
      REQUIRE(mat.color == 0xFF0000FF);
    }
    
    // Find
    {
      Nil::Resource::Material mat{};
      Nil::Resource::find_by_name("Foo", mat);
      
      REQUIRE(mat.color == 0xFF0000FF);
    }
    
    // Get
    {
      size_t count = 0;
      Nil::Resource::Material *mats = nullptr;
      
      Nil::Resource::get(&count, &mats);
      
      REQUIRE(count == 2);
      REQUIRE(mats[1].color == 0xFF0000FF);
    }
  }
}