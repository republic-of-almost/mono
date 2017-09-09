#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>


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
  
  SECTION("Init a Null Node")
  {
    Nil::Node node(nullptr);
    
    // Moves
    node = static_cast<Nil::Node&&>(Nil::Node());
    
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
  
  SECTION("Try to move a ref")
  {
    Nil::Node owner;
    Nil::Node ref(owner);
    
    REQUIRE(owner.is_ref() == false);
    REQUIRE(ref.is_ref() == true);
    
    Nil::Node try_to_move(nullptr);
    REQUIRE(try_to_move.is_valid() == false);
    
    try_to_move = static_cast<Nil::Node&&>(ref);
    
    REQUIRE(try_to_move.is_valid() == false);
    REQUIRE(ref.is_valid() == true);
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
  
  SECTION("Add Tags")
  {
    Nil::Node node;
    
    REQUIRE(node.get_tag_count() == 0);
    REQUIRE(node.add_tag("FOO"));
    REQUIRE(node.get_tag_count() == 1);
    REQUIRE(node.add_tag("BAR"));
    REQUIRE(node.get_tag_count() == 2);
    
    node.clear_tags();
    
    REQUIRE(node.get_tag_count() == 0);
  }
  
  SECTION("Has Tag")
  {
    Nil::Node node;
    
    REQUIRE(node.add_tag("FOO"));
    REQUIRE(node.has_tag("FOO") == true);
    REQUIRE(node.has_tag("ZIP") == false);
  }
  
  SECTION("Get Tag")
  {
    Nil::Node node;
    
    node.add_tag("FOO");
    node.add_tag("BAR");
    
    REQUIRE(strcmp(node.get_tag(0), "FOO") == 0);
    REQUIRE(strcmp(node.get_tag(1), "BAR") == 0);
  }
}
