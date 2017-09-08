#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph.hpp>
#include <graph/graph_data.hpp>
#include <lib/entity.hpp>
#include <lib/logging.hpp>


namespace {


constexpr uint32_t node_owned_id = 1;
constexpr uint32_t node_ref_id = 0;

constexpr char node_msg_trying_to_destroy_null_node[] = "Trying to destroy null node";
constexpr char node_msg_invalid_node[] = "Node is invalid";

const size_t node_tag_max_length = 65;
const size_t node_tag_max_count = 64;
size_t node_tag_curr_count = 0;
char node_tag_buffer[node_tag_max_count * node_tag_max_length]{};


} // anon ns


namespace Nil {

namespace lib_ent = lib::entity;


Node::Node()
: m_node_id(lib_ent::create(node_owned_id, Graph::node_create(Data::get_graph_data())))
{
}


Node::Node(const decltype(nullptr))
: m_node_id(lib_ent::create(node_ref_id, 0))
{
}


Node::Node(const uint32_t node_id, const bool is_owning)
: m_node_id(lib_ent::create(is_owning ? node_owned_id : node_ref_id, node_id))
{
}


Node::~Node()
{
  const uint32_t type_id = lib_ent::type(m_node_id);
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(type_id > node_ref_id && instance_id > 0)
  {
    Graph::node_remove(Data::get_graph_data(), instance_id);
  }
}


// ------------------------------------------------------------ [ Operators ] --
  
  
bool
Node::operator==(const Node &other) const
{
  return get_id() == other.get_id();
}


bool
Node::operator!=(const Node &other) const
{
  return get_id() != other.get_id();
}


Node::operator bool() const
{
  return is_valid();
}


// ---------------------------------------------------------- [ Copy / Move ] --


Node::Node(const Node &other)
: m_node_id(other.get_id() ? lib_ent::create(node_ref_id, other.get_id()) : 0)
{
}

  
Node&
Node::operator=(const Node &other)
{
  // If the old one is owning then we need to destroy it
  if(is_valid() && !is_ref())
  {
    destroy();
  }

  m_node_id = other.get_id() ? lib_ent::create(node_ref_id, other.get_id()) : 0;
  
  return *this;
}


Node::Node(Node &&other) noexcept
: m_node_id(!other.is_ref() ? lib_ent::create(node_owned_id, other.get_id()) : lib_ent::create(node_ref_id, other.get_id()))
{
  if(!other.is_ref())
  {
    other.m_node_id = lib_ent::create(node_ref_id, other.get_id());
  }
}

  
Node&
Node::operator=(Node &&other) noexcept
{
  // If the old one is owning then we need to destroy it
  if(is_valid() && !is_ref())
  {
    destroy();
  }
  
  // Refs cannot be moved.
  if(other.is_ref())
  {
    return *this;
  }

  // Clean up this node if its an owning node.
  const uint32_t type_id = lib_ent::type(m_node_id);
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(type_id > node_ref_id && instance_id)
  {
    Graph::node_remove(Data::get_graph_data(), instance_id);
  }

  // Move other node.
  {
    this->m_node_id = lib_ent::create(node_owned_id, other.get_id());
    other.m_node_id = lib_ent::create(node_ref_id, other.get_id());
  }
  
  return *this;
}



// ---------------------------------------------------------------- [ State ] --
  

bool
Node::destroy()
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    return Graph::node_remove(Data::get_graph_data(), instance_id);
  }
  
  LOG_WARNING(node_msg_trying_to_destroy_null_node);
  
  return false;
}


bool
Node::is_ref() const
{
  if(!is_valid())
  {
    return false;
  }
  
  return (lib_ent::type(m_node_id) == node_ref_id);
}


bool
Node::is_top_level() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    return !!Graph::node_get_parent(Data::get_graph_data(), instance_id);
  }
  
  LOG_ERROR(node_msg_invalid_node);
  
  return false;
}


bool
Node::is_valid() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    Graph::Data *graph = Data::get_graph_data();
    
    const bool is_valid = Graph::node_exists(graph, instance_id);
//    const bool is_pending = Graph::node_pending(graph, instance_id);
    
    return is_valid;// || is_pending;
  }
  
  return false;
}


// -------------------------------------------------------- [ Relationships ] --


