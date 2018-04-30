#ifndef REP_API_INCLUDED_37A05CEF_D8DD_4173_807E_5DC16131C13A
#define REP_API_INCLUDED_37A05CEF_D8DD_4173_807E_5DC16131C13A


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Basic Types ] -- */


#ifndef __cplusplus
#ifdef _MSC_VER
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif
#else
#include <cstdint>
#endif


typedef void(*rep_task)(void*);


/* ------------------------------------------------------- [ Application ] -- */


struct rep_app_desc
{
  const char *title;
  int width;
  int height;

  rep_task frame_job;
  void *frame_arg;
};


void
rep_app_create(
  const struct rep_app_desc * desc);


void
rep_app_get(
  struct rep_app_desc * out_desc);


void
rep_app_set(
  const struct rep_app_desc * desc);


void
rep_app_destroy();


/* -------------------------------------------------------------- [ Time ] -- */


struct rep_time_desc
{
  float delta_time;
};


void
rep_time_get(
  struct rep_time_desc * out_desc);


/* ------------------------------------------------------------- [ Input ] -- */


typedef enum rep_kb_id
{
  REP_KB_UNKNOWN,

  REP_KB_0, REP_KB_1, REP_KB_2, REP_KB_3, REP_KB_4, REP_KB_5, REP_KB_6,
  REP_KB_7, REP_KB_8, REP_KB_9,

  REP_KB_A, REP_KB_B, REP_KB_C, REP_KB_D, REP_KB_E, REP_KB_F, REP_KB_G,
  REP_KB_H, REP_KB_I, REP_KB_J, REP_KB_K, REP_KB_L, REP_KB_M, REP_KB_N,
  REP_KB_O, REP_KB_P, REP_KB_Q, REP_KB_R, REP_KB_S, REP_KB_T, REP_KB_U,
  REP_KB_V, REP_KB_W, REP_KB_X, REP_KB_Y, REP_KB_Z,

  REP_KB_UP, REP_KB_DOWN, REP_KB_LEFT, REP_KB_RIGHT,

  REP_KB_F1, REP_KB_F2, REP_KB_F3, REP_KB_F4, REP_KB_F5, REP_KB_F6,
  REP_KB_F7, REP_KB_F8, REP_KB_F9, REP_KB_F10, REP_KB_F11, REP_KB_F12,
  REP_KB_F13, REP_KB_F14, REP_KB_F15,

  REP_KB_ESC, REP_KB_TAB,

  REP_KB_COUNT
} rep_kb_id;


typedef enum rep_keystate
{
  REP_KEYSTATE_UP, REP_KEYSTATE_FRAME_UP,
  REP_KEYSTATE_DOWN, REP_KEYSTATE_FRAME_DOWN,
} rep_keystate;


struct rep_keyboard_desc
{
  rep_keystate *keys;
  int key_count;
};


typedef enum rep_ms_button_id
{
  REP_MS_LEFT, REP_MS_RIGHT, REP_MS_MIDDLE,

  REP_MS_COUNT,
} rep_msbutton_id;


struct rep_mouse_desc
{
  rep_keystate *ms_buttons;
  int button_count;

  float position[2];
  float delta[2];
  float scroll[2];
};


typedef enum rep_gp_button_id
{
  REP_GP_A, REP_GP_B, REP_GP_Y, REP_GP_X,
  REP_GP_DPAD_UP, REP_GP_DPAD_DOWN, REP_GP_DPAD_LEFT, REP_GP_DPAD_RIGHT,
  REP_GP_LEFT_AXIS, REP_GP_RIGHT_AXIS,
  REP_GP_LEFT_BUMPER, REP_GP_RIGHT_BUMPER,
  REP_GP_START, REP_GP_MENU,

  REP_GP_COUNT
} rep_msbutton_id;


struct rep_gamepad_desc
{
  rep_keystate *gp_buttons;

  float triggers[2];

  float axis_1[2];
  float axis_2[2];
};


struct rep_input_desc
{
  struct rep_keyboard_desc *kb;
  int kb_count;

  struct rep_mouse_desc *ms;
  int ms_count;

  struct rep_gamepad_desc *gp;
  int gp_count;
};


void
rep_input_get(
  struct rep_input_desc *out_desc);


/* -------------------------------------------------------------- [ Task ] -- */


struct rep_task_desc
{
  rep_task function;
  void *arg;
};


uint64_t
rep_task_submit(
  struct rep_task_desc * tasks,
  unsigned count);


void
rep_task_wait(
  uint64_t marker);


/* ------------------------------------------------------------ [ Object ] -- */


struct rep_object_desc
{
	uint32_t object_parent;
};


void
rep_object_create(
  uint32_t * out_object_ids,
  const struct rep_object_desc * desc,
  unsigned count);


void
rep_object_update(
  const uint32_t * object_ids,
  const struct rep_object_desc * desc,
  unsigned count);


void
rep_object_destroy(
  const uint32_t * object_ids,
  unsigned count);


/* --------------------------------------------------------- [ Transform ] -- */


struct rep_transform_desc
{
  float position[3];
  float scale[3];
  float rotation[4];
};


void
rep_transform_set(
  uint32_t * object_ids,
  struct rep_transform_desc * desc,
  unsigned count);


void
rep_transform_get(
  const uint32_t * object_ids,
  struct rep_transform_desc * desc,
  unsigned count);


/* ------------------------------------------------------------ [ Camera ] -- */


struct rep_camera_desc
{
  unsigned width;
  unsigned height;
  float fov;
};


void
rep_camera_set(
  uint32_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count);


void
rep_camera_get(
  const uint32_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count);


/* --------------------------------------------------- [ Mesh Renderable ] -- */


struct rep_mesh_renderable_desc
{
  uint64_t material_id;
  uint64_t mesh_id;
};


void
rep_mesh_renderable_set(
  uint32_t * object_ids,
  struct rep_mesh_renderable_desc * desc,
  unsigned count);


void
rep_mesh_renderable_get(
  const uint32_t * object_ids,
  struct rep_mesh_renderable_desc * desc,
  unsigned count);


/* -------------------------------------------------------------- [ Mesh ] -- */


void
rep_mesh_find(
  uint64_t * out_mesh_ids,
  const char ** mesh_names,
  unsigned mesh_name_count);


/* ---------------------------------------------------------- [ Material ] -- */


void
rep_material_find(
  uint64_t * out_material_ids,
  const char ** material_names,
  unsigned material_name_count);


#ifdef __cplusplus
} /* extern C */
#endif


#endif /* inc guard */
