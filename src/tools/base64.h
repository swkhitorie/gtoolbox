#ifndef USER_BASE64_H_
#define USER_BASE64_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

char *base64_encode(uint8_t *input, int inlen, uint8_t *output, int *outlen);
int base64_decode(uint8_t *input, int inlen, uint8_t *output, int *outlen);

#ifdef __cplusplus
}
#endif

#endif
