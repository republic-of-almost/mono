#include <data/data.hpp>
#include <nil/data/window.hpp>
#include <nil/data/developer.hpp>
#include <nil/data/mouse.hpp>
#include <nil/data/light.hpp>
#include <nil/data/keyboard.hpp>
#include <nil/data/transform.hpp>
#include <math/vec/vec3.hpp>
#include <lib/array.hpp>


#include <nil/data/texture.hpp>
#include <nil/data/texture_resource.hpp>
#include <nil/data/mesh_resource.hpp>
#include <nil/data/mesh.hpp>
#include <nil/data/renderable.hpp>
#include <assets/cube_mesh.hpp>

#include <lib/model.hpp>
#include <stb/stb_image.h>
#include <math/geometry/misc.hpp>


namespace Game_data {


namespace
{
  lib::array<float> line_info;
}


void
load_assets()
{
  uint32_t texture_id_counter = 0;
  
  Nil::Node assets = get_assets();

  // Load assets
  {
    Nil::Node asset;
    asset.set_parent(assets);
    asset.set_name("Cube Bev");
    
    Nil::Data::Mesh_resource mesh{};

    mesh.id = (uint32_t)Game_asset::CUBE_BEV;

    mesh.position_vec3 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_positions));
    memcpy(mesh.position_vec3, Nil_ext::Mesh::bev_cube_positions, sizeof(Nil_ext::Mesh::bev_cube_positions));

    mesh.normal_vec3 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_normals));
    memcpy(mesh.normal_vec3, Nil_ext::Mesh::bev_cube_normals, sizeof(Nil_ext::Mesh::bev_cube_normals));

    mesh.texture_coords_vec2 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_texture_coords));
    memcpy(mesh.texture_coords_vec2, Nil_ext::Mesh::bev_cube_texture_coords, sizeof(Nil_ext::Mesh::bev_cube_texture_coords));

    mesh.count = Nil_ext::Mesh::bev_cube_mesh_vert_count;

    Nil::Data::set(asset, mesh);
  }
  
  // Load assets
  {
    Nil::Node asset;
    asset.set_parent(assets);
    asset.set_name("Cube");
    
    Nil::Data::Mesh_resource mesh{};

    mesh.id = (uint32_t)Game_asset::CUBE;

    mesh.position_vec3 = (float*)malloc(sizeof(Nil_ext::Mesh::cube_positions));
    memcpy(mesh.position_vec3, Nil_ext::Mesh::cube_positions, sizeof(Nil_ext::Mesh::cube_positions));

    mesh.normal_vec3 = (float*)malloc(sizeof(Nil_ext::Mesh::cube_normals));
    memcpy(mesh.normal_vec3, Nil_ext::Mesh::cube_normals, sizeof(Nil_ext::Mesh::cube_normals));

    mesh.texture_coords_vec2 = (float*)malloc(sizeof(Nil_ext::Mesh::cube_texture_coords));
    memcpy(mesh.texture_coords_vec2, Nil_ext::Mesh::cube_texture_coords, sizeof(Nil_ext::Mesh::cube_texture_coords));

    mesh.count = Nil_ext::Mesh::cube_mesh_vert_count;

    Nil::Data::set(asset, mesh);
  }
  
  // Load asset

  uint32_t counter = (uint32_t)Game_asset::STATIC;
  
  lib::model model = lib::model_import::load_obj_from_file("/Users/PhilCK/Desktop/rep_of_a/assets/they_never_pay/mesh/static.obj");
  
  {
    Nil::Node asset;
    asset.set_parent(assets);
    asset.set_name("Static");
    
    for(uint32_t i = 0; i < model.mesh_count; ++i)
    {
      Nil::Node child;
      child.set_parent(asset);
      child.set_name(model.name[i]);
      
      Nil::Data::Texture tex_data{};
      
      static int count = 0;
      
      if(model.material_id[i] >= 0)
      {
        tex_data.data_type = Nil::Data::Texture::FILENAME;
        tex_data.data = (uintptr_t)model.mesh_material[model.material_id[i]].map_path[0];
        tex_data.data_size = strlen(model.mesh_material[model.material_id[i]].map_path[0]);
        
        Nil::Data::set(child, tex_data);
      
//        ++count;
//        int x = 0;
//        int y = 0;
//        int c = 0;
//        stbi_uc *img_data = nullptr;
//        const char *path = model.mesh_material[model.material_id[i]].map_path[0];
//        
//        stbi_set_flip_vertically_on_load(true);
//        img_data = stbi_load(path, &x, &y, &c, 0);
//        
//        tex_data.data       = img_data;
//        tex_data.id         = ++texture_id_counter;
//        tex_data.dimentions = 2;
//        tex_data.compoents  = c;
//        tex_data.width      = x;
//        tex_data.height     = y;
//        tex_data.sizeof_data = c * x * y * sizeof(char);
//        
//        Nil::Data::set(child, tex_data);
//        
//        stbi_image_free(img_data);
      }
    
      Nil::Data::Mesh_resource mesh{};
      
      mesh.id = ++counter;
      
      mesh.position_vec3       = model.verts[i];
      mesh.normal_vec3         = model.normals[i];
      mesh.texture_coords_vec2 = model.uvs[i];
      mesh.count               = model.vertex_count[i];
      
      Nil::Data::set(child, mesh);
      
      Nil::Data::Mesh mesh_instance{};
      mesh_instance.mesh_id = mesh.id;
      
      Nil::Data::set(child, mesh_instance);
      
      Nil::Data::Renderable mat{};
      mat.shader = Nil::Data::Renderable::LIT;
      mat.color[0]   = 1.f;
      mat.color[1]   = 0.f;
      mat.color[2]   = 0.f;
      mat.color[3]   = 1.f;
      mat.texture_01 = tex_data.id;
      mat.mesh_id    = mesh.id;
      Nil::Data::set(child, mat);
    }
  }
  
  lib::model lights = lib::model_import::load_obj_from_file("/Users/PhilCK/Desktop/rep_of_a/assets/they_never_pay/mesh/lights.obj");
  {
    Nil::Node asset;
    asset.set_parent(assets);
    asset.set_name("Lights");
    
    for(uint32_t i = 0; i < lights.mesh_count; ++i)
    {
      Nil::Node child;
      child.set_parent(asset);
      child.set_name(lights.name[i]);
      
      const math::vec3 origin = math::get_vec3_origin(lights.verts[i], lights.triangle_count[i]);
      
      Nil::Data::Transform trans;
      memcpy(trans.position, origin.data, sizeof(trans.position));
      
      Nil::Data::set(child, trans);
      
      Nil::Data::Light light;
      light.atten_const       = 0.1f;
      light.atten_linear      = 0.14f;
      light.atten_exponential = 0.07f;
      
      Nil::Data::set(child, light);
    }
  }
}


