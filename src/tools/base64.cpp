#include "base64.h"
#include <stdio.h>
#include <string.h>

char *base64_encode(uint8_t *input,int inlen,uint8_t *output,int *outlen)
{
    const char *base64_tbl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int off = 0;
    int i, mod;

    mod = inlen % 3;
    for (i = 0; i < (inlen - mod); i+= 3) {
        output[off++] = base64_tbl[(input[i] >> 2) & 0x3F];//i [7,2]
        output[off++] = base64_tbl[( ((input[i] & 0x3) << 4) | (input[i+1] >> 4) ) & 0x3F];//i [1,0] i+1 [7,4]
        output[off++] = base64_tbl[( ((input[i+1] & 0xF) << 2) | (input[i+2] >> 6) ) & 0x3F];//i+1 [3,0] i+2 [7,6]
        output[off++] = base64_tbl[input[i+2] & 0x3F];//i+2 [5,0]
    }

    if (mod == 1) {
        output[off++] = base64_tbl[(input[i] >> 2) & 0x3F];//i [7,2]
        output[off++] = base64_tbl[(input[i] & 0x3) << 4];
        output[off++] = '=';
        output[off++] = '=';
    } else if (mod == 2) {
        output[off++] = base64_tbl[(input[i] >> 2) & 0x3F];//i [7,2]
        output[off++] = base64_tbl[( ((input[i] & 0x3) << 4) | (input[i+1] >> 4) ) & 0x3F];//i [1,0] i+1 [7,4]
        output[off++] = base64_tbl[(input[i+1] & 0xF) << 2];
        output[off++] = '=';
    }
    output[off] = '\0';
    if (NULL != outlen) {
        *outlen = off;
    }
    return output;
}

int base64_decode(uint8_t *input,int inlen,uint8_t *output,int *outlen)
{
    //解码需要一张反着的表
    const char *base64_tbl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int off = 0;
    int i, mod;
    uint8_t reverse_tbl[256] = {0};

    if (NULL == input) {
        return -1;
    }
    if (inlen == 0) {
        inlen = strlen(input);
    }
    if (inlen == 0 || inlen % 4 != 0) {
        /*error not a base64*/
        return -1;
    }

    for (i = 0; i < 64; i++) {
        reverse_tbl[base64_tbl[i]] = i;
    }

    for (i = 0; i < inlen - 4; i+= 4) {
        output[off++] = (reverse_tbl[input[i]] << 2) | (reverse_tbl[input[i + 1]] >> 4)  & 0xFF;
        output[off++] = (reverse_tbl[input[i+1]] << 4) | (reverse_tbl[input[i + 2]] >> 2)  & 0xFF;
        output[off++] = (reverse_tbl[input[i+2]] << 6) | (reverse_tbl[input[i + 3]])  & 0xFF;
    }

    if (input[i + 2] == '=') {
        output[off++] = (reverse_tbl[input[i]] << 2) | (reverse_tbl[input[i + 1]] >> 4)  & 0xFF;
    } else if(input[i + 3] == '=') {
        output[off++] = (reverse_tbl[input[i]] << 2) | (reverse_tbl[input[i + 1]] >> 4)  & 0xFF;
        output[off++] = (reverse_tbl[input[i+1]] << 4) | (reverse_tbl[input[i + 2]] >> 2)  & 0xFF;
    } else {
        output[off++] = (reverse_tbl[input[i]] << 2) | (reverse_tbl[input[i + 1]] >> 4)  & 0xFF;
        output[off++] = (reverse_tbl[input[i+1]] << 4) | (reverse_tbl[input[i + 2]] >> 2)  & 0xFF;
        output[off++] = (reverse_tbl[input[i+2]] << 6) | (reverse_tbl[input[i + 3]])  & 0xFF;
    }

    if (NULL != outlen) {
        *outlen = off;
    }
    return 0;
}

