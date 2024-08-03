#ifndef ENC_SIGN_DEC_VERIFY_H   
#define ENC_SIGN_DEC_VERIFY_H "enc_sign_dec_verify.h"

void digital_signature(FILE *plainData,mpz_t d,mpz_t N);
void verify_signature(mpz_t e,mpz_t N);
void encrypt(FILE *plainData,mpz_t e,mpz_t d,mpz_t N);
void decrypt(mpz_t e,mpz_t d,mpz_t N);

#endif

