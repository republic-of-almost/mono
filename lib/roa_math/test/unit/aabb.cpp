/*
#include <catch/catch.hpp>
#include <math/geometry/aabb.hpp>

TEST_CASE("AABB")
{

  // --------------------------------------------------------- [ Initialize ] --

  SECTION("Init default")
  {
    const math::aabb box = math::aabb_init();
    const math::vec3 extents = math::aabb_get_extents(box);
    
    REQUIRE(math::is_near(math::vec3_length(extents), 0.f));
  }
  
  SECTION("Init with vectors")
  {
    const math::vec3 min = math::vec3_init(1.f);
    const math::vec3 max = math::vec3_init(2.f);
    const math::aabb box = math::aabb_init(min, max);
    
    REQUIRE(math::vec3_is_near(min, math::aabb_get_min(box)));
    REQUIRE(math::vec3_is_near(max, math::aabb_get_max(box)));
  }

  SECTION("Init with origin and size")
  {
    const math::vec3 o = math::vec3_init(0.f);
    const float scale = 3.f;
    const math::aabb box = math::aabb_init(o, scale);
    
    const math::vec3 get_min = math::aabb_get_min(box);
    const math::vec3 get_max = math::aabb_get_max(box);
    
    REQUIRE(math::vec3_is_near(math::vec3_init(-scale * 0.5f), get_min));
    REQUIRE(math::vec3_is_near(math::vec3_init(+scale * 0.5f), get_max));
  }

  SECTION("Init with vertex array")
  {
    const float verts[] {
      -1.f, -1.f, -1.f,
       0.f,  0.f,  0.f,
      +1.f, +1.f, +1.f,
    };
    
    const math::aabb box = math::aabb_init(verts, sizeof(verts) / sizeof(float));
    
    REQUIRE(math::vec3_is_near(math::vec3_init(-1.f), math::aabb_get_min(box)));
    REQUIRE(math::vec3_is_near(math::vec3_init(+1.f), math::aabb_get_max(box)));
  }
  
  // --------------------------------------------------------------- [ Data ] --
  
  SECTION("Extents")
  {
    const float scale = 3.f;
    const math::aabb box = math::aabb_init(math::vec3_zero(), scale);
    const math::vec3 extents = math::aabb_get_extents(box);
    
    REQUIRE(math::is_near(math::get_x(extents), scale));
    REQUIRE(math::is_near(math::get_y(extents), scale));
    REQUIRE(math::is_near(math::get_z(extents), scale));
  }
  
  SECTION("Half extents")
  {
    const float scale = 3.f;
    const math::aabb box = math::aabb_init(math::vec3_zero(), scale);
    const math::vec3 extents = math::aabb_get_half_extents(box);
    
    const float half_scale = scale * 0.5f;
    
    REQUIRE(math::is_near(math::get_x(extents), half_scale));
    REQUIRE(math::is_near(math::get_y(extents), half_scale));
    REQUIRE(math::is_near(math::get_z(extents), half_scale));
  }

  SECTION("Min Max")
  {
    const math::vec3 min = math::vec3_init(5.f);
    const math::vec3 max = math::vec3_init(2.f);
    const math::aabb box = math::aabb_init(min, max);
    
    const math::vec3 get_min = math::aabb_get_min(box);
    const math::vec3 get_max = math::aabb_get_max(box);
    
    REQUIRE(math::vec3_is_near(max, get_max));
    REQUIRE(math::vec3_is_near(min, get_min));
  }
  
  // --------------------------------------------------------- [ Operations ] --
  
  SECTION("Combine")
  {
    const math::aabb box_1 = math::aabb_init(math::vec3_zero(), 2.f);
    const math::aabb box_2 = math::aabb_init(math::vec3_zero(), 5.f);
    const math::aabb box_3 = math::aabb_combine(box_1, box_2);
    const math::vec3 extents = math::aabb_get_extents(box_3);
    
    REQUIRE(math::is_near(math::get_x(extents), 5.f));
    REQUIRE(math::is_near(math::get_y(extents), 5.f));
    REQUIRE(math::is_near(math::get_z(extents), 5.f));
  }
  
  SECTION("Scale")
  {
    math::aabb box_1 = math::aabb_init(math::vec3_zero(), 1.f);
    math::aabb_scale(box_1, math::vec3_init(2.f, 3.f, 4.f));
    
    const math::vec3 extents = math::aabb_get_extents(box_1);
    
    REQUIRE(math::is_near(math::get_x(extents), 2.f));
    REQUIRE(math::is_near(math::get_y(extents), 3.f));
    REQUIRE(math::is_near(math::get_z(extents), 4.f));
  }

  SECTION("Origin")
  {
    math::aabb box_1 = math::aabb_init(math::vec3_zero(), 1.f);
    const math::vec3 extents_1 = math::aabb_get_extents(box_1);
    const math::vec3 origin_1 = math::aabb_get_origin(box_1);
    
    REQUIRE(math::vec3_is_near(math::vec3_zero(), origin_1));
    
    math::aabb_set_origin(box_1, math::vec3_one());
    const math::vec3 extents_2 = math::aabb_get_extents(box_1);
    const math::vec3 origin_2 = math::aabb_get_origin(box_1);
    
    REQUIRE(math::vec3_is_near(math::vec3_one(), origin_2));
    REQUIRE(math::vec3_is_near(extents_1, extents_2));
  }

  // --------------------------------------------------------------- [ Test ] --
  
  SECTION("Init")
  {
    const math::aabb box_1 = math::aabb_init(math::vec3_init(1.f), 2.f);
    const math::aabb box_2 = math::aabb_init(math::vec3_init(2.f), 2.f);
    const math::aabb box_3 = math::aabb_init(math::vec3_init(6.f), 2.f);
    
    REQUIRE(math::aabb_intersection_test(box_1, box_2) == true);
    REQUIRE(math::aabb_intersection_test(box_1, box_3) == false);
  }
}
*/

