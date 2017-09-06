#include <roa/object.hpp>
#include <common/common.hpp>
#include <nil/node.hpp>
#include <nil/resource/directory.hpp>
#include <nil/resource/scene.hpp>
#include <nil/resource/model.hpp>


namespace ROA {
namespace Model {


bool
load(const char *filename)
{
  Nil::Node node(nullptr);
  
  return Nil::Resource::Model::load(filename);
}


} // ns


namespace Scene {


bool
load(ROA::Object obj, const char *filename)
{
  Nil::Node node = ROA_detail::get_node(obj);
  const char *path = Nil::Resource::directory(filename);
  
  return Nil::Resource::Scene::load(node, path);
}


} // ns
} // ns
