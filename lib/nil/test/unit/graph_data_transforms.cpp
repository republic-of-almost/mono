#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <math/math.hpp>


TEST_CASE("Graph Data Transforms")
{
  Nil::Engine nil_engine;
  
  SECTION("Sensible Default Transform")
  {
    const Nil::Node node;
    
    REQUIRE(Nil::Data::has_transform(node)); // Should exist by default.
    
    Nil::Data::Transform trans;
    Nil::Data::get(node, trans);
    
    const math::vec3 pos   = math::vec3_init(trans.position);
    const math::vec3 scale = math::vec3_init(trans.scale);
    const math::quat rot   = math::quat_init(trans.rotation);
    
    REQUIRE(math::vec3_is_near(pos,   math::vec3_zero()));
    REQUIRE(math::vec3_is_near(scale, math::vec3_one()));
    REQUIRE(math::quat_is_near(rot,   math::quat_init()));
  }
  
  SECTION("Simple Inherited World Position")
  {
    Nil::Node parent;
    Nil::Node child;
    Nil::Node gran_child;
    
    child.set_parent(parent);
    gran_child.set_parent(child);
  
    float move_to[] = {0.f, 1.f, 0.f};
    
    // Move Child
    {
      Nil::Data::Transform child_trans{};
      Nil::Data::get(child, child_trans);
      
      memcpy(child_trans.position, move_to, sizeof(child_trans.position));
      Nil::Data::set(child, child_trans);
      
      Nil::Data::Transform gran_world_trans{};
      Nil::Data::get(gran_child, gran_world_trans);
      
      float expected_01[] {0.f, 1.f, 0.f};
      
      REQUIRE(
        math::vec3_is_near(
          math::vec3_init(expected_01),
          math::vec3_init(gran_world_trans.world_position)
        )
      );
    }
    
    // Move Parent
    {
      Nil::Data::Transform parent_trans{};
      Nil::Data::get(parent, parent_trans);
      
      memcpy(parent_trans.position, move_to, sizeof(parent_trans.position));
      Nil::Data::set(parent, parent_trans);
      
      float expected_02[] {0.f, 2.f, 0.f};
      
      Nil::Data::Transform child_world_trans{};
      Nil::Data::get(child, child_world_trans);
      
      REQUIRE(
        math::vec3_is_near(
          math::vec3_init(expected_02),
          math::vec3_init(child_world_trans.world_position)
        )
      );
      
      Nil::Data::Transform gran_world_trans;
      Nil::Data::get(gran_child, gran_world_trans);
      
      float expected_03[] {0.f, 2.f, 0.f};
      
      REQUIRE(
        math::vec3_is_near(
          math::vec3_init(expected_03),
          math::vec3_init(gran_world_trans.world_position)
        )
      );
    }
  }
}
