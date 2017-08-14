#include <roa/query.hpp>
#include <roa/collection.hpp>
#include <roa/object.hpp>
#include <roa/ray.hpp>
#include <nil/data/bounding_box.hpp>
#include <math/math.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>
#include <vector>


namespace ROA {
namespace Query {


Collection
bounding_boxes(const Ray &ray, Ray_search search)
{
  // -- Param Check -- //
  LOG_TODO_ONCE("Check ray is valid");

  // -- Get Data -- //
  size_t bb_count               = 0;
  Nil::Data::Bounding_box *data = nullptr;
  size_t node_id_count = 0;
  uint32_t *node_ids   = nullptr;
  {
    Nil::Data::get(&bb_count, &data, true);
    Nil::Data::get(&node_id_count, &node_ids);
    
    
    LIB_ASSERT(node_id_count);
    LIB_ASSERT(node_ids);
  }
  
  // -- Data for test -- //
  const math::ray r = math::ray_init(
    math::vec3_init(ray.get_start().get_data()),
    math::vec3_init(ray.get_end().get_data())
  );
  
  float closest = math::float_max();
  ROA::Object closest_obj(nullptr);
  
  std::vector<ROA::Object> objs;
  
  // -- Search For Ray Collisions -- //
  for(size_t i = 0; i < bb_count; ++i)
  {
    const uint32_t node_id = node_ids[i];
    LIB_ASSERT(node_id != 0);
  
    const math::aabb box = math::aabb_init(
      math::vec3_init(data[i].min),
      math::vec3_init(data[i].max)
    );
    
    const float dist = math::ray_test_aabb(r, box);
    
    if(dist > 0.f)
    {
      if(dist < closest)
      {
        const ROA::Object obj = ROA::Object(node_ids[i]);
        closest_obj = obj;
      }
      
      objs.emplace_back(ROA::Object(node_ids[i]));
      closest = math::min(dist, closest);
    }
  }
  
  // -- Results -- //
  {
    if(Ray_search::ALL == search)
    {
      return Collection(objs.data(), objs.size());
    }
    else if(Ray_search::NEAREST == search && closest_obj.is_valid())
    {
      return Collection(&closest_obj, 1);
    }
  }
  
  return Collection(nullptr, 0);
}


} // ns
} // ns
