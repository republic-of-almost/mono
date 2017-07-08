#ifndef FWD_INCLUDED_22AF1857_0685_4D1B_849A_F3380F902EF1
#define FWD_INCLUDED_22AF1857_0685_4D1B_849A_F3380F902EF1


namespace Nil {


class Node;
class Engine;

class Node_list;
class Node_controller;

struct Aspect;


namespace Resource {

struct Audio;
struct Material;
struct Mesh;
struct Texture;

} // ns


namespace Data {


struct Audio;
struct Audio_resource;
struct Bounding_box;
struct Camera;
struct Collider;
struct Developer;
struct Gamepad;
struct Keyboard;
struct Light;
struct Logic;
struct Mesh;
struct Mesh_resource;
struct Mouse;
struct Renderable;
struct Resource;
struct Rigidbody;
struct Transform;
struct Window;


namespace Event {
enum ENUM {

  ADDED   = 1 << 0,
  UPDATED = 1 << 1,
  REMOVED = 1 << 2,
};
}

namespace Status {
enum ENUM {
  PENDING     = 1 << 0,
  ERROR       = 1 << 1,
  OK          = 1 << 2,
  UNKNOWN     = 1 << 3,
  NOT_FOUND   = 1 << 4,
};
}


} // ns
} // ns


#endif // inc guard
