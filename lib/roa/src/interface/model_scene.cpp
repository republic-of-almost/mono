#include <roa/object.hpp>
#include <common/common.hpp>
#include <common/context.hpp>
#include <nil/node.hpp>
#include <nil/resource/directory.hpp>
#include <nil/resource/scene.hpp>
#include <nil/resource/model.hpp>


namespace {


bool asset_load(Nil::Node node, const char *filename)
{
  const char *path = Nil::Resource::directory(filename);
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  return Nil::Resource::Scene::load(ctx, node, path);
}


} // anon ns


namespace ROA {
namespace Model {


bool
load(const char *filename)
{
  Nil::Node node(nullptr);
  
  return asset_load(node, filename);
}


} // ns


namespace Scene {


bool
load(ROA::Object obj, const char *filename)
{
  Nil::Node node = ROA_detail::get_node(obj);

  return asset_load(node, filename);
}


} // ns
} // ns
