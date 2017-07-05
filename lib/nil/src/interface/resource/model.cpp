#include <nil/resource/model.hpp>
#include <nil/resource/texture.hpp>
#include <nil/resource/material.hpp>
#include <nil/resource/mesh.hpp>
#include <lib/model.hpp>
#include <lib/logging.hpp>
#include <lib/file.hpp>


namespace Nil {
namespace Resource {
namespace Model {


bool
load(const char *filename)
{
  if(!lib::file::exists(filename))
  {
    LOG_ERROR("Can't find file to load");
    return false;
  }

  const lib::model model = lib::model_import::load_obj_from_file(filename);

  // Load meshes
  for(size_t i = 0; i < model.mesh_count; ++i)
  {
    Nil::Resource::Mesh mesh{};
    
    mesh.position_vec3       = model.verts[i];
    mesh.normal_vec3         = model.normals[i];
    mesh.texture_coords_vec2 = model.uvs[i];
    mesh.count               = model.vertex_count[i];
    
    Nil::Resource::load(model.name[i], mesh);
  }
  
  // Load textures
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
  
  // Load materials
  for(size_t i = 0; i < model.material_count; ++i)
  {
    Nil::Resource::Material mat;
    
    
    
    Nil::Resource::load(model.mesh_material[i].name, mat);
  }
  
  return true;
}


} // ns
} // ns
} // ns
