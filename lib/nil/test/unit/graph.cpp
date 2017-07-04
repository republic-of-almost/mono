#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>


TEST_CASE("Graph")
{
  Nil::Engine nil_engine;
  
  SECTION("Basic RTTI")
  {
    REQUIRE(nil_engine.graph_data_count() == 0);
    
    // Scoped
    {
      Nil::Node node;
      REQUIRE(nil_engine.graph_data_count() == 1);
    }
    
    REQUIRE(nil_engine.graph_data_count() == 0);
  }
  
  SECTION("Null Node")
  {
    Nil::Node node(nullptr);
    REQUIRE(nil_engine.graph_data_count() == 0);
  }
  
  SECTION("Child")
  {
    Nil::Node parent;
    REQUIRE(nil_engine.graph_data_count() == 1);
    Nil::Node child;
    REQUIRE(nil_engine.graph_data_count() == 2);
    
    child.set_parent(parent);
    REQUIRE(nil_engine.graph_data_count() == 2);
  }
}
