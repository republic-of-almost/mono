/*
 Keyboard
 --
 This is a little hard to abstract into data only. So this will have to be
 application specific.
 
 You pass a ptr and count back.
 This can be a direct one to one lookup.
 
 Or iterate through only active keys.
*/
#ifndef KEYBOARD_INCLUDED_7F779786_CB96_43BB_97F1_188BF322F487
#define KEYBOARD_INCLUDED_7F779786_CB96_43BB_97F1_188BF322F487


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


namespace KeyState {
enum ENUM {

  UP,
  UP_ON_FRAME,
  DOWN,
  DOWN_ON_FRAME,

}; // enum
} // ns


namespace KeyCode {
enum ENUM {

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


inline const char*
keycode_to_string(const uint32_t keycode)
{
  switch(keycode)
  {
    // Alpha
    case(KeyCode::A):     return "A";
    case(KeyCode::B):     return "B";
    case(KeyCode::C):     return "C";
    case(KeyCode::D):     return "D";
    case(KeyCode::E):     return "E";
    case(KeyCode::F):     return "F";
    case(KeyCode::G):     return "G";
    case(KeyCode::H):     return "H";
    case(KeyCode::I):     return "I";
    case(KeyCode::J):     return "J";
    case(KeyCode::K):     return "K";
    case(KeyCode::L):     return "L";
    case(KeyCode::M):     return "M";
    case(KeyCode::N):     return "N";
    case(KeyCode::O):     return "O";
    case(KeyCode::P):     return "P";
    case(KeyCode::Q):     return "Q";
    case(KeyCode::R):     return "R";
    case(KeyCode::S):     return "S";
    case(KeyCode::T):     return "T";
    case(KeyCode::U):     return "U";
    case(KeyCode::V):     return "V";
    case(KeyCode::W):     return "W";
    case(KeyCode::X):     return "X";
    case(KeyCode::Y):     return "Y";
    case(KeyCode::Z):     return "Z";
    
    // Numeric
    case(KeyCode::ONE):    return "ONE";
    case(KeyCode::TWO):    return "TWO";
    case(KeyCode::THREE):  return "THREE";
    case(KeyCode::FOUR):   return "FOUR";
    case(KeyCode::FIVE):   return "FIVE";
    case(KeyCode::SIX):    return "SIX";
    case(KeyCode::SEVEN):  return "SEVEN";
    case(KeyCode::EIGHT):  return "EIGHT";
    case(KeyCode::NINE):   return "NINE";
    case(KeyCode::ZERO):   return "ZERO";
    
    // Arrows
    case(KeyCode::UP):     return "UP";
    case(KeyCode::DOWN):   return "DOWN";
    case(KeyCode::LEFT):   return "LEFT";
    case(KeyCode::RIGHT):  return "RIGHT";
    
    // Function Keys
    case(KeyCode::F1):   return "F1";
    case(KeyCode::F2):   return "F2";
    case(KeyCode::F3):   return "F3";
    case(KeyCode::F4):   return "F4";
    case(KeyCode::F5):   return "F5";
    case(KeyCode::F6):   return "F6";
    case(KeyCode::F7):   return "F7";
    case(KeyCode::F8):   return "F8";
    case(KeyCode::F9):   return "F9";
    case(KeyCode::F10):  return "F10";
    case(KeyCode::F11):  return "F11";
    case(KeyCode::F12):  return "F12";
    case(KeyCode::F13):  return "F13";
    case(KeyCode::F14):  return "F14";
    case(KeyCode::F15):  return "F15";
    case(KeyCode::F16):  return "F16";
    case(KeyCode::F17):  return "F17";
    case(KeyCode::F18):  return "F18";
    case(KeyCode::F19):  return "F19";
    
    // Modifiers
    case(KeyCode::L_SHIFT):   return "L_SHIFT";
    case(KeyCode::R_SHIFT):   return "R_SHIFT";
    case(KeyCode::L_CTRL):    return "L_CTRL";
    case(KeyCode::R_CTRL):    return "R_CTRL";
    case(KeyCode::CAPS_LOCK): return "CAPS_LOCK";
    
    // Other
    case(KeyCode::SPACE):     return "SPACE";
    case(KeyCode::ENTER):     return "ENTER";
    case(KeyCode::RETURN):    return "RETURN";
    case(KeyCode::ESCAPE):    return "ESCAPE";
  
    // Else
    default:              return "Unknown Key";
  }
}


struct Keyboard
{
  uint32_t  id;
  uint32_t* key_state;
  size_t    key_count;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Keyboard &out);


void
set(Node &node, const Keyboard &in);


void
remove_keyboard(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_keyboard(const Node &node);


uint64_t
get_type_id(const Keyboard &in);


size_t
keyboard_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Keyboard **out_data, Node **out_node);



} // n
} // ns


#endif // inc guard
