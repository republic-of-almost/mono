#include <nil/resource/material.hpp>
#include <lib/array.hpp>


namespace {


lib::array<uint32_t> keys;
lib::array<Nil::Resource::Material> materials;


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


void
find_by_name(const char *name, Material &out)
{
  
}


// ----------------------------------------------------------- [ Get / Load ] --


void
load(const char *name, Material &in_out)
{
  materials.emplace_back(in_out);
}


void
get(size_t *count, Material **out)
{
  *count = keys.size();
  *out = materials.data();
}



} // ns
} // ns
