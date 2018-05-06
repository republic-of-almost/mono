#ifndef ENCODE_INCLUDED_
#define ENCODE_INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


void roa_base64_encode(
  const unsigned char *data,
  unsigned input_length,
  unsigned char **output_data,
  unsigned *output_length);


void roa_base64_decode(
  const unsigned char *data,
  unsigned input_length,
  unsigned char **output_data,
  unsigned *output_length);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
