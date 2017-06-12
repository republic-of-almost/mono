#ifndef AUDIO_RESOURCE_INCLUDED_D65D15B0_1D6A_4636_876E_AC3F7CCA86B1
#define AUDIO_RESOURCE_INCLUDED_D65D15B0_1D6A_4636_876E_AC3F7CCA86B1


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Audio_resource
{
  uint32_t id;
  uint32_t status;
  
  char *filename;
};


void
get(const Node &node, Audio_resource &data);


void
set(Node &node, const Audio_resource &data);


bool
has_audio_resource(const Node &node);


uint64_t
get_type_id(const Audio_resource &data);


} // ns
} // ns


#endif // inc guard
