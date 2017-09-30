#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>


TEST_CASE("Data - Camera")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);

  Nil::Node node;
  
  SECTION("Initial")
  {
    size_t count = 0;
    Nil::Data::Camera *cam = nullptr;
    Nil::Data::get(&count, &cam);

    REQUIRE(count == 0);
    REQUIRE(Nil::Data::has_camera(node) == false);
  }
  
  SECTION("Set / Get")
  {
    // Set
    {
      Nil::Data::Camera camera{};
      camera.type         = Nil::Data::Camera::ORTHOGRAPHIC;
      camera.priority     = 2;
      camera.width        = 0.5f;
      camera.height       = 1.f;
      camera.fov          = 0.1234f;
      camera.near_plane   = 0.1f;
      camera.far_plane    = 1000.f;
      camera.clear_color_buffer = true;
      camera.clear_depth_buffer = true;
      
      Nil::Data::set(node, camera);
      
      REQUIRE(Nil::Data::has_camera(node));
    }

    // Get
    {
      Nil::Data::Camera camera{};
      Nil::Data::get(node, camera);
      
      REQUIRE(camera.type == Nil::Data::Camera::ORTHOGRAPHIC);
      REQUIRE(camera.priority == 2);
      REQUIRE(camera.width == 0.5f);
      REQUIRE(camera.height == 1.f);
      REQUIRE(camera.fov == 0.1234f);
      REQUIRE(camera.near_plane == 0.1f);
      REQUIRE(camera.far_plane == 1000.f);
      REQUIRE(camera.clear_color_buffer == true);
      REQUIRE(camera.clear_depth_buffer == true);
    }
    
    // Check
    {
      size_t count = 0;
      Nil::Data::Camera *cam = nullptr;
      Nil::Data::get(&count, &cam);
    
      REQUIRE(count == 1);
      REQUIRE(Nil::Data::has_camera(node) == true);
    }
  }
  
  SECTION("Output")
  {
    // Set camera, and changed tranform
    {
      Nil::Data::Camera cam{};
      Nil::Data::set(node, cam);
      
      Nil::Data::Transform trans{};
      Nil::Data::get(node, trans);
      
      trans.position[0] = 1.f;
      trans.position[1] = 2.f;
      trans.position[2] = 3.f;
      
      Nil::Data::set(node, trans);
    }
    
    // Check camera has correct eye position
    {
      Nil::Data::Camera cam{};
      Nil::Data::get(node, cam);
      
      REQUIRE(cam.position[0] == 1.f);
      REQUIRE(cam.position[1] == 2.f);
      REQUIRE(cam.position[2] == 3.f);
    }
  }
  
  SECTION("Remove by Destroy")
  {
    // Set
    {
      Nil::Data::Camera camera{};
      Nil::Data::set(node, camera);
    
      node.destroy();
    
      size_t count = 0;
      Nil::Data::Camera *cam = nullptr;
      Nil::Data::get(&count, &cam);
    
      REQUIRE(count == 0);
      REQUIRE(Nil::Data::has_camera(node) == false);
    }
  }
  
  nil_ctx_destroy(&ctx);
}
