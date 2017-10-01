#include <asset_loading/asset_loading.hpp>
#include <nil/resource/mesh.hpp>
#include <nil/resource/texture.hpp>
#include <nil/resource/directory.hpp>
#include <nil/resource/material.hpp>
#include <nil/data/renderable.hpp>
#include <lib/string.hpp>
#include <lib/file.hpp>
#include <lib/logging.hpp>
#include <lib/model.hpp>


namespace Nil {
namespace Assets {


bool
load_obj(Nil_ctx *ctx, Nil::Node node, const char *filename)
{
  // -- File Check -- //
  
  if(!lib::file::exists(filename))
  {
    LOG_ERROR("Can't find file to load");
    return false;
  }
  
  // -- Load Model -- //

  const lib::model model = lib::model_import::load_obj_from_file(filename);

  // -- Load Meshes -- //

  for(size_t i = 0; i < model.mesh_count; ++i)
  {
    Nil::Resource::Mesh mesh{};
    mesh.name                = model.name[i];
    mesh.position_vec3       = model.verts[i];
    mesh.normal_vec3         = model.normals[i];
    mesh.texture_coords_vec2 = model.uvs[i];
    mesh.triangle_count      = model.vertex_count[i];

    Nil::Resource::load(mesh);
  }
  
  // -- Load Textures -- //

  for(size_t i = 0; i < model.material_count; ++i)
  {
    for(size_t j = 0; j < model.mesh_material[i].map_count; ++j)
    {
      Nil_texture texture{};

      const char *path = Nil::Resource::directory(model.mesh_material[i].map_path[j]);
      
      char load_name[2048]{};
      lib::string::filename_from_path(path, load_name, sizeof(load_name), true);

      if(!nil_rsrc_texture_find_by_name(ctx, load_name, &texture))
      {
        texture.name      = load_name;
        texture.data_type = NIL_DATA_FILENAME;
        texture.data      = (uintptr_t)path;
        texture.data_size = strlen(path) + 1;

        const uint32_t id = nil_rsrc_texture_create(ctx, &texture);
        
        if(id)
        {
          nil_rsrc_texture_set_load_status(ctx, id, NIL_RSRC_STATUS_PENDING);
        }
      }
    }

  }

  // -- Load Materials -- //

  for(size_t i = 0; i < model.material_count; ++i)
  {
    lib::material *mesh_mat = &model.mesh_material[i];

    Nil::Resource::Material mat{};
    mat.name = model.mesh_material[i].name;
    mat.color = 0xEEEEFF00;

    Nil_texture tex{};

    // -- Get Texture Name -- //
    char name[2048]{};
    lib::string::filename_from_path(mesh_mat->map_path[0], name, sizeof(name), true);

    if(!nil_rsrc_texture_find_by_name(ctx, name, &tex))
    {
      mat.color = mat.color | 0x000000FF;
    }

    mat.texture_01 = tex.id;

    Nil::Resource::load(mat);
  }

  // -- Create Nodes and Renderables -- //

  if(node)
  {
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
      renderable.material_id  = mat.id;
      renderable.mesh_id      = mesh.id;

      Nil::Data::set(child_node, renderable);
      Nil::Data::set(child_node, mesh.bounding_box);
    }
  }

  return true;
};


} // ns
} // ns
