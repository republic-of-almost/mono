#ifndef CODEX_INCLUDED_C54E292E_C848_470B_B076_AAC5F4AEF89C
#define CODEX_INCLUDED_C54E292E_C848_470B_B076_AAC5F4AEF89C


struct codex_object_data
{
	/* array */ float *float_data;
	/* array */ int *integer_data;
	/* array */ unsigned *unsigned_data;
	/* array */ void **ptr_data;
};


struct codex_ctx
{
  /* array */ unsigned *object_ids;
  /* array */ unsigned *parent_ids;

  /* array */ struct codex_object_data object_data;
  
  unsigned object_instance_counter;
};


#endif /* inc guard */