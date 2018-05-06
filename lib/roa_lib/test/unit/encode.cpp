#include <roa_lib/encode.h>
#include <roa_lib/fundamental.h>
#include <catch/catch.hpp>
#include <cstdlib>
#include <cstring>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Encode")
{
  unsigned char data_to_encode[] = "Something 123 Blah!";

  /* encode */

  unsigned encode_size = 0;

  roa_base64_encode(
    ROA_ARR_DATA(data_to_encode),
    ROA_ARR_COUNT(data_to_encode),
    ROA_NULL,
    &encode_size);

  unsigned char *encoded_data = (unsigned char *)malloc(encode_size);

  roa_base64_encode(
    ROA_ARR_DATA(data_to_encode),
    ROA_ARR_COUNT(data_to_encode),
    &encoded_data,
    &encode_size);

  /* decode */

  unsigned decode_size = 0;

  roa_base64_decode(
    encoded_data,
    encode_size,
    ROA_NULL,
    &decode_size);

  unsigned char *decoded_data = (unsigned char *)malloc(decode_size);

  roa_base64_decode(
    encoded_data,
    encode_size,
    &decoded_data,
    &decode_size);

  REQUIRE(strcmp((const char*)&data_to_encode[0], (const char*)decoded_data) == 0);
}
