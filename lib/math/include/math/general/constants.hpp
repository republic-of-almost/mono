#ifndef CONSTANTS_INCLUDED_6E0D4485_BD6A_403B_894A_A2FE4A509E40
#define CONSTANTS_INCLUDED_6E0D4485_BD6A_403B_894A_A2FE4A509E40


#include "../detail/detail.hpp"


// --------------------------------------------------------------- [ Config ] --


#ifndef MATH_CONST_CONSTEXPR
#define MATH_CONST_CONSTEXPR constexpr
#endif


_MATH_NS_OPEN


// ------------------------------------------------------------ [ Constants ] --


MATH_CONST_CONSTEXPR float           pi()          { return 3.14159265359f;  }
MATH_CONST_CONSTEXPR float           two_pi()      { return 2.f * pi();      }
MATH_CONST_CONSTEXPR float           half_pi()     { return pi() * 0.5f;     }
MATH_CONST_CONSTEXPR float           tau()         { return two_pi();        }
MATH_CONST_CONSTEXPR float           half_tau()    { return pi();            }
MATH_CONST_CONSTEXPR float           quart_tau()   { return half_pi();       }
MATH_CONST_CONSTEXPR float           g_ratio()     { return 1.61803398875f;  }
MATH_CONST_CONSTEXPR float           root_two()    { return 1.41421356237f;  }
MATH_CONST_CONSTEXPR float           root_three()  { return 1.73205080757f;  }
MATH_CONST_CONSTEXPR float           epsilon()     { return 0.000000001f;    }
MATH_CONST_CONSTEXPR float           ten_epsilon() { return epsilon() * 10.f;}
MATH_CONST_CONSTEXPR float           float_max()   { return __FLT_MAX__;     }
MATH_CONST_CONSTEXPR float           float_min()   { return -__FLT_MAX__;    }



_MATH_NS_CLOSE


#endif // inc guard
