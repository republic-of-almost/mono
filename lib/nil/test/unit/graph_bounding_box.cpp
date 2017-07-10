#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <math/math.hpp>


TEST_CASE("Graph Bounding Box")
{
  Nil::Engine nil_engine;
  
  SECTION("Sensible Default Bounding Boxes")
  {
    const Nil::Node node;
    Nil::Data::Bounding_box bbox;
    Nil::Data::get(node, bbox);
    
    const math::vec3 min = math::vec3_init(bbox.min);
    const math::vec3 max = math::vec3_init(bbox.max);
    
    REQUIRE(
      math::vec3_is_near(
        min,
        math::vec3_zero()
      )
    );
    
    REQUIRE(
      math::vec3_is_near(
        max,
        math::vec3_zero()
      )
    );
  }
  
  SECTION("Inhertied Bounding Boxes")
  {
    Nil::Node parent;
    Nil::Node child;
    Nil::Node gran_child;
    
    child.set_parent(parent);
    gran_child.set_parent(child);
    
    // Set the bounding boxes
    {
      Nil::Data::Bounding_box parent_box{
        {-1.f, -1.f, -1.f},
        {+1.f, +1.f, +1.f},
      };
      Nil::Data::set(parent, parent_box);
      
      Nil::Data::Bounding_box child_box{
        {-0.f, -0.f, -0.f},
        {+4.f, +2.f, +3.f},
      };
      Nil::Data::set(child, child_box);
      
      Nil::Data::Bounding_box gran_child_box{
        {-4.f, -3.f, -2.f},
        {+0.f, +0.f, +0.f},
      };
      Nil::Data::set(gran_child, gran_child_box);
    }
    
    {
      
    }
    
//    REQUIRE(false);
  }
}