void
setup()
{
  // Window
  {
    Nil::Node node = get_app();
    node.set_name("App");
    
    Nil::Data::Window win{};
    constexpr char title[] = "App";
    strcat(win.title, title);
    
    Nil::Data::set(node, win);
  }

  // Debug Lines
  {
    Nil::Node node = get_debug_lines();
    node.set_name("DebugLines");
    
    // Line data
    Nil::Data::Developer line_data;
    line_data.type_id = 2;
    line_data.aux_01 = (uintptr_t)line_info.data();
    line_data.aux_02 = 0;
    
    Nil::Data::set(node, line_data);
  }

  // Mouse
  {
    Nil::Node node = get_mouse();
    node.set_name("Mouse1");
    
    Nil::Data::Mouse ms{};
    ms.id = 0;
    
    Nil::Data::set(node, ms);
  }
  
  // Keyboard
  {
    Nil::Node node = get_keyboard();
    node.set_name("KB1");
    
    Nil::Data::Keyboard kb{};
    kb.id = 0;
    
    Nil::Data::set(node, kb);
  }
  
  // Assets
  {
    Nil::Node node = get_assets();
    node.set_name("Assets");
  }
}


math::vec3
get_world_up()
{
  return math::vec3_init(0,1,0);
}


math::vec3
get_world_left()
{
  return math::vec3_init(1,0,0);
}


math::vec3
get_world_fwd()
{
  return math::vec3_init(0,0,1);
}


Nil::Node
get_app()
{
  static Nil::Node node;
  return node;
}


Nil::Node
get_mouse()
{
  static Nil::Node node;
  return node;
}


Nil::Node
get_keyboard()
{
  static Nil::Node node;
  return node;
}


Nil::Node
get_debug_lines()
{
  static Nil::Node node;
  return node;
}


Nil::Node
get_assets()
{
  static Nil::Node node;
  return node;
}


void
debug_line(const math::vec3 start, const math::vec3 end, const math::vec3 col)
{
  // Line data //
  Nil::Node node = get_debug_lines();
  
  if(node.is_valid() && Nil::Data::has_developer(node))
  {
    Nil::Data::Developer line_data;
    Nil::Data::get(node, line_data);
    
    const size_t count = (size_t)line_data.aux_02;
    
    if(count >= line_info.size())
    {
      line_info.resize(count + 9);
    }

    
    line_info[count + 0] = (math::get_x(start));
    line_info[count + 1] = (math::get_y(start));
    line_info[count + 2] = (math::get_z(start));

    line_info[count + 3] = (math::get_x(end));
    line_info[count + 4] = (math::get_y(end));
    line_info[count + 5] = (math::get_z(end));
    
    line_info[count + 6] = (math::get_x(col));
    line_info[count + 7] = (math::get_y(col));
    line_info[count + 8] = (math::get_z(col));
    
    
    line_data.aux_01 = (uintptr_t)line_info.data();
    line_data.aux_02 += 9;
    
    Nil::Data::set(node, line_data);
  }
}


} // ns
