#include <roa/query.hpp>
#include <roa/collection.hpp>
#include <roa/ray.hpp>
#include <nil/data/bounding_box.hpp>
#include <math/math.hpp>


namespace ROA {
namespace Query {


Collection
bounding_boxes(const Ray &ray, Ray_search search)
{
  size_t count = 0;
  Nil::Data::Bounding_box *data = nullptr;

  Nil::Data::get(&count, &data, true);
  
  const math::ray r = math::ray_init(
    math::vec3_init(ray.get_start().get_data()),
    math::vec3_init(ray.get_end().get_data())
  );
  
  float closest = math::float_max();
  
  for(size_t i = 0; i < count; ++i)
  {
    const math::aabb box = math::aabb_init(
      math::vec3_init(data[i].min),
      math::vec3_init(data[i].max)
    );
    
    const float dist = math::ray_test_aabb(r, box);
    
    if(dist != 0.f)
    {
      closest = math::min(dist, closest);
    }
  }
}


} // ns
} // ns
