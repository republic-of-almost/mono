#ifndef FONT_INCLUDED_91656D56_5349_4E63_A1D5_33E523085CB9
#define FONT_INCLUDED_91656D56_5349_4E63_A1D5_33E523085CB9


#include <nil/resource/font.hpp>
#include <lib/array.hpp>


struct Font_data {
  lib::array<uint32_t>            keys{uint32_t{}};
  lib::array<Nil::Resource::Font> font{Nil::Resource::Font{}};
};


#endif // inc guard
