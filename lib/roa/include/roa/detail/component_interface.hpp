#ifndef COMPONENT_INTERFACE_INCLUDED_F0F83D3B_3846_47C2_B376_BAA6B29BC7F6
#define COMPONENT_INTERFACE_INCLUDED_F0F83D3B_3846_47C2_B376_BAA6B29BC7F6


#include <roa/fundamental.hpp>


namespace ROA_detail {


// ----------------------------------------- [ Internal component interface ] --


bool            add_component(const uint32_t object_id, ROA::Component *comp);
bool            has_component(const uint32_t object_id, const uint32_t rtti_id);
ROA::Component* get_component(const uint32_t object_id, const uint32_t rtti_id);


// ----------------------------------------------------------------- [ Data ] --


ROA::Bounding_box     get_node_data(ROA::Object obj, const ROA::Bounding_box &data);
bool                  has_node_data(ROA::Object obj, const ROA::Bounding_box &data);
void                  set_node_data(ROA::Object obj, const ROA::Bounding_box &data);

ROA::Camera           get_node_data(ROA::Object obj, const ROA::Camera &data);
bool                  has_node_data(ROA::Object obj, const ROA::Camera &data);
void                  set_node_data(ROA::Object obj, const ROA::Camera &data);

ROA::Light            get_node_data(ROA::Object obj, const ROA::Light &data);
bool                  has_node_data(ROA::Object obj, const ROA::Light &data);
void                  set_node_data(ROA::Object obj, const ROA::Light &data);

ROA::Transform        get_node_data(ROA::Object obj, const ROA::Transform &data);
bool                  has_node_data(ROA::Object obj, const ROA::Transform &data);
void                  set_node_data(ROA::Object obj, const ROA::Transform &data);

ROA::Renderable       get_node_data(ROA::Object obj, const ROA::Renderable &data);
bool                  has_node_data(ROA::Object obj, const ROA::Renderable &data);
void                  set_node_data(ROA::Object obj, const ROA::Renderable &data);
  
ROA::Audio_player     get_node_data(ROA::Object obj, const ROA::Audio_player &data);
bool                  has_node_data(ROA::Object obj, const ROA::Audio_player &data);
void                  set_node_data(ROA::Object obj, const ROA::Audio_player &data);

ROA::Logic            get_node_data(ROA::Object obj, const ROA::Logic &data);
bool                  has_node_data(ROA::Object obj, const ROA::Logic &data);
void                  set_node_data(ROA::Object obj, const ROA::Logic &data);


} // ns


#endif // inc guard
