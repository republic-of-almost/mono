#ifndef AUDIO_RESOURCE_INCLUDED_D65D15B0_1D6A_4636_876E_AC3F7CCA86B1
#define AUDIO_RESOURCE_INCLUDED_D65D15B0_1D6A_4636_876E_AC3F7CCA86B1


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Audio_resource
{
  uint32_t id;
  uint32_t status;
  
  char *filename;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Audio_resource &data);


void
set(Node &node, const Audio_resource &data);


void
remove_audio_resource(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_audio_resource(const Node &node);


uint64_t
get_type_id(const Audio_resource &data);


size_t
audio_resource_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Audio_resource **out_data, Node **out_node);



} // ns
} // ns


#endif // inc guard
