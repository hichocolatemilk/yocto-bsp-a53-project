#include "sha256.h"


int main()
{
    unsigned char data[] = "Hello, World!";
    unsigned char hash[32];
    sha256(data, strlen((char *)data), hash);
    printf("SHA-256 hash: ");
    for(int i = 0; i < 32; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");   
    return 0;
}

void sha256(const unsigned char *data, size_t data_len, unsigned char *hash)
{
    EVP_MD_CTX *mdctx;

    if((mdctx = EVP_MD_CTX_new()) == NULL)
    {
        printf("EVP_MD_CTX_new failed\n");
        return;
    }
    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
    {
        printf("EVP_DigestInit_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }
    if(1 != EVP_DigestUpdate(mdctx, data, data_len))
    {
        printf("EVP_DigestUpdate failed\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }
    if(1 != EVP_DigestFinal_ex(mdctx, hash, NULL))
    {
        printf("EVP_DigestFinal_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }
    EVP_MD_CTX_free(mdctx);
}