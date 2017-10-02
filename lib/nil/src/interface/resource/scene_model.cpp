#include <nil/resource/scene.hpp>
#include <nil/resource/model.hpp>
#include <nil/node.hpp>
#include <asset_loading/asset_loading.hpp>
#include <lib/file.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>


namespace {


/*
  Loading scene and model is the same with the exception that nodes are
  created for scene, so we funnel both through here.
*/
inline bool
load_assets(Nil_ctx *ctx, Nil::Node node, const char *filename)
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


} // ns


namespace Nil {
namespace Resource {
namespace Model {


bool
load(Nil_ctx *ctx, const char *filename)
{
  return load_assets(ctx, Nil::Node(nullptr), filename);
}


} // ns
} // ns
} // ns


namespace Nil {
namespace Resource {
namespace Scene {


bool
load(Nil_ctx *ctx, Nil::Node &node, const char *filename)
{
  return load_assets(ctx, node, filename);
}


} // ns
} // ns
} // ns
