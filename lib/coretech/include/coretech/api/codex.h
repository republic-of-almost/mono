#ifndef CODEX_INCLUDED_8D10ED39_925C_4500_AB85_268FED28E554
#define CODEX_INCLUDED_8D10ED39_925C_4500_AB85_268FED28E554


#include <coretech/fundamental.h>


/* ------------------------------------------------------ [ Codex Access ] -- */


enum {

  CT_CODEX_DATA_WINDOW,

};


enum {
  
  /* none */
  CT_CODEX_UNKNOWN,
  
  /* window api */
  CT_CODEX_FIELD_WINDOW_UD,
  CT_CODEX_FIELD_WINDOW_WIDTH_I1,
  CT_CODEX_FIELD_WINDOW_HEIGHT_I1,
  CT_CODEX_FIELD_WINDOW_FULLSCREEN_B1,
  
  /* object api */
  CT_CODEX_FIELD_OBJECT_POSITION_F3,
  CT_CODEX_FIELD_OBJECT_SCALE_F3,
  CT_CODEX_FIELD_OBJECT_ROTATION_F4,
};


/* ------------------------------------------------------ [ Codex Reader ] -- */


void ct_codex_get_reader(struct ct_codex *ex, struct ct_codex_reader *out_reader, uint32_t data, uint32_t instance);

void ct_codex_get_f(struct ct_codex_reader *reader, float *out_data);
void ct_codex_get_i(struct ct_codex_reader *reader, int32_t *out_data);
void ct_codex_get_b(struct ct_codex_reader *reader, bool *out_data);
void ct_codex_get_ud(struct ct_codex_reader *reader, uintptr_t *out_data);


/* ------------------------------------------------------ [ Codex Writer ] -- */


void ct_codex_get_writer(struct ct_codex *ex, struct ct_codex_writer *out_writer, uint32_t data, uint32_t instance);

void ct_codex_set_f(struct ct_codex_reader *write, float *out_data);
void ct_codex_set_i(struct ct_codex_reader *write, int32_t *out_data);
void ct_codex_set_b(struct ct_codex_reader *write, bool *out_data);
void ct_codex_set_ud(struct ct_codex_reader *write, uintptr_t *out_data);

void ct_codex_set_commit(struct ct_codex_writer *write);



#endif /* inc guard */
