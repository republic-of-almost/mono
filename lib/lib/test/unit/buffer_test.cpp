//#include <catch/catch.hpp>
//#include <lib/buffer.hpp>
//
//
//TEST_CASE("Buffer")
//{
//
//  lib::buffer::data somebuffer;
//
//  SECTION("Un-initialized buffer")
//  {
//    REQUIRE(lib::buffer::size(&somebuffer) == 0);
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 0);
//    REQUIRE(lib::buffer::empty(&somebuffer) == true);
//  }
//
//  SECTION("Initialize Buffer")
//  {
//    lib::buffer::init(
//      &somebuffer,
//      sizeof(int),
//      1
//    );
//
//    REQUIRE(lib::buffer::size(&somebuffer) == 0);
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 1);
//    REQUIRE(lib::buffer::empty(&somebuffer) != false);
//  }
//
//  SECTION("Push")
//  {
//    lib::buffer::init(
//      &somebuffer,
//      sizeof(int),
//      1
//    );
//
//    int *array = nullptr;
//
//    lib::buffer::push(&somebuffer);
//    array = (int*)lib::buffer::bytes(&somebuffer);
//    array[0] = 1;
//
//    REQUIRE(lib::buffer::size(&somebuffer) == 1);
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 1);
//
//    lib::buffer::push(&somebuffer);
//    array = (int*)lib::buffer::bytes(&somebuffer);
//    array[1] = 2;
//
//    REQUIRE(lib::buffer::size(&somebuffer) == 2);
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 2);
//
//    lib::buffer::push(&somebuffer);
//    array = (int*)lib::buffer::bytes(&somebuffer);
//    array[2] = 3;
//
//    REQUIRE(lib::buffer::size(&somebuffer) == 3);
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 4);
//
//    REQUIRE(array[0] == 1);
//    REQUIRE(array[1] == 2);
//    REQUIRE(array[2] == 3);
//  }
//
//  SECTION("Resize")
//  {
//    lib::buffer::init(
//      &somebuffer,
//      sizeof(int),
//      1
//    );
//
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 1);
//
//    lib::buffer::resize(&somebuffer, 32);
//
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 32);
//
//    int *array = (int*)lib::buffer::bytes(&somebuffer);
//    for(uint32_t i = 0; i < 32; ++i)
//    {
//      array[i] = i;
//    }
//
//    for(uint32_t i = 0; i < 32; ++i)
//    {
//      REQUIRE(i == array[i]);
//    }    
//
//    lib::buffer::resize(&somebuffer, 64);
//
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 64);
//    array = (int*)lib::buffer::bytes(&somebuffer);
//
//    for(uint32_t i = 0; i < 32; ++i)
//    {
//      REQUIRE(i == array[i]);
//    }
//  }
//
//  SECTION("Reserve")
//  {
//    lib::buffer::init(
//      &somebuffer,
//      sizeof(int),
//      1
//    );
//
//    lib::buffer::reserve(&somebuffer, 32);
//
//    REQUIRE(lib::buffer::capacity(&somebuffer) == 32);
//    REQUIRE(lib::buffer::size(&somebuffer) == 0);
//  }
//
//  SECTION("Insert")
//  {
//    lib::buffer::init(
//      &somebuffer,
//      sizeof(int),
//      4
//    );
//
//    lib::buffer::push(&somebuffer);
//    lib::buffer::push(&somebuffer);
//    lib::buffer::push(&somebuffer);
//
//    int *array = (int*)lib::buffer::bytes(&somebuffer);
//    array[0] = 1;
//    array[1] = 2;
//    array[2] = 3;
//
//    lib::buffer::insert(&somebuffer, 1);
//    array = (int*)lib::buffer::bytes(&somebuffer);
//
//    REQUIRE(array[0] == 1);
//    REQUIRE(array[2] == 2);
//    REQUIRE(array[3] == 3);
//  }
//
//  SECTION("Erase")
//  {
//    lib::buffer::init(
//      &somebuffer,
//      sizeof(int),
//      4
//    );
//
//    lib::buffer::push(&somebuffer);
//    lib::buffer::push(&somebuffer);
//    lib::buffer::push(&somebuffer);
//
//    int *array = (int*)lib::buffer::bytes(&somebuffer);
//    array[0] = 1;
//    array[1] = 2;
//    array[2] = 3;
//
//    lib::buffer::erase(&somebuffer, 1);
//
//    REQUIRE(array[1] == 3);
//  }
//
//  SECTION("clear")
//  {
//    lib::buffer::init(
//      &somebuffer,
//      sizeof(int),
//      4
//    );
//
//    lib::buffer::push(&somebuffer);
//    lib::buffer::push(&somebuffer);
//
//    REQUIRE(lib::buffer::size(&somebuffer) == 2);
//
//    lib::buffer::clear(&somebuffer);
//
//    REQUIRE(lib::buffer::size(&somebuffer) == 0);
//  }
//
//  SECTION("last")
//  {
//    lib::buffer::init(
//      &somebuffer,
//      sizeof(int),
//      4
//    );
//
//    int *array = (int*)lib::buffer::last(&somebuffer);
//
//    REQUIRE(array == nullptr);
//
//    lib::buffer::push(&somebuffer);
//    lib::buffer::push(&somebuffer);
//    lib::buffer::push(&somebuffer);
//    lib::buffer::push(&somebuffer);
//    array = (int*)lib::buffer::last(&somebuffer);
//
//    REQUIRE(array != nullptr);
//
//    array = (int*)lib::buffer::bytes(&somebuffer);
//    array[0] = 1;
//    array[1] = 2;
//    array[2] = 3;
//    array[3] = 4;
//
//    array = (int*)lib::buffer::last(&somebuffer);
//    REQUIRE(*array == 4);
//  }
//}
