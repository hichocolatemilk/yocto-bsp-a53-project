#include "AES256-CBC.h"

// int main()
// {

//     unsigned char key[32] = {
//         0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
//         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
//         0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
//         0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
//     };
//     unsigned char iv[16] = {
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//     };

//     unsigned char *plaintext = (unsigned char *)"Hello, World!";
//     unsigned char ciphertext[128] = {0};
//     unsigned char decryptedtext[128] = {0};
//     int ciphertext_len = 0;
//     int decryptedtext_len = 0;

//     ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, iv, ciphertext);
//     if(ciphertext_len == -1)
//     {
//         printf("Encryption failed\n");
//         return -1;
//     }
//     printf("Encrypted text: %s\n", ciphertext);


//     decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
//     if(decryptedtext_len == -1)
//     {
//         printf("Decryption failed\n");
//         return -1;
//     }
//     printf("Decrypted text: %s\n", decryptedtext);
//     return 0;
// }


int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    int ret;
    ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
    {
        printf("ctx return %p\n", (void *)ctx);
        return -1;
    }

    ret = EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    if(ret != 1)
    {
        printf("EVP_EncryptInit_ex return %d\n", ret);
        return -1;
    }
    ret = EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    if(ret != 1)
    {
        printf("EVP_EncryptUpdate return %d\n", ret);
        return -1;
    }
    ciphertext_len = len;

    ret = EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    if(ret != 1)
    {
        printf("EVP_EncryptFinal_ex return %d\n", ret);
        return -1;
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *decryptedtext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int decryptedtext_len;
    int ret;
    ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
    {
        printf("ctx return %p\n", (void *)ctx);
        return -1;
    }

    ret = EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    if(ret != 1)
    {
        printf("EVP_DecryptInit_ex return %d\n", ret);
        return -1;
    }
    ret = EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, ciphertext_len);
    if(ret != 1)
    {
        printf("EVP_DecryptUpdate return %d\n", ret);
        return -1;
    }
    decryptedtext_len = len;

    ret = EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len);
    if(ret != 1)
    {
        printf("EVP_DecryptFinal_ex return %d\n", ret);
        return -1;
    }
    decryptedtext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return decryptedtext_len; // 0이 아니라 실제 길이를 반환!}
}