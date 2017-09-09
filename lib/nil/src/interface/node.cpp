#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph.hpp>
#include <graph/graph_data.hpp>
#include <lib/entity.hpp>
#include <lib/logging.hpp>


// -------------------------------------------------------- [ Config / Data ] --


#ifndef NIL_MAX_TAG_NAME_LENGTH
#define NIL_MAX_TAG_NAME_LENGTH 64
#endif


namespace {


// ---------------------------------------------------------- [ Identifiers ] --


constexpr uint32_t node_owned_id = 1;
constexpr uint32_t node_ref_id   = 0;
constexpr char node_no_name[]    = "Unknown";
constexpr char node_no_tag[]     = "";


// ----------------------------------------------------------------- [ Tags ] --


// dont exceed this is a hard limit, uint64_t
constexpr size_t node_tag_max_count  = 64;
constexpr size_t node_tag_max_length = NIL_MAX_TAG_NAME_LENGTH + 1;
size_t           node_tag_curr_count = 0;

char node_tag_buffer[node_tag_max_count * node_tag_max_length]{};


// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_node_invalid[]     = "Node is invalid";
constexpr char msg_node_tag_limit[]   = "Exceeds tag limit of 64";
constexpr char msg_node_tag_trunc[]   = "Tag will be truncated";
constexpr char msg_node_tag_invalid[] = "Tag must be a valid string";


} // anon ns


namespace Nil {


namespace lib_ent = lib::entity;


// ------------------------------------------------------------- [ Lifetime ] --


Node::Node()
: m_node_id(
  lib_ent::create(
    node_owned_id,
    Graph::node_create(Data::get_graph_data())
  )
)
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


// ---------------------------------------------------------------- [ State ] --
  

bool
Node::destroy()
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  if(instance_id)
  {
    return Graph::node_remove(Data::get_graph_data(), instance_id);
  }
  
  LOG_WARNING(msg_node_invalid);
  
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
    
    return is_valid;
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
  
  LOG_ERROR(msg_node_invalid);
  
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
  
  LOG_ERROR(msg_node_invalid);
  
  return Node(uint32_t{0});
}


size_t
Node::get_child_count() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  return Graph::node_child_count(Data::get_graph_data(), instance_id);
}


Node
Node::get_child(const size_t i) const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  return Node(Graph::node_get_child(Data::get_graph_data(), instance_id, i), false);
}


size_t
Node::get_descendant_count() const
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);
  
  return Graph::node_descendants_count(Data::get_graph_data(), instance_id);
}


// ---------------------------------------------------------- [ Name / Tags ] --


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
  
  LOG_ERROR(msg_node_invalid);

  return node_no_name;
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
  
  LOG_ERROR(msg_node_invalid);
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
      LOG_ERROR(msg_node_tag_invalid);
      return false;
    }
  }

  // -- Pedantic Checks -- //
  #ifdef NIL_PEDANTIC
  {
    if(strlen(tag) > node_tag_max_length)
    {
      LOG_WARNING(msg_node_tag_trunc);
    }
  }
  #endif
  
  uint64_t tag_id = 0;
  
  // -- Find Tag ID -- //
  {
    size_t tag_id = 0;
    
    for(size_t i = 0; i < node_tag_max_count; ++i)
    {
      if(strcmp(&node_tag_buffer[i * node_tag_max_length], tag) == 0)
      {
        tag_id = 1 << i;
      }
    }
  }
  
  // -- Insert New Tag -- //
  if(!tag_id)
  {
    if(node_tag_curr_count < node_tag_max_length)
    {
      tag_id = 1 << node_tag_curr_count;
      
      size_t tag_len = math::min((uint32_t)strlen(tag), (uint32_t)node_tag_max_length);
      
      memcpy(&node_tag_buffer[node_tag_curr_count * node_tag_max_length], tag, tag_len);
      node_tag_curr_count += 1;
    }
    else
    {
      LOG_WARNING(msg_node_tag_limit);
      return false;
    }
  }
  
  // -- Set Tag ID -- //
  if(tag_id)
  {
    const uint32_t instance_id = lib_ent::instance(m_node_id);
    
    if(instance_id)
    {
      uint64_t tags = 0;
      Graph::node_get_tags(Data::get_graph_data(), instance_id, &tags);
      
      tags |= tag_id;
      Graph::node_set_tags(Data::get_graph_data(), instance_id, tags);
    }
  }
  
  return true;
}


bool
Node::has_tag(const char *tag) const
{
  // -- Param Checks -- //
  #ifdef NIL_PEDANTIC
  {
    const bool tag_not_null = tag != nullptr;
    const bool tag_has_length = strlen(tag);
    
    LIB_ASSERT(tag_not_null);
    LIB_ASSERT(tag_has_length);
    
    if(!tag_not_null || !tag_has_length)
    {
      LOG_ERROR(msg_node_tag_invalid);
      return false;
    }
  }

  {
    if(strlen(tag) > node_tag_max_length)
    {
      LOG_WARNING(msg_node_tag_trunc);
    }
  }
  #endif

  // -- Check we have this tag -- //
  uint64_t this_tag = 0;
  {
    for(size_t i = 0; i < node_tag_curr_count; ++i)
    {
      if(strcmp(tag, &node_tag_buffer[i * node_tag_max_length]) == 0)
      {
        this_tag = 1 << i;
        break;
      }
    }
  }

  // -- Check against node tags -- //
  if(this_tag)
  {
    const uint32_t instance_id = lib_ent::instance(m_node_id);

    if(instance_id)
    {
      uint64_t tags = 0;
      Graph::node_get_tags(Data::get_graph_data(), instance_id, &tags);
      
      return !(tags & this_tag);
    }
  }
  
  return false;
}


void
Node::remove_tag(const char *tag)
{

}


const char*
Node::get_tag(const size_t tag_index) const
{
  // -- Param Checks -- //
  {
    const bool over_max_tags = tag_index >= node_tag_max_count;
  
    LIB_ASSERT(over_max_tags == false);
    
    if(over_max_tags)
    {
      LOG_WARNING(msg_node_tag_limit);
      return "";
    }
    
  }

  // -- Get Tags -- //
  uint64_t tags = 0;
  {
    const uint32_t instance_id = lib_ent::instance(m_node_id);

    if(instance_id)
    {
      Graph::node_get_tags(Data::get_graph_data(), instance_id, &tags);
    }
  }

  // -- Count Bits -- //
  uint64_t tag_bit = 0;
  size_t bit_counter = 0;
  {
    for(uint64_t b = 0; b < node_tag_max_count; ++b)
    {
      const uint64_t this_bit = 1 << b;
      
      if(this_bit & tags)
      {
        if(bit_counter == tag_index)
        {
          tag_bit = this_bit;
          
          break;
        }
        
        bit_counter += 1;
      }
    }
  }
  
  // -- Find Tag -- //
  {
    if(tag_bit)
    {
      LIB_ASSERT(bit_counter < node_tag_max_count);
      
      const char *tag = &node_tag_buffer[bit_counter * node_tag_max_length];
      return tag;
    }
  }
  
  return node_no_tag;
}


size_t
Node::get_tag_count() const
{
  // -- Get Tags -- //
  uint64_t tags = 0;
  {
    const uint32_t instance_id = lib_ent::instance(m_node_id);

    if(instance_id)
    {
      Graph::node_get_tags(Data::get_graph_data(), instance_id, &tags);
    }
  }

  // -- Count Bits -- //
  size_t bit_counter = 0;
  {
    for(uint64_t b = 0; b < node_tag_max_count; ++b)
    {
      const uint64_t this_bit = uint64_t{1} << b;
      
      if(this_bit & tags)
      {
        bit_counter += 1;
      }
    }
  }
  
  return bit_counter;
}


void
Node::clear_tags()
{
  const uint32_t instance_id = lib_ent::instance(m_node_id);

  if(instance_id)
  {
    constexpr uint64_t tags = 0;
    Graph::node_set_tags(Data::get_graph_data(), instance_id, tags);
  }
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
  
  LOG_ERROR(msg_node_invalid);
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
  
  LOG_ERROR(msg_node_invalid);
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
  
  LOG_ERROR(msg_node_invalid);
  
  return 0;
}


} // ns


// --------------------------------------------------------------- [ Config ] --


#undef NIL_MAX_TAG_NAME_LENGTH
