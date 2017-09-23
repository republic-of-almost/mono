#include <roa/detail/resource_interface.hpp>
#include <roa/material.hpp>
#include <roa/shader.hpp>
#include <common/resource_identifiers.hpp>
#include <nil/resource/resource.hpp>
#include <lib/logging.hpp>
#include <lib/entity.hpp>


namespace ROA_detail {


// ---------------------------------------------------------------- [ Setup ] --


ROA::Resource
setup_resource(const uint32_t id)
{
  ROA::Resource rsrc;
  rsrc.m_id = id;
  
  ROA::Resource other = rsrc;
  
  return rsrc;
}


// ------------------------------------------------------------- [ Material ] --


ROA::Material
create_resource(ROA::Material &mat, const char *name)
{
  Nil::Resource::Material rsrc_data{};
  rsrc_data.name = name;
  rsrc_data.color = 0xFFFFFFFF;

  const bool submited = Nil::Resource::load(rsrc_data);
  
  if(submited)
  {
    const uint32_t entity_id = lib::entity::create(Resource::MATERIAL, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Material*>(&rsrc);
  }
  
  LOG_FATAL("Failed to create Material");
  return ROA::Material{};
}


ROA::Material
load_resource(ROA::Material &mat, const char *file)
{
  LOG_FATAL("Failed to load Material - Disc format not yet considered");
  return ROA::Material{};
}


ROA::Material
find_resource(ROA::Material &mat, const char *name)
{
  Nil::Resource::Material rsrc_data{};

  const bool found = Nil::Resource::find_by_name(name, rsrc_data);
  
  if(found)
  {
    const uint32_t entity_id = lib::entity::create(Resource::MATERIAL, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Material*>(&rsrc);
  }
  
  LOG_FATAL("Failed to find Material");
  return ROA::Material{};
}


// --------------------------------------------------------------- [ Shader ] --


ROA::Shader
create_resource(ROA::Shader &mat, const char *name)
{
  Nil::Resource::Shader rsrc_data{};
  rsrc_data.name = name;

  const bool submited = Nil::Resource::load(rsrc_data);
  
  if(submited)
  {
    const uint32_t entity_id = lib::entity::create(Resource::MATERIAL, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Shader*>(&rsrc);
  }
  
  LOG_FATAL("Failed to create Material");
  return ROA::Shader{};
}


ROA::Shader
load_resource(ROA::Shader &mat, const char *file)
{
  LOG_FATAL("Failed to load Shader - Disc format not yet considered");
  return ROA::Shader{};
}


ROA::Shader
find_resource(ROA::Shader &mat, const char *name)
{
  Nil::Resource::Material rsrc_data{};

  const bool found = Nil::Resource::find_by_name(name, rsrc_data);
  
  if(found)
  {
    const uint32_t entity_id = lib::entity::create(Resource::MATERIAL, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Shader*>(&rsrc);
  }
  
  LOG_FATAL("Failed to find Shader");
  return ROA::Shader{};
}


} // ns
