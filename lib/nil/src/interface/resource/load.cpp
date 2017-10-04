#include <nil/resource/load.hpp>
#include <nil/node.hpp>
#include <asset_loading/asset_loading.hpp>
#include <lib/file.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>


inline bool
nil_rsrc_load_model_scene(Nil_ctx *ctx, Nil::Node node, const char *filename)
{
  // -- Param Check -- //

  if(!lib::file::exists(filename))
  {
    LOG_ERROR("Can't find file to load");
    return false;
  }
  
  // -- File type -- //
  
  if(lib::file::has_extension(filename, ".gltf"))
  {
    return Nil::Assets::load_gltf(ctx, node, filename);
  }
  else if(lib::file::has_extension(filename, ".obj"))
  {
    return Nil::Assets::load_obj(ctx, node, filename);
  }
  else
  {
    LOG_ERROR("Unknown filetype");
    return false;
  }
}



bool
nil_rsrc_load_model(Nil_ctx *ctx, const char *filename)
{
  nil_rsrc_load_model_scene(ctx, Nil::Node(nullptr), filename);
}


bool
nil_rsrc_load_scene(Nil_ctx *ctx, Nil::Node &node, const char *filename)
{
  nil_rsrc_load_model_scene(ctx, node, filename);
}

