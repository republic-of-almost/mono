#include <roa/roa.hpp>
#include <common/common.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <lib/assert.hpp>
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
  
  Nil::Node *this_node = reinterpret_cast<Nil::Node*>(&m_id);
}


Object::Object(const uint64_t data_bitfield)
: Object()
{
  if(data_bitfield & Data::BOUNDING_BOX)
  {
    Bounding_box data;
    this->set_bounding_box(data);
  }
  
  if(data_bitfield & Data::CAMERA)
  {
    Camera data;
    this->set_camera(data);
  }
  
  if(data_bitfield & Data::LOGIC)
  {
    Logic data;
    this->set_logic(data);
  }
  
  if(data_bitfield & Data::RENDERABLE)
  {
    Renderable data;
    this->set_renderable(data);
  }
}


Object::Object(const ROA_nullptr)
: m_id(0)
{
}


Object::Object(const Object &other) noexcept
{
  Nil::Node *other_node = reinterpret_cast<Nil::Node*>(&other.m_id);

  new(&m_id) Nil::Node(*other_node);
  Nil::Node *this_node = reinterpret_cast<Nil::Node*>(&this_node);
}


Object::Object(Object &&other) noexcept
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
Object::operator=(const Object &other) noexcept
{
  Nil::Node *this_node = reinterpret_cast<Nil::Node*>(&m_id);
  Nil::Node *that_node = reinterpret_cast<Nil::Node*>(&other.m_id);

  this_node->operator=(*that_node);
  
  return *this;
}


Object&
Object::operator=(Object &&other) noexcept
{
  Nil::Node *this_node = reinterpret_cast<Nil::Node*>(&m_id);
  Nil::Node *that_node = reinterpret_cast<Nil::Node*>(&other.m_id);

  this_node->operator=(static_cast<Nil::Node&&>(*that_node)); // hmm

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


void
Object::set_parent(const ROA::Object obj)
{
  Nil::Node *this_node = reinterpret_cast<Nil::Node*>(&m_id);
  Nil::Node *that_node = reinterpret_cast<Nil::Node*>(&obj.m_id);
  
  this_node->set_parent(*that_node);
}


Object
Object::get_child(const size_t child) const
{
  Nil::Node this_node = *reinterpret_cast<Nil::Node*>(&m_id);
  Nil::Node child_node = this_node.get_child(child);
  
  return Object(child_node.get_id());
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


uintptr_t
Object::get_user_data() const
{
  if(m_id)
  {
    return reinterpret_cast<Nil::Node*>(&m_id)->get_user_data();
  }

  return 0;
}


void
Object::set_user_data(uintptr_t user_data)
{
  if(m_id)
  {
    reinterpret_cast<Nil::Node*>(&m_id)->set_user_data(user_data);
  }
}


// ------------------------------------------------------ [ Data Components ] --


const Bounding_box
Object::get_bounding_box() const
{
  return const_cast<Object*>(this)->get_bounding_box();
}
  
  
Bounding_box
Object::get_bounding_box()
{
  return *reinterpret_cast<const Bounding_box*>(this);
}


void
Object::set_bounding_box(const Bounding_box &in)
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


const Camera
Object::get_camera() const
{
  return const_cast<Object*>(this)->get_camera();
}

Camera
Object::get_camera()
{
  return *reinterpret_cast<Camera*>(this);
}


void
Object::set_camera(const Camera &in)
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


const Light
Object::get_light() const
{
  return const_cast<Object*>(this)->get_light();
}


Light
Object::get_light()
{
  return *reinterpret_cast<Light*>(this);
}


void
Object::set_light(const Light &in)
{
  if(in.get_instance_id() != get_instance_id())
  {
    Nil::Data::Light data{};
    
    Nil::Node other_node = ROA_detail::get_node(in);
    Nil::Node this_node = ROA_detail::get_node(*this);
    
    Nil::Data::get(other_node, data);
    Nil::Data::set(this_node, data);
  }
}


const Logic
Object::get_logic() const
{
  return const_cast<Object*>(this)->get_logic();
}

Logic
Object::get_logic()
{
  return *reinterpret_cast<Logic*>(this);
}


void
Object::set_logic(const Logic &in)
{
  if(in.get_instance_id() != get_instance_id())
  {
    Nil::Data::Logic data{};
    
    Nil::Node other_node = ROA_detail::get_node(in);
    Nil::Node this_node = ROA_detail::get_node(*this);
    
    Nil::Data::get(other_node, data);
    Nil::Data::set(this_node, data);
  }
}


const Transform
Object::get_transform() const
{
  return const_cast<Object*>(this)->get_transform();
}


Transform
Object::get_transform()
{
  return *reinterpret_cast<Transform*>(this);
}


void
Object::set_transform(const Transform &in)
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


const Renderable
Object::get_renderable() const
{
  return const_cast<Object*>(this)->get_renderable();
}


Renderable
Object::get_renderable()
{
  return *reinterpret_cast<Renderable*>(this);
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


const Audio_player
Object::get_audio_player() const
{
  return const_cast<Object*>(this)->get_audio_player();
}


Audio_player
Object::get_audio_player()
{
  return *reinterpret_cast<Audio_player*>(this);
}


void
Object::set_audio_player(const Audio_player &in)
{
  if(in.get_instance_id() != get_instance_id())
  {
    Nil::Data::Audio data{};
    
    Nil::Node other_node = ROA_detail::get_node(in);
    Nil::Node this_node = ROA_detail::get_node(*this);
    
    Nil::Data::get(other_node, data);
    Nil::Data::set(this_node, data);
  }
}


} // ns
