#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"/usr/include/openssl/evp.h"

void hash(const char *mode, const char* dataToHash, size_t dataSize, unsigned char* outHashed) {
    unsigned int md_len = -1;
    const EVP_MD *md = EVP_get_digestbyname(mode);
    if(NULL != md) {
        EVP_MD_CTX *mdctx;
        mdctx = EVP_MD_CTX_new();
        if (mdctx != NULL){
                EVP_MD_CTX_init(mdctx);
		EVP_DigestInit_ex(mdctx, md, NULL);
                EVP_DigestUpdate(mdctx, dataToHash, dataSize);
                EVP_DigestFinal_ex(mdctx, outHashed, &md_len);
                EVP_MD_CTX_free(mdctx);
        }
        else{
                printf("Something went wrong! wahid :( ");
        }

    }
    else{
        printf("Something went wrong! attaba :( ");
    }
}
