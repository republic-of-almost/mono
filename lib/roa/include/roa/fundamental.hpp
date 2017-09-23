/*
  Includes everything needed for the ROA interfaces and nothing else.
  Other than some standard includes don't include anything else here.
*/
#ifndef FUNDAMENTAL_INCLUDED_A0D602DD_313D_4456_93A0_10E453F3A2BA
#define FUNDAMENTAL_INCLUDED_A0D602DD_313D_4456_93A0_10E453F3A2BA


// ------------------------------------------------------------- [ Includes ] --


#include <stdint.h>
#include <stddef.h>


// ---------------------------------------------------------------- [ Types ] --


using ROA_nullptr = decltype(nullptr);


// ------------------------------------------------- [ Forward declerations ] --


namespace ROA {


class Application;
class Audio_player;
class Audio_source;
class Bounding_box;
class Camera;
class Collection;
class Component;
class Color;
class Light;
class Logic;
class Material;
class Mesh;
class Object;
class Quaternion;
struct Point;
class Ray;
class Renderable;
class Resource;
class Shader;
class Transform;
class Vector3;

enum class Ray_search;
enum class Resource_status;

} // ns


#endif // inc guard
