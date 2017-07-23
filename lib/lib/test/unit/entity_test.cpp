#include <catch/catch.hpp>
#include <lib/utilities.hpp>


TEST_CASE("entity")
{
  SECTION("Type Instance")
  {
    const uint32_t entity = lib::entity::create(1, 2);
    REQUIRE(lib::entity::type(entity) == 1);
    REQUIRE(lib::entity::instance(entity) == 2);
  
    const uint32_t limits = lib::entity::create(0xFF - 1, 0xFFFFFF - 1);
    REQUIRE(lib::entity::type(limits) == 0xFF - 1);
    REQUIRE(lib::entity::instance(limits) == 0xFFFFFF - 1);
  }

  SECTION("To -> From Pointers")
  {
    const uint32_t entity_a = lib::entity::create(1,2);
    const uint32_t entity_b = lib::entity::create(0xFF - 1, 0xFFFFFF - 1);

    const uintptr_t int_ptr_a = lib::entity::to_int_ptr(entity_a);
    const uintptr_t int_ptr_b = lib::entity::to_int_ptr(entity_b);

    REQUIRE(entity_a == lib::entity::from_ptr(int_ptr_a));
    REQUIRE(entity_b == lib::entity::from_ptr(int_ptr_b));

    const void * ptr_a = lib::entity::to_void_ptr(entity_a);
    const void * ptr_b = lib::entity::to_void_ptr(entity_b);

    REQUIRE(entity_a == lib::entity::from_ptr(ptr_a));
    REQUIRE(entity_b == lib::entity::from_ptr(ptr_b));
  }
}
