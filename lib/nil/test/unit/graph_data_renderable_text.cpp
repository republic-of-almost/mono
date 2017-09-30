#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/renderable_text.hpp>


TEST_CASE("Data - Renderable Text")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);

  Nil::Node node;
  
  SECTION("Initial")
  {
    size_t count = 0;
    Nil::Data::Renderable_text *text = nullptr;
    Nil::Data::get(&count, &text);

    REQUIRE(count == 0);
    REQUIRE(Nil::Data::has_renderable_text(node) == false);
  }
  
  SECTION("Set / Get")
  {
    // Set
    {
      Nil::Data::Renderable_text text{};
      text.font_id = 123;
      text.text = "howdy";
      
      Nil::Data::set(node, text);
      
      REQUIRE(Nil::Data::has_renderable_text(node));
    }

    // Get
    {
      Nil::Data::Renderable_text text{};
      Nil::Data::get(node, text);
      
      REQUIRE(text.font_id == 123);
      REQUIRE(strcmp(text.text, "howdy") == 0);
    }
    
    // Check
    {
      size_t count = 0;
      Nil::Data::Renderable_text *data = nullptr;
      Nil::Data::get(&count, &data);
    
      REQUIRE(count == 1);
      REQUIRE(Nil::Data::has_renderable_text(node) == true);
    }
  }
  
  SECTION("Output")
  {
    // World mat test
  }
  
  SECTION("Remove by Destroy")
  {
    // Set
    {
      Nil::Data::Renderable_text text{};
      Nil::Data::set(node, text);
    
      node.destroy();
    
      size_t count = 0;
      Nil::Data::Renderable_text *data = nullptr;
      Nil::Data::get(&count, &data);
    
      REQUIRE(count == 0);
      REQUIRE(Nil::Data::has_renderable_text(node) == false);
    }
  }
  
  nil_ctx_destroy(&ctx);
}
