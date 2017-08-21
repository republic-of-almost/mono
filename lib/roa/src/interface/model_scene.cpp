#include <roa/model.hpp>
#include <roa/object.hpp>
#include <common/common.hpp>
#include <nil/node.hpp>
#include <nil/resource/resource.hpp>
#include <nil/data/data.hpp>
#include <lib/utilities.hpp>
#include <string.h>


namespace {

/*
  Loading scene and model is the same with the exception that nodes are
  created for scene, so we funnel both through here.
*/
inline bool
load_assets(Nil::Node node, const char *filename)
{
  // -- Param Check -- //

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

      const char *path = Nil::Resource::directory(model.mesh_material[i].map_path[j]);

      texture.data_type = Nil::Resource::Texture::FILENAME;
      texture.data      = (uintptr_t)path;
      texture.data_size = strlen(path) + 1;

      Nil::Resource::load(
        model.mesh_material[i].map_path[j],
        texture
      );
    }

  }

  // -- Load Materials -- //

  for(size_t i = 0; i < model.material_count; ++i)
  {
    lib::material *mesh_mat = &model.mesh_material[i];

    Nil::Resource::Material mat{};
    mat.color = 0xEEEEFF00;

    Nil::Resource::Texture tex{};

    // -- Get Texture Name -- //
    char name[2048]{};
    lib::string::filename_from_path(mesh_mat->map_path[0], name, sizeof(name), true);

    if(!Nil::Resource::find_by_name(name, tex))
    {
      mat.color = mat.color | 0x000000FF;
    }

    mat.texture_01 = tex.id;

    Nil::Resource::load(model.mesh_material[i].name, mat);
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
}


} // anon ns



namespace ROA {
namespace Model {


bool
load(const char *filename)
{
  Nil::Node node(nullptr);
  return load_assets(node, Nil::Resource::directory(filename));
}


} // ns


namespace Scene {


bool
load(ROA::Object obj, const char *filename)
{
  Nil::Node node = ROA_detail::get_node(obj);
  return load_assets(node, Nil::Resource::directory(filename));
}


} // ns
} // ns
