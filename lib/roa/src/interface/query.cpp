#include <roa/query.hpp>
#include <roa/collection.hpp>
#include <roa/object.hpp>
#include <roa/ray.hpp>
#include <nil/data/bounding_box.hpp>
#include <math/math.hpp>
#include <vector>


namespace ROA {
namespace Query {


Collection
bounding_boxes(const Ray &ray, Ray_search search)
{
  size_t count = 0;
  Nil::Data::Bounding_box *data = nullptr;
  uint32_t *node_ids = nullptr;
  
  Nil::Data::get(&count, &data, true);
  Nil::Data::get(&count, &node_ids);
  
  const math::ray r = math::ray_init(
    math::vec3_init(ray.get_start().get_data()),
    math::vec3_init(ray.get_end().get_data())
  );
  
  float closest = math::float_max();
  ROA::Object closest_obj(nullptr);
  
  std::vector<ROA::Object> objs;
  
  for(size_t i = 0; i < count; ++i)
  {
    const math::aabb box = math::aabb_init(
      math::vec3_init(data[i].min),
      math::vec3_init(data[i].max)
    );
    
    const float dist = math::ray_test_aabb(r, box);
    
    if(dist != 0.f)
    {
      objs.emplace_back(ROA::Object(node_ids[i]));
      closest = math::min(dist, closest);
    }
  }
  
  return Collection(objs.data(), objs.size());
}


} // ns
} // ns
