#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>


TEST_CASE("Data")
{
  Nil::Engine nil_engine;
  
  SECTION("Camera - Remove by Destroy")
  {
    Nil::Node node;
    
    // Check
    {
      size_t count = 0;
      Nil::Data::Camera *cam = nullptr;
      Nil::Data::get(&count, &cam);
    
      REQUIRE(count == 0);
      REQUIRE(Nil::Data::has_camera(node) == false);
    }
    
    // Set
    {
      Nil::Data::Camera camera{};
      camera.width = 800;
      camera.height = 480;
      
      Nil::Data::set(node, camera);
    }
    
    // Get
    {
      Nil::Data::Camera camera{};
      Nil::Data::get(node, camera);
      
      REQUIRE(camera.width == 800);
      REQUIRE(camera.height == 480);
    }
    
    // Check
    {
      size_t count = 0;
      Nil::Data::Camera *cam = nullptr;
      Nil::Data::get(&count, &cam);
    
      REQUIRE(count == 1);
      REQUIRE(Nil::Data::has_camera(node) == true);
    }
    
    // Destroy node, should remove data
    node.destroy();
    
    // Check
    {
      size_t count = 0;
      Nil::Data::Camera *cam = nullptr;
      Nil::Data::get(&count, &cam);
    
      REQUIRE(count == 0);
      REQUIRE(Nil::Data::has_camera(node) == false);
    }
  }
}
