#ifndef TEXTURE_INCLUDED_4F51B28A_804B_4894_9AC0_43838A184317
#define TEXTURE_INCLUDED_4F51B28A_804B_4894_9AC0_43838A184317


#include <nil/resource/texture.hpp>
#include <lib/array.hpp>


struct Nil_texture_data
{
  lib::array<uint32_t, 128> keys;
  lib::array<Nil_texture, 128> textures;
};


#endif // inc guard
