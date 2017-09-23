#ifndef RESOURCE_INTERFACE_INCLUDED_99563A0D_3D83_46D5_BF6C_5964DB4BBD8B
#define RESOURCE_INTERFACE_INCLUDED_99563A0D_3D83_46D5_BF6C_5964DB4BBD8B


#include <roa/fundamental.hpp>


namespace ROA_detail {


ROA::Resource         setup_resource(const uint32_t id);


ROA::Material         create_resource(ROA::Material &mat, const char *name);
ROA::Material         load_resource(ROA::Material &mat, const char *file);
ROA::Material         find_resource(ROA::Material &mat, const char *name);


ROA::Shader           create_resource(ROA::Shader &rsrc, const char *name);
ROA::Shader           load_resource(ROA::Shader &rsrc, const char *file);
ROA::Shader           find_resource(ROA::Shader &rsrc, const char *name);


ROA::Mesh             create_resource(ROA::Mesh &rsrc, const char *name);
ROA::Mesh             load_resource(ROA::Mesh &rsrc, const char *file);
ROA::Mesh             find_resource(ROA::Mesh &rsrc, const char *name);


ROA::Audio_source     create_resource(ROA::Audio_source &rsrc, const char *name);
ROA::Audio_source     load_resource(ROA::Audio_source &rsrc, const char *file);
ROA::Audio_source     find_resource(ROA::Audio_source &rsrc, const char *name);


} // ns


#endif // inc guard
