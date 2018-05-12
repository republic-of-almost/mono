#ifndef ALLOC_INCLUDED_A848C93D_B1BE_4DCC_8F9C_38445128E811
#define ALLOC_INCLUDED_A848C93D_B1BE_4DCC_8F9C_38445128E811


void * rdr_alloc(unsigned bytes);
void   rdr_free(void *);
void * rdr_frame_alloc(unsigned bytes);


#endif /* inc guard */
