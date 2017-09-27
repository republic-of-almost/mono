/*
  fwd.hpp
  --
  This files fwd declears all nil types and includes very common headers.
  It also contains some basic/common types.
  
  Be wary about what you put in here.
*/
#ifndef FWD_INCLUDED_22AF1857_0685_4D1B_849A_F3380F902EF1
#define FWD_INCLUDED_22AF1857_0685_4D1B_849A_F3380F902EF1


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


namespace Nil {


class Node;
class Engine;

class Node_list;
class Node_controller;

struct Aspect;


namespace Resource {

struct Audio;
struct Font;
struct Material;
struct Mesh;
struct Shader;
struct Texture;

enum class Load_status {
  NONE,
  PENDING,
  LOADED,
  FAILED,
  
  ERROR,
};

} // ns


namespace Data {


struct Audio;
struct Bounding_box;
struct Camera;
struct Developer;
struct Gamepad;
struct Keyboard;
struct Light;
struct Logic;
struct Mouse;
struct Renderable;
struct Renderable_text;
struct Rigidbody;
struct Transform;
struct Window;


namespace Event {
enum ENUM : uint32_t {

  ADDED   = 1 << 0,
  UPDATED = 1 << 1,
  REMOVED = 1 << 2,
};
}

//namespace Status {
//enum ENUM : uint32_t {
//  PENDING     = 1 << 0,
//  ERR         = 1 << 1,
//  OK          = 1 << 2,
//  UNKNOWN     = 1 << 3,
//  NOT_FOUND   = 1 << 4,
//};
//}


} // ns
} // ns


/* CAPI */

struct Nil_ctx;
struct Nil_shader;

typedef enum
{
  RSRC_STATUS_NONE,
  RSRC_STATUS_PENDING,
  RSRC_STATUS_LOADED,
  RSRC_STATUS_FAILED,
  RSRC_STATUS_ERROR
} Nil_resource_status;

typedef enum
{
  SHD_NONE,

  SHD_LINE_RENDERER_01,
  SHD_MESH_RENDERER_01,
  SHD_PAINT_RENDERER_01,
  
} Nil_shader_type;



typedef int NIL_BOOL;
#define NIL_TRUE 1
#define NIL_FALSE 0


#endif // inc guard
