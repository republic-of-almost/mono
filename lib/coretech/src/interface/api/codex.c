#include <coretech/api/codex.h>


struct ct_codex_window
{
  uint32_t width;
  uint32_t height;
  bool fullscreen;
  uintptr_t user_data;
};


struct ct_codex_ctx
{
  struct ct_codex_window *windows;
};


static struct ct_codex_ctx*
ct_codex_get_ctx()
{
  static struct ct_codex_ctx ctx;
  return &ctx;
}



/* ------------------------------------------------------ [ Codex Reader ] -- */


void ct_codex_get_reader(
  struct ct_codex *ex,
  struct ct_codex_reader *out_reader,
  uint32_t data, uint32_t instance)
{
}


void ct_codex_get_f(struct ct_codex_reader *reader, float *out_data)
{
}


void ct_codex_get_i(struct ct_codex_reader *reader, int32_t *out_data)
{
}


void ct_codex_get_b(struct ct_codex_reader *reader, bool *out_data)
{
}


void ct_codex_get_ud(struct ct_codex_reader *reader, uintptr_t *out_data)
{
}


/* ------------------------------------------------------ [ Codex Writer ] -- */


void ct_codex_get_writer(
  struct ct_codex *ex,
  struct ct_codex_writer *out_writer,
  uint32_t data, uint32_t instance)
{
}


void ct_codex_set_f(struct ct_codex_reader *write, float *out_data)
{
}


void ct_codex_set_i(struct ct_codex_reader *write, int32_t *out_data)
{
}


void ct_codex_set_b(struct ct_codex_reader *write, bool *out_data)
{
}


void ct_codex_set_ud(struct ct_codex_reader *write, uintptr_t *out_data)
{
}


void ct_codex_set_commit(struct ct_codex_writer *write)
{
}
