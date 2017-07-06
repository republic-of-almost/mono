#include <nil/resource/texture.hpp>
#include <lib/array.hpp>


// ------------------------------------------------------------- [ Resource ] --


namespace {


lib::array<uint32_t> keys;
lib::array<Nil::Resource::Texture> textures;


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


void
find_by_name(const char *name, Texture &out)
{
  
}


// ----------------------------------------------------------- [ Get / Load ] --


void
load(const char *name, Texture &in)
{
  keys.emplace_back((uint32_t)(keys.size() + 1));
  textures.emplace_back(in);
}


void
get(size_t *count, Texture **in_out)
{
  *count = keys.size();
  *in_out = textures.data();
}



} // ns
} // ns
