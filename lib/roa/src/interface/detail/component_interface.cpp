#include <roa/detail/component_interface.hpp>
#include <roa/bounding_box.hpp>
#include <roa/camera.hpp>
#include <roa/light.hpp>
#include <roa/transform.hpp>
#include <roa/renderable.hpp>
#include <roa/audio_player.hpp>
#include <roa/logic.hpp>
#include <nil/node.hpp>
#include <nil/data/bounding_box.hpp>
#include <nil/data/camera.hpp>
#include <nil/data/light.hpp>
#include <nil/data/transform.hpp>
#include <nil/data/renderable.hpp>
#include <nil/data/audio.hpp>
#include <nil/data/logic.hpp>


namespace ROA_detail {


// ----------------------------------------- [ Internal component interface ] --

// -- defined in component_engine.cpp -- //


// ---------------------------------------------------- [ Bounding Box Data ] --


ROA::Bounding_box
get_node_data(ROA::Object obj, const ROA::Bounding_box &data)
{
  const bool has_data = has_node_data(obj, data);

  if(has_data)
  {
    return *reinterpret_cast<const ROA::Bounding_box*>(&obj);
  }
  
  return data;
}


bool
has_node_data(ROA::Object obj, const ROA::Bounding_box &data)
{
  if(obj)
  {
    Nil::Data::Bounding_box data{};
    Nil::Node this_node(obj.get_instance_id());
    
    return Nil::Data::has(this_node, data);
  }
  
  return false;
}


void
set_node_data(ROA::Object obj, const ROA::Bounding_box &data)
{
  if(obj)
  {
    Nil::Data::Bounding_box nil_data{};
    
    Nil::Node other_node(data.get_instance_id());
    Nil::Node this_node(obj.get_instance_id());
    
    Nil::Data::get(other_node, nil_data);
    Nil::Data::set(this_node, nil_data);
  }
}


// ---------------------------------------------------------- [ Camera Data ] --


ROA::Camera
get_node_data(ROA::Object obj, const ROA::Camera &data)
{
  const bool has_data = has_node_data(obj, data);

  if(has_data)
  {
    return *reinterpret_cast<const ROA::Camera*>(&obj);
  }
  
  return data;
}


bool
has_node_data(ROA::Object obj, const ROA::Camera &data)
{
  if(obj)
  {
    Nil::Data::Camera data{};
    Nil::Node this_node(obj.get_instance_id());
    
    return Nil::Data::has(this_node, data);
  }
  
  return false;
}


void
set_node_data(ROA::Object obj, const ROA::Camera &data)
{
  if(obj)
  {
    Nil::Data::Camera nil_data{};
    
    Nil::Node other_node(data.get_instance_id());
    Nil::Node this_node(obj.get_instance_id());
    
    Nil::Data::get(other_node, nil_data);
    Nil::Data::set(this_node, nil_data);
  }
}


// ----------------------------------------------------------- [ Light Data ] --


ROA::Light
get_node_data(ROA::Object obj, const ROA::Light &data)
{
  const bool has_data = has_node_data(obj, data);

  if(has_data)
  {
    return *reinterpret_cast<const ROA::Light*>(&obj);
  }
  
  return data;
}


bool
has_node_data(ROA::Object obj, const ROA::Light &data)
{
  if(obj)
  {
    Nil::Data::Light data{};
    Nil::Node this_node(obj.get_instance_id());
    
    return Nil::Data::has(this_node, data);
  }
  
  return false;
}


void
set_node_data(ROA::Object obj, const ROA::Light &data)
{
  if(obj)
  {
    Nil::Data::Light nil_data{};
    
    Nil::Node other_node(data.get_instance_id());
    Nil::Node this_node(obj.get_instance_id());
    
    Nil::Data::get(other_node, nil_data);
    Nil::Data::set(this_node, nil_data);
  }
}


// ------------------------------------------------------- [ Transform Data ] --


ROA::Transform
get_node_data(ROA::Object obj, const ROA::Transform &data)
{
  const bool has_data = has_node_data(obj, data);

  if(has_data)
  {
    return *reinterpret_cast<const ROA::Transform*>(&obj);
  }
  
  return data;
}


bool
has_node_data(ROA::Object obj, const ROA::Transform &data)
{
  if(obj)
  {
    Nil::Data::Transform data{};
    Nil::Node this_node(obj.get_instance_id());
    
    return Nil::Data::has(this_node, data);
  }
  
  return false;
}


void
set_node_data(ROA::Object obj, const ROA::Transform &data)
{
  if(obj)
  {
    Nil::Data::Transform nil_data{};
    
    Nil::Node other_node(data.get_instance_id());
    Nil::Node this_node(obj.get_instance_id());
    
    Nil::Data::get(other_node, nil_data);
    Nil::Data::set(this_node, nil_data);
  }
}


// ------------------------------------------------------ [ Renderable Data ] --


ROA::Renderable
get_node_data(ROA::Object obj, const ROA::Renderable &data)
{
  const bool has_data = has_node_data(obj, data);

  if(has_data)
  {
    return *reinterpret_cast<const ROA::Renderable*>(&obj);
  }
  
  return data;
}



bool
has_node_data(ROA::Object obj, const ROA::Renderable &data)
{
  if(obj)
  {
    Nil::Data::Renderable data{};
    Nil::Node this_node(obj.get_instance_id());
    
    return Nil::Data::has(this_node, data);
  }
  
  return false;
}


void
set_node_data(ROA::Object obj, const ROA::Renderable &data)
{
  if(obj)
  {
    Nil::Data::Renderable nil_data{};
    
    Nil::Node other_node(data.get_instance_id());
    Nil::Node this_node(obj.get_instance_id());
    
    Nil::Data::get(other_node, nil_data);
    Nil::Data::set(this_node, nil_data);
  }
}


// ---------------------------------------------------- [ Audio_player Data ] --


ROA::Audio_player
get_node_data(ROA::Object obj, const ROA::Audio_player &data)
{
  const bool has_data = has_node_data(obj, data);

  if(has_data)
  {
    return *reinterpret_cast<const ROA::Audio_player*>(&obj);
  }
  
  return data;
}


bool
has_node_data(ROA::Object obj, const ROA::Audio_player &data)
{
  if(obj)
  {
    Nil::Data::Audio data{};
    Nil::Node this_node(obj.get_instance_id());
    
    return Nil::Data::has(this_node, data);
  }
  
  return false;
}


void
set_node_data(ROA::Object obj, const ROA::Audio_player &data)
{
  if(obj)
  {
    Nil::Data::Audio nil_data{};
    
    Nil::Node other_node(data.get_instance_id());
    Nil::Node this_node(obj.get_instance_id());
    
    Nil::Data::get(other_node, nil_data);
    Nil::Data::set(this_node, nil_data);
  }
}


// ----------------------------------------------------------- [ Logic Data ] --


ROA::Logic
get_node_data(ROA::Object obj, const ROA::Logic &data)
{
  const bool has_data = has_node_data(obj, data);

  if(has_data)
  {
    return *reinterpret_cast<const ROA::Logic*>(&obj);
  }
  
  return data;
}


bool
has_node_data(ROA::Object obj, const ROA::Logic &data)
{
  if(obj)
  {
    Nil::Data::Logic data{};
    Nil::Node this_node(obj.get_instance_id());
    
    return Nil::Data::has(this_node, data);
  }
  
  return false;
}


void
set_node_data(ROA::Object obj, const ROA::Logic &data)
{
  if(obj)
  {
    Nil::Data::Logic nil_data{};
    
    Nil::Node other_node(data.get_instance_id());
    Nil::Node this_node(obj.get_instance_id());
    
    Nil::Data::get(other_node, nil_data);
    Nil::Data::set(this_node, nil_data);
  }
}


} // ns
