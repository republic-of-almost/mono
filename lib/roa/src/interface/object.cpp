#include <roa/object.hpp>
#include <roa/bounding_box.hpp>
#include <roa/transform.hpp>
#include <roa/logic.hpp>
#include <roa/camera.hpp>
#include <roa/renderable.hpp>
#include <common/common.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <new>


/*
  NOTE
  --
  Using a uint64_t as a data store is a temp messure.
  We know we want this to be stack allocated at this end.
  We know it needs to have the same semantics as Nil::Node
  We know we don't wish to expose Nil in the ROA interface.
  I think what we want is to deal with the graph direct.
  but that means perhaps converting alot of Nil getters to use uint32_t's
  instead of Node.
*/

namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Object::Object(const uint32_t instance_id)
: m_id(0)
{
  new(&m_id) Nil::Node(instance_id);
}


Object::Object()
: m_id(0)
{
  static_assert(sizeof(Nil::Node) <= sizeof(m_id), "Not enough storeage");
  
  new(&m_id) Nil::Node();
}


Object::Object(const uint64_t data_bitfield)
: Object()
{
  
}


Object::Object(const ROA_nullptr)
: m_id(0)
{
}


Object::Object(const Object &other)
{
  Nil::Node *other_node = reinterpret_cast<Nil::Node*>(&other.m_id);

  new(&m_id) Nil::Node(*other_node);
  Nil::Node *this_node = reinterpret_cast<Nil::Node*>(&this_node);
}


Object::Object(Object &&other)
{
  Nil::Node *other_node = reinterpret_cast<Nil::Node*>(&other.m_id);

  new(&m_id) Nil::Node();
  Nil::Node *this_node = reinterpret_cast<Nil::Node*>(&m_id);
  *this_node = static_cast<Nil::Node&&>(*other_node);
}


Object::~Object()
{
  Nil::Node *node = reinterpret_cast<Nil::Node*>(&m_id);
  
  if(!node->is_ref())
  {
    node->destroy();
  }
}


Object&
Object::operator=(const Object &other)
{
  return *this;
}


Object&
Object::operator=(const Object &&other)
{
  return *this;
}


void
Object::destroy()
{
  if(m_id)
  {
    reinterpret_cast<Nil::Node*>(&m_id)->destroy();
  }
}


bool
Object::is_valid() const
{
  if(m_id)
  {
    return reinterpret_cast<Nil::Node*>(&m_id)->is_valid();
  }
  
  return false;
}


bool
Object::is_ref() const
{
  if(m_id)
  {
    return reinterpret_cast<Nil::Node*>(&m_id)->is_ref();
  }
  
  return false;
}


// -------------------------------------------------------- [ Relationships ] --


Object
Object::get_parent() const
{
  Nil::Node *node = reinterpret_cast<Nil::Node*>(&m_id);
  return Object(node->get_id());
}


Object
Object::get_child(const size_t child) const
{
  return Object(nullptr);
}


size_t
Object::get_child_count() const
{
  return reinterpret_cast<Nil::Node*>(&m_id)->get_child_count();
}


// ----------------------------------------------------------- [ Attributes ] --

const char*
Object::get_name() const
{
  Nil::Node *node = reinterpret_cast<Nil::Node*>(&m_id);
  return node->get_name();
}


void
Object::set_name(const char *name)
{
  Nil::Node *node = reinterpret_cast<Nil::Node*>(&m_id);
  node->set_name(name);
}


const char*
Object::get_type_name() const
{
  return "Unknown";
}


uint32_t
Object::get_type_id() const
{
  return 0;
}


uint32_t
Object::get_instance_id() const
{
  if(m_id)
  {
    return reinterpret_cast<Nil::Node*>(&m_id)->get_id();
  }

  return 0;
}


// ------------------------------------------------------ [ Data Components ] --
  
  
Bounding_box
Object::get_bounding_box() const
{
  return *reinterpret_cast<Bounding_box*>(const_cast<Object*>(this));
}


void
Object::set_bounding_box(const ROA::Bounding_box &in)
{
  if(in.get_instance_id() != get_instance_id())
  {
    Nil::Data::Bounding_box data{};
    
    Nil::Node other_node = ROA_detail::get_node(in);
    Nil::Node this_node = ROA_detail::get_node(*this);
    
    Nil::Data::get(other_node, data);
    Nil::Data::set(this_node, data);
  }
}


Camera
Object::get_camera() const
{
}


void
Object::set_camera(const ROA::Camera &in)
{
  if(in.get_instance_id() != get_instance_id())
  {
    Nil::Data::Camera data{};
    
    Nil::Node other_node = ROA_detail::get_node(in);
    Nil::Node this_node = ROA_detail::get_node(*this);
    
    Nil::Data::get(other_node, data);
    Nil::Data::set(this_node, data);
  }
}


Logic
Object::get_logic() const
{
  return Logic();
}


void
Object::set_logic(const ROA::Logic &in)
{
}


Transform
Object::get_transform() const
{
  return *reinterpret_cast<Transform*>(const_cast<Object*>(this));
}


void
Object::set_transform(const ROA::Transform &in)
{
  if(in.get_instance_id() != get_instance_id())
  {
    Nil::Data::Transform data{};
    
    Nil::Node other_node = ROA_detail::get_node(in);
    Nil::Node this_node = ROA_detail::get_node(*this);
    
    Nil::Data::get(other_node, data);
    Nil::Data::set(this_node, data);
  }
}


Renderable
Object::get_renderable() const
{
}


void
Object::set_renderable(const Renderable &in)
{
  if(in.get_instance_id() != get_instance_id())
  {
    Nil::Data::Renderable data{};
    
    Nil::Node other_node = ROA_detail::get_node(in);
    Nil::Node this_node = ROA_detail::get_node(*this);
    
    Nil::Data::get(other_node, data);
    Nil::Data::set(this_node, data);
  }
}


} // ns
