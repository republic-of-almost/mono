#ifndef COMMON_INCLUDED_F1353393_9B85_497B_BC09_2A06241FB648
#define COMMON_INCLUDED_F1353393_9B85_497B_BC09_2A06241FB648


#include <roa/fundamental.hpp>
#include <roa/object.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>


namespace ROA_detail {


Nil::Engine&
get_engine();


Nil::Node
get_application_node();


Nil::Node
get_node(ROA::Object obj);


template<typename T>
void
get_nil_data(ROA::Object obj, T &out_data)
{
  Nil::Node node = ROA_detail::get_node(obj);

  if(Nil::Data::has(node, out_data))
  {
    Nil::Data::get(node, out_data);
  }
  else
  {
    LIB_ASSERT(false);
    LOG_ERROR("Failed to find data");
  }
}


template<typename T>
void
set_nil_data(ROA::Object obj, T &in_data)
{
  Nil::Node node = ROA_detail::get_node(obj);

  if(Nil::Data::has(node, in_data))
  {
    Nil::Data::set(node, in_data);
  }
  else
  {
    LIB_ASSERT(false);
    LOG_ERROR("Failed to find data");
  }
}


} // ns


#endif // inc guard