bool
Node::set_parent(const Node &other)
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    const uint32_t ref_type = other.is_valid() ? node_ref_id : node_owned_id;
  
    m_node_id = lib_ent::create(ref_type, instance_id);
    return Graph::node_set_parent(Data::get_graph_data(), other.get_id(), instance_id);
  }
  
  LOG_ERROR(node_msg_invalid_node);
  
  return false;
}


Node
Node::get_parent() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    return Node(Graph::node_get_parent(Data::get_graph_data(), instance_id));
  }
  
  LOG_ERROR(node_msg_invalid_node);
  
  return Node(uint32_t{0});
}


size_t
Node::get_child_count() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
//  if(instance_id)
  {
    return Graph::node_child_count(Data::get_graph_data(), instance_id);
  }
  
  LOG_ERROR(node_msg_invalid_node);
  
  return 0;
}


Node
Node::get_child(const size_t i) const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
//  if(instance_id)
  {
    return Node(Graph::node_get_child(Data::get_graph_data(), instance_id, i), false);
  }
  
  LIB_ASSERT(false);
  LOG_ERROR(node_msg_invalid_node);
  
  return Node(nullptr);
}


size_t
Node::get_descendant_count() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    return Graph::node_descendants_count(Data::get_graph_data(), instance_id);
  }
  
  LOG_ERROR(node_msg_invalid_node);
  
  return 0;
}


// --------------------------------------------------------- [ Names / Tags ] --


const char*
Node::get_name() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    const char *name = nullptr;
    Graph::node_get_name(Data::get_graph_data(), instance_id, &name);
    
    return name;
  }
  
  LOG_ERROR(node_msg_invalid_node);

  return "Unknown";
}
  
  
void
Node::set_name(const char *name)
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    if(Graph::node_set_name(Data::get_graph_data(), instance_id, name))
    {
      return;
    }
  }
  
  LOG_ERROR(node_msg_invalid_node);
}


bool
Node::add_tag(const char *tag)
{
  // -- Param Checks -- //
  {
    const bool tag_not_null = tag != nullptr;
    const bool tag_has_length = strlen(tag);
    
    LIB_ASSERT(tag_not_null);
    LIB_ASSERT(tag_has_length);
    
    if(!tag_not_null || !tag_has_length)
    {
      LOG_ERROR("Invalid tag cannot be added");
      return false;
    }
  }

  // -- Pedantic Checks -- //
  #ifdef NIL_PEDANTIC
  {
    if(strlen(tag) > node_tag_max_length)
    {
      LOG_WARNING("Tag will be truncated");
    }
  }
  #endif
  
  // -- Find Tag ID and Set -- //
  {
    size_t tag_id = 0;
    
    for(size_t i = 0; i < node_tag_max_count; ++i)
    {
      if(strcmp(&node_tag_buffer[i * node_tag_max_length], tag) == 0)
      {
        tag_id = 1 << i;
      }
    }
    
    if(tag_id)
    {
      
    }
  }
  
  // -- Insert New Tag and Set -- //
  {
  }
  
  return true;
}


void
Node::has_tag(const char *tag) const
{
}


void
Node::remove_tag(const char *tag)
{
}


void
Node::clear_tags()
{
}


// ----------------------------------------------------------------- [ Misc ] --


uintptr_t
Node::get_user_data() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    uintptr_t user_data;
    if(Graph::node_get_user_data(
        Data::get_graph_data(),
        instance_id,
        &user_data)
      )
    {
      return user_data;
    }
  }
  
  LOG_ERROR(node_msg_invalid_node);
  return 0;
}
  
  

void
Node::set_user_data(const uintptr_t user_data)
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    if(Graph::node_set_user_data(
        Data::get_graph_data(),
        instance_id,
        &user_data)
      )
    {
      return;
    }
  }
  
  LOG_ERROR(node_msg_invalid_node);
}


uint32_t
Node::get_id() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  return instance_id;
}


uint64_t
Node::get_data_type_id() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    uint64_t type_id;
    Graph::node_get_data_type_id(Data::get_graph_data(), instance_id, &type_id);
    return type_id;
  }
  
  LOG_ERROR(node_msg_invalid_node);
  
  return 0;
}


} // ns
