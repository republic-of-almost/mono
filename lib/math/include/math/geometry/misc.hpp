#ifndef MISC_INCLUDED_F161A14F_DB99_49FD_82A1_58EF2B4F889E
#define MISC_INCLUDED_F161A14F_DB99_49FD_82A1_58EF2B4F889E


#include "../detail/detail.hpp"
#include "../vec/vec3.hpp"
#include "../general/general.hpp"


_MATH_NS_OPEN


// ------------------------------------------------------------ [ Interface ] --


inline vec3        get_vec3_origin(const float verts[], const size_t count);


// ----------------------------------------------------------------- [ Impl ] --


inline vec3
get_vec3_origin(const float verts[], const size_t count)
{
  if(count > 1)
  {
    constexpr float v_large = 100000000000;
    float min_pos[3]{+v_large, +v_large, +v_large};
    float max_pos[3]{-v_large, -v_large, -v_large};
    
    for(int i = 1; i < count; ++i)
    {
      min_pos[0] = math::min(verts[(i * 3) + 0], min_pos[0]);
      min_pos[1] = math::min(verts[(i * 3) + 1], min_pos[1]);
      min_pos[2] = math::min(verts[(i * 3) + 2], min_pos[2]);
      
      max_pos[0] = math::max(verts[(i * 3) + 0], max_pos[0]);
      max_pos[1] = math::max(verts[(i * 3) + 1], max_pos[1]);
      max_pos[2] = math::max(verts[(i * 3) + 2], max_pos[2]);
    }
    
    return vec3_init(
      min_pos[0] + (max_pos[0] - min_pos[0]),
      min_pos[1] + (max_pos[1] - min_pos[1]),
      min_pos[2] + (max_pos[2] - min_pos[2])
    );
  }
  
  return vec3_zero();
}



_MATH_NS_CLOSE


#endif // inc guard
