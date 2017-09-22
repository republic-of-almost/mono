#include <catch/catch.hpp>
#include <math/geometry/ray.hpp>

TEST_CASE("Ray")
{
  // --------------------------------------------------------- [ Initialize ] --
  
  SECTION("Init Blank")
  {
    const math::ray r = math::ray_init();
    
    REQUIRE(math::vec3_is_near(r.start, math::vec3_zero()));
    REQUIRE(math::vec3_is_near(r.end, math::vec3_zero()));
  }

  SECTION("Init Raw Array")
  {
    const float start[] { 0.f,0.f,0.f};
    const float end[] {1.f,1.f,1.f};
    
    const math::ray r = math::ray_init(start, &end[0]);
    
    REQUIRE(math::vec3_is_near(r.start, math::vec3_zero()));
    REQUIRE(math::vec3_is_near(r.end, math::vec3_one()));
  }
  
  SECTION("Init Vec3")
  {
    const math::ray r = math::ray_init(math::vec3_zero(), math::vec3_one());
  
    REQUIRE(math::vec3_is_near(r.start, math::vec3_zero()));
    REQUIRE(math::vec3_is_near(r.end, math::vec3_one()));
  }
  
  // --------------------------------------------------------- [ Operations ] --
  
  SECTION("Inverse")
  {
    const math::ray r    = math::ray_init(math::vec3_zero(), math::vec3_one());
    const math::ray ir   = math::ray_inverse(r);
    const math::vec3 dir = math::ray_direction(ir);
    const math::vec3 nor = math::vec3_normalize(math::vec3_init(-1.f));
    
    REQUIRE(math::vec3_is_near(dir, nor));
  }
  
  SECTION("Length")
  {
    const math::ray r    = math::ray_init(math::vec3_zero(), math::vec3_one());
    const float length   = math::ray_length(r);
    const float expected = math::root_three();
    
    REQUIRE(math::is_near(length, expected));
  }
  
  SECTION("Direction")
  {
    const math::ray r    = math::ray_init(math::vec3_zero(), math::vec3_one());
    const math::vec3 dir = math::ray_direction(r);
    const math::vec3 nor = math::vec3_normalize(math::vec3_one());
    
    REQUIRE(math::vec3_is_near(dir, nor));
  }
  
  SECTION("Ray From Viewport")
  {
    /*
      Test data needed.
    */
  }
  
  // ----------------------------------------------------------- [ Equality ] --
  
  SECTION("Is Near")
  {
    const math::ray r1 = math::ray_init(math::vec3_zero(), math::vec3_one());
    const math::ray r2 = math::ray_init(math::vec3_zero(), math::vec3_one());
    const math::ray r3 = math::ray_init(math::vec3_one(), math::vec3_one());
    
    REQUIRE(math::ray_is_near(r1, r2) == true);
    REQUIRE(math::ray_is_near(r1, r3) == false);
  }
  
  // --------------------------------------------------------------- [ Test ] --
  
  SECTION("Test AABB")
  {
    /*
      Test data needed.
    */
  }
  
  SECTION("Test Plane")
  {
    /*
      Test data needed.
    */
  }
  
  SECTION("Test Triangles")
  {
    /*
      Test data needed.
    */
  }
  
  SECTION("Find Edge")
  {
    /*
      Test data needed.
    */
  }
}
