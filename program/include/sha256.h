#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

void sha256(const unsigned char *data, size_t data_len, unsigned char *hash);