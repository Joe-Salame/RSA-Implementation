#ifndef OPERATIONS_H  
#define OPERATIONS_H "operations.h"

void square_and_multiply(mpz_t key,mpz_t z_number,mpz_t z_result,mpz_t z_N);
void square_and_multiply_ui(int power,mpz_t z_number,mpz_t z_result,mpz_t z_N);
void power(mpz_t key,mpz_t z_number,mpz_t z_result);
void power_ui(mpz_t z_result,mpz_t z_number,int pow);
void gmp_inverse(mpz_t z_nb1,mpz_t z_nb2,mpz_t z_v);
int gmp_primeCheck(mpz_t z_nb1,mpz_t z_nb2);
int test_miller_rabin(mpz_t z_num,int t);


#endif

