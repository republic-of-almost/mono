#include <roa/detail/resource_interface.hpp>
#include <roa/material.hpp>
#include <roa/shader.hpp>
#include <roa/mesh.hpp>
#include <roa/material.hpp>
#include <roa/model.hpp>
#include <roa/audio_source.hpp>
#include <common/context.hpp>
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
  rsrc_data.name   = name;
  rsrc_data.color  = 0xFFFFFFFF;
  rsrc_data.status = Nil::Resource::Load_status::NONE;

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
  Nil_shader rsrc_data{};
  rsrc_data.name   = name;
  rsrc_data.status = NIL_RSRC_STATUS_NONE;
  
  const uint32_t id = nil_rsrc_shader_create(ROA_detail::get_ctx(), &rsrc_data);
  
  if(id)
  {
    const uint32_t entity_id = lib::entity::create(Resource::SHADER, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Shader*>(&rsrc);
  }
  
  LOG_FATAL("Failed to create Shader");
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
  Nil_shader rsrc_data{};

  const bool found = nil_rsrc_shader_find_by_name(ROA_detail::get_ctx(), name, &rsrc_data);
  
  if(found)
  {
    const uint32_t entity_id = lib::entity::create(Resource::SHADER, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Shader*>(&rsrc);
  }
  
  LOG_FATAL("Failed to find Shader");
  return ROA::Shader{};
}


// ----------------------------------------------------------------- [ Mesh ] --


ROA::Mesh
create_resource(ROA::Mesh &rsrc, const char *name)
{
  Nil::Resource::Mesh rsrc_data{};
  rsrc_data.name = name;

  const bool submited = Nil::Resource::load(rsrc_data);
  
  if(submited)
  {
    const uint32_t entity_id = lib::entity::create(Resource::MESH, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Mesh*>(&rsrc);
  }
  
  LOG_FATAL("Failed to create Mesh");
  return ROA::Mesh{};
}


ROA::Mesh
load_resource(ROA::Mesh &rsrc, const char *file)
{
  LOG_FATAL("Failed to load Mesh - Disc format not yet considered");
  return ROA::Mesh{};
}


ROA::Mesh
find_resource(ROA::Mesh &rsrc, const char *name)
{
  Nil::Resource::Mesh rsrc_data{};

  const bool found = Nil::Resource::find_by_name(name, rsrc_data);
  
  if(found)
  {
    const uint32_t entity_id = lib::entity::create(Resource::MESH, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Mesh*>(&rsrc);
  }
  
  LOG_FATAL("Failed to find Mesh");
  return ROA::Mesh{};
}


// --------------------------------------------------------- [ Audio Source ] --


ROA::Audio_source
create_resource(ROA::Audio_source &rsrc, const char *name)
{
  Nil::Resource::Audio rsrc_data{};
  rsrc_data.name = name;

  const bool submited = Nil::Resource::load(rsrc_data);
  
  if(submited)
  {
    const uint32_t entity_id = lib::entity::create(Resource::AUDIO, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Audio_source*>(&rsrc);
  }
  
  LOG_FATAL("Failed to create Audio source");
  return ROA::Audio_source{};
}


ROA::Audio_source
load_resource(ROA::Audio_source &rsrc, const char *file)
{
  LOG_FATAL("Failed to load Audio - Disc format not yet considered");
  return ROA::Audio_source{};
}


ROA::Audio_source
find_resource(ROA::Audio_source &rsrc, const char *name)
{
  Nil::Resource::Audio rsrc_data{};

  const bool found = Nil::Resource::find_by_name(name, rsrc_data);
  
  if(found)
  {
    const uint32_t entity_id = lib::entity::create(Resource::AUDIO, rsrc_data.id);
    ROA::Resource rsrc = setup_resource(entity_id);
    return *reinterpret_cast<ROA::Audio_source*>(&rsrc);
  }
  
  LOG_FATAL("Failed to find Audio Source");
  return ROA::Audio_source{};
}


} // ns
