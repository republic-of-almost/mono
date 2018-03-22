#ifndef SIGNAL_INCLUDED_3E3DE28D_72DD_4135_8961_6DA5A3CFDA70
#define SIGNAL_INCLUDED_3E3DE28D_72DD_4135_8961_6DA5A3CFDA70


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Signal Type ] -- */


typedef void* roa_signal;


/* --------------------------------------------------- [ Signal Lifetime ] -- */


roa_signal
roa_signal_create();


void
roa_signal_destroy(roa_signal *s);


/* ---------------------------------------------------- [ Signal Actions ] -- */


void
roa_signal_raise(roa_signal *s);


int
roa_signal_wait(roa_signal *s, int timeout_ms);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */