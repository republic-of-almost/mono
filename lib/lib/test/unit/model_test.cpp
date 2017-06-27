//#include <catch/catch.hpp>
//#include <lib/model.hpp>
//#include <stdint.h>
//
//
//TEST_CASE("Model Test")
//{
//  SECTION("Init Raw With No Data")
//  {
//    lib::model m = lib::model_import::from_raw(nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0);
//    
//    REQUIRE(m.vertices == nullptr);
//    REQUIRE(m.vertex_count == 0);
//    
//    REQUIRE(m.normals == nullptr);
//    REQUIRE(m.normal_count == 0);
//    
//    REQUIRE(m.texture_coords == nullptr);
//    REQUIRE(m.texture_coord_count == 0);
//    
//    REQUIRE(m.triangle_index == nullptr);
//    REQUIRE(m.triangle_index_count == 0);
//  }
//  
//  SECTION("Init Raw With Data")
//  {
//    const char *foo[] = {
//      "aaabbb",
//      "bbbcc",
//    };
//  
//    const float verts[] = {1,2,3};
//    const float norms[] = {4,5,6,7};
//    const float tex_c[] = {8,9,10,11,12};
//    
//    const uint32_t index_a[] = {1,2,3};
//    const uint32_t index_b[] = {4,5,6};
//    const uint32_t *index[] = {index_a, index_b};
//  
//    lib::model m = lib::model_import::from_raw(
//      verts,
//      sizeof(verts) / sizeof(float),
//      norms,
//      sizeof(norms) / sizeof(float),
//      tex_c,
//      sizeof(tex_c) / sizeof(float),
//      index,
//      2
//    );
//    
//    REQUIRE(m.vertices[0] == 1.f);
//    REQUIRE(m.vertices[1] == 2.f);
//    REQUIRE(m.vertices[2] == 3.f);
//    REQUIRE(m.vertex_count == 3);
//    
//    REQUIRE(m.normals[0] == 4.f);
//    REQUIRE(m.normals[1] == 5.f);
//    REQUIRE(m.normals[2] == 6.f);
//    REQUIRE(m.normals[3] == 7.f);
//    REQUIRE(m.normal_count == 4);
//    
//    REQUIRE(m.texture_coords[0] == 8.f);
//    REQUIRE(m.texture_coords[1] == 9.f);
//    REQUIRE(m.texture_coords[2] == 10.f);
//    REQUIRE(m.texture_coords[3] == 11.f);
//    REQUIRE(m.texture_coords[4] == 12.f);
//    REQUIRE(m.texture_coord_count == 5);
//    
////    REQUIRE(m.triangle_index[0][0] == 1);
////    REQUIRE(m.triangle_index[0][1] == 2);
////    REQUIRE(m.triangle_index[0][2] == 3);
////    REQUIRE(m.triangle_index[1][0] == 4);
////    REQUIRE(m.triangle_index[1][1] == 5);
////    REQUIRE(m.triangle_index[1][2] == 6);
////    REQUIRE(m.triangle_index_count == 2);
//  }
//}
