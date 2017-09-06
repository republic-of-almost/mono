#ifndef MESH_INCLUDED_900EC7E8_4846_4CC9_8D4E_CC3CD8991D17
#define MESH_INCLUDED_900EC7E8_4846_4CC9_8D4E_CC3CD8991D17


#include <roa/fundamental.hpp>
#include <roa/resource.hpp>


namespace ROA {


class Mesh final : public Resource
{
public:
  
  
  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  explicit          Mesh();
  explicit          Mesh(const uint32_t id);
  explicit          Mesh(const char *name);
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  Bounding_box      get_bounding_box() const;
  
  float *           get_positions_vec3() const;
  float *           get_normals_vec3() const;
  float *           get_texture_coords_vec2() const;
  size_t            get_vertex_count() const;
  size_t            get_triangle_count() const;
  
  uint32_t *        get_index() const;
  size_t            get_index_count() const;
  
  
private:
  

};


} // ns


#endif // inc guard
