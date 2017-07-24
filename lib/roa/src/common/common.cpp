#include <common/common.hpp>
#include <roa/object.hpp>
#include <nil/nil.hpp>


namespace ROA_detail {


Nil::Engine&
get_engine()
{
  static Nil::Engine engine;
  return engine;
}


Nil::Node
get_application_node()
{
  static Nil::Node node;
  return node;
}


Nil::Node
get_node(ROA::Object obj)
{
  return Nil::Node(obj.get_instance_id());
};


} // ns
