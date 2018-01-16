#ifndef ROA_CONTEXT_INCLUDED_1C790A98_2094_4F2E_A039_E2259440A77C
#define ROA_CONTEXT_INCLUDED_1C790A98_2094_4F2E_A039_E2259440A77C


struct roa_context
{
  struct optio_dispatcher_ctx *optio_ctx;
  struct GLFWwindow *glfw_window;

  void *user_frame_func;
  void *user_frame_arg;

  int quit;
};


struct roa_context*
roa_ctx();


#endif /* inc guard */