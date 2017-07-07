#include <nil/resource/scene.hpp>
#include <nil/resource/texture.hpp>
#include <nil/resource/material.hpp>
#include <nil/resource/mesh.hpp>
#include <nil/data/renderable.hpp>
#include <nil/node.hpp>
#include <lib/model.hpp>
#include <lib/logging.hpp>
#include <lib/file.hpp>
#include <stdio.h>


namespace Nil {
namespace Resource {
namespace Scene {


bool
load(Nil::Node &node, const char *filename)
{
  // -- Param Check -- //

  if(!lib::file::exists(filename))
  {
    LOG_ERROR("Can't find file to load");
    return false;
  }
  
  if(!node.is_valid())
  {
    return false;
  }
  
  // -- Load Model -- //
  
  const lib::model model = lib::model_import::load_obj_from_file(filename);

  // -- Load Meshes -- //
  
  for(size_t i = 0; i < model.mesh_count; ++i)
  {
    Nil::Resource::Mesh mesh{};
    
    mesh.position_vec3       = model.verts[i];
    mesh.normal_vec3         = model.normals[i];
    mesh.texture_coords_vec2 = model.uvs[i];
    mesh.count               = model.vertex_count[i];
    
    Nil::Resource::load(model.name[i], mesh);
  }
  
  // -- Load Textures -- //
  
  for(size_t i = 0; i < model.material_count; ++i)
  {
    for(size_t j = 0; j < model.mesh_material[i].map_count; ++j)
    {
      Nil::Resource::Texture texture{};
      
      texture.data_type = Nil::Resource::Texture::FILENAME;
      texture.data      = (uintptr_t)model.mesh_material[i].map_path[j];
      texture.data_size = strlen(model.mesh_material[i].map_path[j]);
      
      Nil::Resource::load(model.mesh_material[i].map_path[j], texture);
    }
  }
  
  // -- Load Materials -- //
  
  for(size_t i = 0; i < model.material_count; ++i)
  {
    lib::material *mesh_mat = &model.mesh_material[i];
    
    Nil::Resource::Material mat{};
    
    mat.shader_type = Nil::Resource::Material::FULLBRIGHT;
    mat.color = 0xFF000000;
    
    Nil::Resource::Texture tex{};
    Nil::Resource::find_by_name(mesh_mat->map_path[0], tex);
    
    mat.texture_01 = 1;//tex.id;
    
    Nil::Resource::load(model.mesh_material[i].name, mat);
  }
  
  // -- Create Nodes and Renderables -- //
  
  for(size_t i = 0; i < model.mesh_count; ++i)
  {
    Nil::Node child_node;
    child_node.set_parent(node);
    child_node.set_name(model.name[i]);
    
    const char *mesh_name = model.name[i];
    
    Nil::Resource::Mesh mesh{};
    Nil::Resource::find_by_name(mesh_name, mesh);
    
    Nil::Resource::Material mat{};
    const int32_t mat_id = model.material_id[i];
    if(mat_id > -1)
    {
      Nil::Resource::find_by_name(model.mesh_material[mat_id].name, mat);
    }
    
    Nil::Data::Renderable renderable{};
    renderable.material_id = mat.id;
    renderable.mesh_id = mesh.id;
    
    Nil::Data::set(child_node, renderable);
  }
  
  return true;
}


} // ns
} // ns
} // ns
