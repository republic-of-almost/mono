#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/bounding_box.hpp>


TEST_CASE("Data - Bounding box")
{
  Nil::Engine nil_engine;
  Nil::Node node;
  
  SECTION("Initial")
  {
    size_t count = 0;
    Nil::Data::Bounding_box *data = nullptr;
    Nil::Data::get(&count, &data);

    REQUIRE(count == 0);
    REQUIRE(Nil::Data::has_bounding_box(node) == false);
  }
  
  SECTION("Set / Get")
  {
    // Set
    {
      Nil::Data::Bounding_box box{};
      
      Nil::Data::set(node, box);
      
      REQUIRE(Nil::Data::has_bounding_box(node) == true);
    }

    // Get
    {
      Nil::Data::Bounding_box box{};
      Nil::Data::get(node, box);
    }
    
    // Check
    {
      size_t count = 0;
      Nil::Data::Bounding_box *box = nullptr;
      Nil::Data::get(&count, &box);
    
      REQUIRE(count == 1);
      REQUIRE(Nil::Data::has_bounding_box(node) == true);
    }
  }
  
  SECTION("Remove by Destroy")
  {
    // Set
    {
      Nil::Data::Bounding_box box{};
      Nil::Data::set(node, box);
    
      node.destroy();
    
      size_t count = 0;
      Nil::Data::Bounding_box *data = nullptr;
      Nil::Data::get(&count, &data);
    
      REQUIRE(count == 0);
      REQUIRE(Nil::Data::has_bounding_box(node) == false);
    }
  }
}
