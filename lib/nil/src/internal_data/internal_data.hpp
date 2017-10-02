#ifndef DATA_INCLUDED_EC5AD2F3_79FA_4DF2_A66E_E49EA376C478
#define DATA_INCLUDED_EC5AD2F3_79FA_4DF2_A66E_E49EA376C478


#include <nil/fwd.hpp>
#include <nil/aspect.hpp>
#include <nil/task.hpp>
#include <internal_data/resources/texture.hpp>
#include <internal_data/resources/shader.hpp>
#include <internal_data/resources/mesh.hpp>
#include <graph/graph_fwd.hpp>
#include <lib/timer.hpp>


#ifndef NIL_MAX_ASPECT_COUNT
#define NIL_MAX_ASPECT_COUNT 16
#endif

#ifndef NIL_MAX_CPU_TASK_QUEUE
#define NIL_MAX_CPU_TASK_QUEUE 32
#endif

#ifndef NIL_MAX_GPU_TASK_QUEUE
#define NIL_MAX_GPU_TASK_QUEUE 16
#endif


struct Nil_aspect_callback
{
  Nil_aspect_callback_fn callback;
  void *user_data;
};


struct Nil_cpu_task
{
  Nil_cpu_task_fn callback;
  void *user_data;
};


struct Nil_gpu_task
{
  Nil_gpu_task_fn callback;
  void *user_data;
};


struct Nil_ctx
{
  /* --------------------------------------------------- [ Resource Data ] -- */
  
  
  Nil_texture_data*    rsrc_texture;
  Nil_shader_data*     rsrc_shader;
  Nil_mesh_data*       rsrc_mesh;
  
  
  /* ------------------------------------------------------- [ Node Data ] -- */

  

  /* ----------------------------------------------------- [ Aspect Data ] -- */


  Nil_aspect_callback aspect_startup_callbacks[NIL_MAX_ASPECT_COUNT];
  size_t              aspect_startup_callback_count;
  
  Nil_aspect_callback aspect_tick_callbacks[NIL_MAX_ASPECT_COUNT];
  size_t              aspect_tick_callback_count;
  
  Nil_aspect_callback aspect_shutdown_callbacks[NIL_MAX_ASPECT_COUNT];
  size_t              aspect_shutdown_callback_count;

  Nil_aspect_callback aspect_ui_window_callbacks[NIL_MAX_ASPECT_COUNT];
  size_t              aspect_ui_window_callback_count;

  Nil_aspect_callback aspect_ui_menu_callbacks[NIL_MAX_ASPECT_COUNT];
  size_t              aspect_ui_menu_callback_count;
  
  size_t              current_aspect_count;
  
  
  /* ------------------------------------------------------- [ Task Data ] -- */
  
  
  Nil_cpu_task        early_think_tasks[NIL_MAX_CPU_TASK_QUEUE];
  size_t              early_think_task_count;

  Nil_cpu_task        think_tasks[NIL_MAX_CPU_TASK_QUEUE];
  size_t              think_task_count;
  
  Nil_cpu_task        late_think_tasks[NIL_MAX_CPU_TASK_QUEUE];
  size_t              late_think_task_count;
  
  Nil_gpu_task        pre_render_tasks[NIL_MAX_GPU_TASK_QUEUE];
  size_t              pre_render_task_count;
  
  Nil_gpu_task        render_tasks[NIL_MAX_GPU_TASK_QUEUE];
  size_t              render_task_count;

  Nil_gpu_task        post_render_tasks[NIL_MAX_GPU_TASK_QUEUE];
  size_t              post_render_task_count;
  
  
  /* --------------------------------------------------------- [ Timeing ] -- */
  
  
  lib::milliseconds   last_tick;
  float               delta_time;


  /* ---------------------------------------------------------- [ Signal ] -- */

  
  bool                quit_signal;
  
  
  /* ------------------------------------------------------ [ Other Data ] -- */
  
  
  Nil::Graph::Data    *graph;
};



namespace Nil {
namespace Data {


Graph::Data*
get_graph_data();



} // ns
} // ns


#endif // inc guard
