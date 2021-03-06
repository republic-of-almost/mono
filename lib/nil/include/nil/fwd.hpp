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

class Node_list;
class Node_controller;



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

} // ns
} // ns


/* CAPI */


/* engine */

struct Nil_ctx;
struct Nil_aspect;

/* data */



/* resources */

struct Nil_audio_src;
struct Nil_font;
struct Nil_material;
struct Nil_mesh;
struct Nil_shader;
struct Nil_texture;
struct Nil_material;
struct Nil_font;

/* callback prototypes */

typedef void(*Nil_aspect_callback_fn)(Nil_ctx *ctx, void *self);
typedef void(*Nil_cpu_task_fn)(Nil_ctx *ctx, void *user_data);
typedef void(*Nil_gpu_task_fn)(Nil_ctx *ctx, void *user_data);

/* state identifiers */

typedef enum
{
  NIL_RSRC_STATUS_NONE,
  NIL_RSRC_STATUS_PENDING,
  NIL_RSRC_STATUS_LOADED,
  NIL_RSRC_STATUS_FAILED,
  NIL_RSRC_STATUS_ERROR
} Nil_resource_status;

typedef enum
{
  NIL_SHD_NONE,
  NIL_SHD_LINE_RENDERER_01,
  NIL_SHD_MESH_RENDERER_01,
  NIL_SHD_PAINT_RENDERER_01,
  
} Nil_shader_type;

typedef enum
{
  NIL_CPU_TASK_EARLY_THINK,
  NIL_CPU_TASK_THINK,
  NIL_CPU_TASK_LATE_THINK,
} Nil_cpu_task_queue;

typedef enum
{
  NIL_GPU_TASK_PRE_RENDER,
  NIL_GPU_TASK_RENDER,
  NIL_GPU_TASK_POST_RENDER,
} Nil_gpu_task_queue;


typedef enum
{
  NIL_AUDIO_SRC_SAMPLE,
  NIL_AUDIO_SRC_SONG,
} Nil_audio_src_type;


typedef enum
{
  NIL_DATA_FILENAME,
  NIL_DATA_RAW,
  NIL_DATA_LOCAL,

} Nil_data_type;


typedef int NIL_BOOL;
#define NIL_TRUE 1
#define NIL_FALSE 0


#endif // inc guard
