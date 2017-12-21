#ifndef CODEX_INCLUDED_C54E292E_C848_470B_B076_AAC5F4AEF89C
#define CODEX_INCLUDED_C54E292E_C848_470B_B076_AAC5F4AEF89C


struct codex_callback {
  void *function;
  void *user_data;
};


struct codex_ctx
{
  /* array */ unsigned *object_ids;
  /* array */ unsigned *parent_ids;
  
  unsigned object_instance_counter;

  /* array */ struct codex_callback *callbacks_created;
  /* array */ struct codex_callback *callbacks_destroyed;
};


#endif /* inc guard */