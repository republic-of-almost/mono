#ifndef COMPONENT_INTERFACE_INCLUDED_F0F83D3B_3846_47C2_B376_BAA6B29BC7F6
#define COMPONENT_INTERFACE_INCLUDED_F0F83D3B_3846_47C2_B376_BAA6B29BC7F6


#include <roa/fundamental.hpp>


namespace ROA_detail {


// ----------------------------------------- [ Internal component interface ] --


bool            add_component(const uint32_t object_id, ROA::Component *comp);
bool            has_component(const uint32_t object_id, const uint32_t rtti_id);
ROA::Component* get_component(const uint32_t object_id, const uint32_t rtti_id);


// ----------------------------------------------------------------- [ Data ] --


void        get_node_data(const uint32_t object_id, const ROA::Bounding_box &data);
void        has_node_data(const uint32_t object_id, const ROA::Bounding_box &data);
void        set_node_data(const uint32_t object_id, const ROA::Bounding_box &data);

void        get_node_data(const uint32_t object_id, const ROA::Camera &data);
void        has_node_data(const uint32_t object_id, const ROA::Camera &data);
void        set_node_data(const uint32_t object_id, const ROA::Camera &data);

void        get_node_data(const uint32_t object_id, const ROA::Light &data);
void        has_node_data(const uint32_t object_id, const ROA::Light &data);
void        set_node_data(const uint32_t object_id, const ROA::Light &data);

void        get_node_data(const uint32_t object_id, const ROA::Transform &data);
void        has_node_data(const uint32_t object_id, const ROA::Transform &data);
void        set_node_data(const uint32_t object_id, const ROA::Transform &data);

void        get_node_data(const uint32_t object_id, const ROA::Renderable &data);
void        has_node_data(const uint32_t object_id, const ROA::Renderable &data);
void        set_node_data(const uint32_t object_id, const ROA::Renderable &data);

void        get_node_data(const uint32_t object_id, const ROA::Audio_player &data);
void        has_node_data(const uint32_t object_id, const ROA::Audio_player &data);
void        set_node_data(const uint32_t object_id, const ROA::Audio_player &data);

void        get_node_data(const uint32_t object_id, const ROA::Logic &data);
void        has_node_data(const uint32_t object_id, const ROA::Logic &data);
void        set_node_data(const uint32_t object_id, const ROA::Logic &data);


} // ns


#endif // inc guard
