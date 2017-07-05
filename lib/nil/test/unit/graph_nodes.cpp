#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>


TEST_CASE("Graph Nodes")
{
  Nil::Engine nil_engine;
  
  SECTION("Null Node")
  {
    Nil::Node node(nullptr);
    
    REQUIRE(node.is_valid() == false);
    REQUIRE(node.is_ref() == false);
    REQUIRE(nil_engine.graph_data_count() == 0);
  }
  
  SECTION("Default Node")
  {
    Nil::Node node;
    
    REQUIRE(node.is_valid() == true);
    REQUIRE(node.is_ref() == false);
  }
  
  SECTION("Basic Out of Scope")
  {
    REQUIRE(nil_engine.graph_data_count() == 0);
    
    // Scoped
    {
      Nil::Node node;
      
      REQUIRE(nil_engine.graph_data_count() == 1);
    }
    
    REQUIRE(nil_engine.graph_data_count() == 0);
  }
  
  SECTION("Child")
  {
    {
      Nil::Node parent;
      Nil::Node child;
      Nil::Node gran_child;
      
      child.set_parent(parent);
      gran_child.set_parent(child);
      
      REQUIRE(nil_engine.graph_data_count() == 3);
      
      REQUIRE(parent.is_ref() == false);
      REQUIRE(parent.is_valid() == true);
      REQUIRE(parent.get_parent().is_valid() == false);
      REQUIRE(parent.get_child_count() == 1);
      REQUIRE(parent.get_descendant_count() == 2);
      
      REQUIRE(child.is_ref() == true);
      REQUIRE(child.is_valid() == true);
      REQUIRE(child.get_parent() == parent);
      REQUIRE(child.get_child_count() == 1);
      REQUIRE(child.get_descendant_count() == 1);
      
      REQUIRE(gran_child.is_ref() == true);
      REQUIRE(gran_child.is_valid() == true);
      REQUIRE(gran_child.get_parent() == child);
      REQUIRE(gran_child.get_child_count() == 0);
      REQUIRE(gran_child.get_descendant_count() == 0);

    }
    
    REQUIRE(nil_engine.graph_data_count() == 0);
  }
  
  SECTION("Child in Scope Parent Out of Scope")
  {
    Nil::Node child;
    {
      Nil::Node parent;
      child.set_parent(parent);
      
      REQUIRE(nil_engine.graph_data_count() == 2);
      
      REQUIRE(parent.is_ref() == false);
      REQUIRE(child.is_valid() == true);
      
      REQUIRE(child.is_ref() == true);
      REQUIRE(child.is_valid() == true);
    }
    
    REQUIRE(child.is_ref() == false);
    REQUIRE(child.is_valid() == false);
    
    REQUIRE(nil_engine.graph_data_count() == 0);
  }
  
  SECTION("Manual Destroy")
  {
    Nil::Node node;
    
    REQUIRE(nil_engine.graph_data_count() == 1);
    REQUIRE(node.is_valid() == true);
    
    node.destroy();
    
    REQUIRE(nil_engine.graph_data_count() == 0);
    REQUIRE(node.is_valid() == false);
  }
  
  SECTION("Manual Destroy Parent")
  {
    Nil::Node parent;
    Nil::Node child;
    Nil::Node gran_child;
    
    child.set_parent(parent);
    gran_child.set_parent(child);
    
    REQUIRE(nil_engine.graph_data_count() == 3);
    
    parent.destroy();
    
    REQUIRE(nil_engine.graph_data_count() == 0);
  }
  
  SECTION("Destroy Child")
  {
    Nil::Node parent;
    Nil::Node child;
    Nil::Node gran_child;
    
    child.set_parent(parent);
    gran_child.set_parent(child);
    
    REQUIRE(nil_engine.graph_data_count() == 3);
    
    child.destroy();
    
    REQUIRE(nil_engine.graph_data_count() == 1);
    
    REQUIRE(parent.is_valid() == true);
    REQUIRE(child.is_valid() == false);
    REQUIRE(gran_child.is_valid() == false);
  }
  
  SECTION("Remove Parent")
  {
    Nil::Node parent;
    Nil::Node child;
    Nil::Node gran_child;
    
    child.set_parent(parent);
    gran_child.set_parent(child);
    
    child.set_parent(Nil::Node(nullptr));
    
    REQUIRE(parent.get_descendant_count() == 0);
    REQUIRE(child.get_descendant_count() == 1);
    
    REQUIRE(child.is_valid() == true);
    REQUIRE(child.is_ref() == false); 
  }
}
