#include <common/common.hpp>
#include <roa/object.hpp>


namespace ROA_detail {


Nil::Node
get_node(ROA::Object obj)
{
  return Nil::Node(obj.get_instance_id());
};


} // ns
