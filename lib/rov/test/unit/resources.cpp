#include <catch/catch.hpp>
#include <rov/rov.hpp>

TEST_CASE("ROV - Resources")
{
  rov_initialize(rovGraphicsApi_NOP, "./");
  
  SECTION("Create Texture")
  {
    const uint32_t id = rov_createTexture(nullptr, 0, 0, 0, rovPixel_RGB8);
    REQUIRE(id);
  }
  
  SECTION("Create Mesh")
  {
    const uint32_t id = rov_createMesh(nullptr, nullptr, nullptr, 0);
    REQUIRE(id);
  }
  
  SECTION("Create Index")
  {
    const uint32_t id = rov_createIndex(nullptr, 0);
    REQUIRE(id);
  }

  SECTION("Create Lights")
  {
    const uint32_t id = rov_createLights(nullptr, 0);
    REQUIRE(id);
  }
  
  SECTION("Update Lights")
  {
    const uint32_t id = rov_updateLights(0, nullptr, 0);
    REQUIRE(id);
  }

  SECTION("Create Rendertarget")
  {
    const uint32_t id = rov_createRenderTarget(0, 0, rovPixel_RGB8);
    REQUIRE(id);
  }
  
  rov_destroy();
}
