#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/audio.hpp>


TEST_CASE("Data - Audio")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);

  Nil::Node node;
  
  SECTION("Initial")
  {
    size_t count = 0;
    Nil::Data::Audio *data = nullptr;
    Nil::Data::get(&count, &data);
    
    REQUIRE(count == 0);
    REQUIRE(Nil::Data::has_audio(node) == false);
  }
  
  SECTION("Set / Get")
  {
    // Set
    {
      Nil::Data::Audio data{};
      
      Nil::Data::set(node, data);
      
      REQUIRE(Nil::Data::has_audio(node) == true);
    }

    // Get
    {
      Nil::Data::Audio data{};
      Nil::Data::get(node, data);
    }
    
    // Check
    {
      size_t count = 0;
      Nil::Data::Audio *data = nullptr;
      Nil::Data::get(&count, &data);
    
      REQUIRE(count == 1);
      REQUIRE(Nil::Data::has_audio(node) == true);
    }
  }
  
  SECTION("Remove by Destroy")
  {
    // Set
    {
      Nil::Data::Audio audio{};
      Nil::Data::set(node, audio);
    	
      node.destroy();
    
      size_t count = 0;
      Nil::Data::Audio *data = nullptr;
      Nil::Data::get(&count, &data);
    
      REQUIRE(count == 0);
      REQUIRE(Nil::Data::has_audio(node) == false);
    }
  }
  
  nil_ctx_destroy(&ctx);
}
