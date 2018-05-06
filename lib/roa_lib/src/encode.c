#include <roa_lib/encode.h>
#include <roa_lib/assert.h>
#include <roa_lib/fundamental.h>
#include <stdlib.h>


static uint8_t encoding_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/' };

static uint8_t *decoding_table = ROA_NULL;
static int mod_table[] = { 0, 2, 1 };


void build_decoding_table() {

  decoding_table = (char*)malloc(256);
  int i;
  for (i = 0; i < 64; i++)
    decoding_table[(unsigned char)encoding_table[i]] = i;
}


void
roa_base64_encode(
  const unsigned char *data,
  unsigned input_length,
  unsigned char **output_data,
  unsigned *output_length)
{
  /* param check */
  ROA_ASSERT(output_length);

  *output_length = 4 * ((input_length + 2) / 3);

  if(output_data)
  {
    unsigned char *encoded_data = *output_data;
    unsigned i, j;

    for (i = 0, j = 0; i < input_length;)
    {
      uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
      uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
      uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

      uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

      encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
      encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
      encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
      encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (i = 0; i < mod_table[input_length % 3]; i++)
      encoded_data[*output_length - 1 - i] = '=';
  }
}


void
roa_base64_decode(
  const unsigned char *data,
  unsigned input_length,
  unsigned char **output_data,
  unsigned *output_length)
{
  /* param check */
  ROA_ASSERT(output_length);

  if (decoding_table == NULL) { build_decoding_table(); }

  if (input_length % 4 != 0) return NULL;

  *output_length = input_length / 4 * 3;
  if (data[input_length - 1] == '=') (*output_length)--;
  if (data[input_length - 2] == '=') (*output_length)--;

  if(output_data)
  {
    unsigned char *decoded_data = *output_data;
    unsigned i,j;

    for (i = 0, j = 0; i < input_length;) {

      uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[(int)data[i++]];
      uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[(int)data[i++]];
      uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[(int)data[i++]];
      uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[(int)data[i++]];

      uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

      if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
      if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
      if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
  }
}