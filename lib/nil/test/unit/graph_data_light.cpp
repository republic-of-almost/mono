#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/light.hpp>
#include <nil/data/transform.hpp>


TEST_CASE("Data - Light")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);

  
  Nil::Node node;
  
  SECTION("Initial")
  {
    size_t count = 0;
    Nil::Data::Light *cam = nullptr;
    Nil::Data::get(&count, &cam);

    REQUIRE(count == 0);
    REQUIRE(Nil::Data::has_light(node) == false);
  }
  
  SECTION("Set / Get")
  {
    // Set
    {
      Nil::Data::Light light{};
      light.type              = Nil::Data::Light::POINT;
      light.color[0]          = 0;
      light.color[1]          = 128;
      light.color[2]          = 255;
      light.atten_const       = 1.f;
      light.atten_linear      = 2.f;
      light.atten_exponential = 3.f;
      
      Nil::Data::set(node, light);
      
      REQUIRE(Nil::Data::has_light(node));
    }

    // Get
    {
      Nil::Data::Light light{};
      Nil::Data::get(node, light);

      REQUIRE(light.type == Nil::Data::Light::POINT);
      REQUIRE(light.color[0] == 0);
      REQUIRE(light.color[1] == 128);
      REQUIRE(light.color[2] == 255);
      REQUIRE(light.atten_const == 1.f);
      REQUIRE(light.atten_linear == 2.f);
      REQUIRE(light.atten_exponential == 3.f);
    }
    
    // Check
    {
      size_t count = 0;
      Nil::Data::Light *data = nullptr;
      Nil::Data::get(&count, &data);
    
      REQUIRE(count == 1);
      REQUIRE(Nil::Data::has_light(node) == true);
    }
  }
  
  SECTION("Output")
  {
    // Set camera, and changed tranform
    {
      Nil::Data::Light light{};
      Nil::Data::set(node, light);
      
      Nil::Data::Transform trans{};
      Nil::Data::get(node, trans);
      
      trans.position[0] = 1.f;
      trans.position[1] = 2.f;
      trans.position[2] = 3.f;
      
      Nil::Data::set(node, trans);
    }
    
    // Check camera has correct eye position
    {
      Nil::Data::Light light{};
      Nil::Data::get(node, light);
      
      REQUIRE(light.position[0] == 1.f);
      REQUIRE(light.position[1] == 2.f);
      REQUIRE(light.position[2] == 3.f);
    }
  }
  
  SECTION("Remove by Destroy")
  {
    // Set
    {
      Nil::Data::Light light{};
      Nil::Data::set(node, light);
    
      node.destroy();
    
      size_t count = 0;
      Nil::Data::Light *data = nullptr;
      Nil::Data::get(&count, &data);
    
      REQUIRE(count == 0);
      REQUIRE(Nil::Data::has_light(node) == false);
    }
  }
  
  nil_ctx_destroy(&ctx);
}
