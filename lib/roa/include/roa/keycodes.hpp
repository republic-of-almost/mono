#ifndef KEYCODES_INCLUDED_C644F8AB_B610_4B0D_9257_066851A09A52
#define KEYCODES_INCLUDED_C644F8AB_B610_4B0D_9257_066851A09A52


#include <roa/fundamental.hpp>


namespace ROA {

namespace KeyState {
enum ENUM : uint32_t {

  UP,
  UP_ON_FRAME,
  DOWN,
  DOWN_ON_FRAME,

}; // enum
} // ns

namespace KeyCode {
enum ENUM : uint32_t {

  // Alpha
  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  
  // Numeric
  ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ZERO,
  
  // Arrows
  UP, DOWN, LEFT, RIGHT,
  
  // Function Keys
  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19,
  
  // Modifiers
  L_SHIFT, R_SHIFT, L_CTRL, R_CTRL, CAPS_LOCK,
  
  // Other
  SPACE, ENTER, RETURN, ESCAPE,
  
  // Must be last
  COUNT
  
}; // enum
} // ns

} // ns


#endif // inc guard
