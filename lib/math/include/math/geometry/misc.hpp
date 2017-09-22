#ifndef MISC_INCLUDED_F161A14F_DB99_49FD_82A1_58EF2B4F889E
#define MISC_INCLUDED_F161A14F_DB99_49FD_82A1_58EF2B4F889E


#include "../detail/detail.hpp"
#include "../vec/vec3.hpp"
#include "../general/general.hpp"


// --------------------------------------------------------------- [ Config ] --
/*
  Inline hint
*/

#ifndef MATH_MISC_INLINE
#define MATH_MISC_INLINE inline
#endif


_MATH_NS_OPEN


// ------------------------------------------------------------ [ Interface ] --


MATH_MISC_INLINE vec3        get_vec3_origin(const float xyz_verts[], const size_t vert_count);


// ----------------------------------------------------------------- [ Impl ] --


vec3
get_vec3_origin(const float xyz_verts[], const size_t vert_count)
{
  if(vert_count > 1)
  {
    constexpr float v_large = 100000000000;
    float min_pos[3]{+v_large, +v_large, +v_large};
    float max_pos[3]{-v_large, -v_large, -v_large};
    
    for(int i = 1; i < vert_count; ++i)
    {
      const size_t index = i * 3;
      
      MATH_ASSERT(i < vert_count);
    
      min_pos[0] = MATH_NS_NAME::min(xyz_verts[index + 0], min_pos[0]);
      min_pos[1] = MATH_NS_NAME::min(xyz_verts[index + 1], min_pos[1]);
      min_pos[2] = MATH_NS_NAME::min(xyz_verts[index + 2], min_pos[2]);
      
      max_pos[0] = MATH_NS_NAME::max(xyz_verts[index + 0], max_pos[0]);
      max_pos[1] = MATH_NS_NAME::max(xyz_verts[index + 1], max_pos[1]);
      max_pos[2] = MATH_NS_NAME::max(xyz_verts[index + 2], max_pos[2]);
    }
    
    return MATH_NS_NAME::vec3_init(
      min_pos[0] + (max_pos[0] - min_pos[0]),
      min_pos[1] + (max_pos[1] - min_pos[1]),
      min_pos[2] + (max_pos[2] - min_pos[2])
    );
  }
  
  return vec3_zero();
}



_MATH_NS_CLOSE


#endif // inc guard
