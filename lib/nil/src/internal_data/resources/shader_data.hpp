#ifndef SHADER_INCLUDED_A6CCEEC8_C089_4FF8_8AEB_61A152D6C4B3
#define SHADER_INCLUDED_A6CCEEC8_C089_4FF8_8AEB_61A152D6C4B3


#include <nil/resource/shader.hpp>
#include <lib/array.hpp>


struct Nil_shader_data
{
  lib::array<uint32_t, 128> keys{uint32_t{0}};
  lib::array<Nil_shader, 128> shader{Nil_shader{}};
};


#endif // inc guard
