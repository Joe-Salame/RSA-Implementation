#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<gmp.h>


void square_and_multiply(mpz_t key, mpz_t z_number, mpz_t z_result, mpz_t z_N) {
    char *binary_key = mpz_get_str(NULL, 2, key);
    mpz_set(z_result, z_number);

    for (int i = 1; i < strlen(binary_key); i++) {
        char key_bit = binary_key[i];
        mpz_mul(z_result, z_result, z_result);
        mpz_mod(z_result, z_result, z_N);

        if (key_bit == '1') {
            mpz_mul(z_result, z_result, z_number);
            mpz_mod(z_result, z_result, z_N);
        }
    }
    free(binary_key);
}

void square_and_multiply_ui(int power,mpz_t z_number,mpz_t z_result,mpz_t z_N){

        mpz_t z_power;

        mpz_init(z_power);

        mpz_set_ui(z_power,power);

        char *binary_power;
        binary_power=mpz_get_str(NULL,2,z_power);
        mpz_set(z_result,z_number);

        for(int i=1;i<strlen(binary_power);i++){
                char bit_power=binary_power[i];
                mpz_mul(z_result,z_result,z_result);
                mpz_mod(z_result,z_result,z_N);

                if(bit_power=='1'){
                        mpz_mul(z_result,z_result,z_number);
                        mpz_mod(z_result,z_result,z_N);
                }
        }
}

void power(mpz_t key,mpz_t z_number,mpz_t z_result){
        char *binary_key;
        binary_key=mpz_get_str(NULL,2,key);
        mpz_set(z_result,z_number);
        for(int i=1;i<strlen(binary_key);i++){
                char key_bit=binary_key[i];
                mpz_mul(z_result,z_result,z_result);
                if(key_bit=='1'){
                        mpz_mul(z_result,z_result,z_number);
                }
        }
}


void power_ui(mpz_t z_result, mpz_t z_number, unsigned int pow) {
    mpz_t z_pow;
    mpz_init(z_pow);
    mpz_set_ui(z_pow, pow);

    char *exp = mpz_get_str(NULL, 2, z_pow);
    mpz_set(z_result, z_number);

    if(pow==0){
        mpz_set_ui(z_result,1);
        return;
    }


    for (int i = 1; i < strlen(exp); i++) {
        mpz_mul(z_result, z_result, z_result);
        if (exp[i] == '1') {
            mpz_mul(z_result, z_result, z_number);
        }
    }

    mpz_clear(z_pow);
    free(exp);
}



void gmp_inverse(mpz_t z_nb1,mpz_t z_nb2,mpz_t z_v){

        mpz_t nb1,nb2,z_u;
        mpz_t z_remainder,z_quotient;
        mpz_t u_prev,v_prev,u_pprev,v_pprev;
        mpz_t mul_q_u,mul_q_v,mul_q_nb2;

        mpz_inits(nb1,nb2,z_u,NULL);
        mpz_inits(z_remainder,z_quotient,NULL);
        mpz_inits(mul_q_u,mul_q_v,mul_q_nb2,NULL);
        mpz_inits(u_prev,v_prev,u_pprev,v_pprev,NULL);

        mpz_set(nb1,z_nb1);
        mpz_set(nb2,z_nb2);
	mpz_set_ui(u_prev,1);
        mpz_set_ui(v_prev,0);
        mpz_set_ui(u_pprev,0);
        mpz_set_ui(v_pprev,1);

        while(mpz_cmp_ui(z_remainder,1)!=0){
                mpz_div(z_quotient,nb1,nb2);
                mpz_mul(mul_q_nb2,z_quotient,nb2);
                mpz_sub(z_remainder,nb1,mul_q_nb2);
                mpz_mul(mul_q_u,z_quotient,u_pprev);
                mpz_sub(z_u,u_prev,mul_q_u);
                mpz_mul(mul_q_v,z_quotient,v_prev);
                mpz_sub(z_v,v_pprev,mul_q_v);

                mpz_set(nb1,nb2);
                mpz_set(nb2,z_remainder);
                mpz_set(u_pprev,u_prev);
                mpz_set(v_pprev,v_prev);
                mpz_set(u_prev,z_u);
                mpz_set(v_prev,z_v);
        }
	
	mpz_mod(z_v,z_v,z_nb2);
        //gmp_printf("%Zu\n",z_v);
}



int gmp_primeCheck(mpz_t z_nb1,mpz_t z_nb2){
        mpz_t nb1,nb2;
        mpz_t z_u,z_v;
        mpz_t z_remainder,z_quotient;
        mpz_t u_prev,v_prev,u_pprev,v_pprev;
        mpz_t mul_q_u,mul_q_v,mul_q_nb2;

        mpz_inits(nb1,nb2,z_u,z_v,NULL);
        mpz_inits(z_remainder,z_quotient,NULL);
        mpz_inits(mul_q_u,mul_q_v,mul_q_nb2,NULL);
        mpz_inits(u_prev,v_prev,u_pprev,v_pprev,NULL);

        mpz_set(nb1,z_nb1);
        mpz_set(nb2,z_nb2);
        mpz_set_ui(u_prev,1);
        mpz_set_ui(v_prev,0);
        mpz_set_ui(u_pprev,0);
        mpz_set_ui(v_pprev,1);
	
	while(1){

                mpz_div(z_quotient,nb1,nb2);
                mpz_mul(mul_q_nb2,z_quotient,nb2);
                mpz_sub(z_remainder,nb1,mul_q_nb2);
                mpz_mul(mul_q_u,z_quotient,u_pprev);
                mpz_sub(z_u,u_prev,mul_q_u);
                mpz_mul(mul_q_v,z_quotient,v_prev);
                mpz_sub(z_v,v_pprev,mul_q_v);
                mpz_set(nb1,nb2);
                mpz_set(nb2,z_remainder);

                mpz_set(u_prev,u_pprev);
                mpz_set(v_prev,v_pprev);
                mpz_set(u_pprev,z_u);
                mpz_set(v_pprev,z_v);

                if(mpz_cmp_ui(z_remainder,0)==0){
                        break;
                }
        }

        if(mpz_cmp_ui(nb1,1)!=0){
                return 0;
        }
        else{
                return 1;
        }
}


int test_miller_rabin(mpz_t z_num,int t){

        int i=1,j=1;
        mpz_t z_prevNum,z_res,z_n,z_s,z_r,z_base;
        mpz_t z_baseCond,z_result,z_minBase,z_div2;

        mpz_init(z_n);
        mpz_init(z_s);
        mpz_init(z_r);
        mpz_init(z_div2);
        mpz_init(z_res);
        mpz_init(z_minBase);
        mpz_init(z_base);
        mpz_init(z_prevNum);
        mpz_init(z_baseCond);

        mpz_init(z_result);

        mpz_set_ui(z_minBase,2);
        mpz_set(z_n,z_num);
        mpz_set_ui(z_prevNum,1);
        mpz_sub_ui(z_n,z_n,1);
       	mpz_sub_ui(z_baseCond,z_num,1);
        mpz_sub(z_baseCond,z_baseCond,z_minBase);

        gmp_randstate_t state;
        gmp_randinit_default(state);
        gmp_randseed_ui(state,time(NULL));

        mpz_set_ui(z_div2,2);
        //gmp_printf("%Zu\n",z_base);

        while(1){
                power_ui(z_div2,z_div2,i);
                mpz_mod(z_res,z_n,z_div2);
                if(mpz_cmp_ui(z_res,0)==1){
                        break;
                }
                mpz_div(z_prevNum,z_num,z_div2);
                i++;
        }
        if(i!=1){
                i--;
        }
        mpz_set_ui(z_s,i);
	mpz_sub_ui(z_s,z_s,1);
        mpz_set(z_r,z_prevNum);
        //gmp_printf("r:%Zu s:%Zu\n",z_r,z_s);
        mpz_t z_pow;

        mpz_init(z_pow);
        for(int i=0;i<t;i++){
                mpz_urandomm(z_base,state,z_baseCond);
                mpz_add(z_base,z_base,z_minBase);
		square_and_multiply(z_r,z_base,z_result,z_num);
		if(mpz_cmp_ui(z_result,1)!=0 && mpz_cmp(z_result,z_n)!=0){
                        j=1;
               		 while(mpz_cmp(z_result,z_n)!=0 && (mpz_cmp_ui(z_s,j)==0 || mpz_cmp_ui(z_s,j)>0)){
                        	
				square_and_multiply_ui(2,z_result,z_result,z_num);
                        	if(mpz_cmp_ui(z_result,1)==0){
                                	return 0;
                        	}
                        	j+=1;
                	}
                if(mpz_cmp(z_result,z_n)!=0){
                        return 0;
                        }

                }
        }
        return 1;
}

void prime_gen(mpz_t z_q,mpz_t z_prime,int t,int k){

        mpz_t z_i,z_pi,z_res;
        mpz_t *p=(mpz_t *)malloc(k * sizeof(mpz_t));
        mpz_t *w=(mpz_t *)malloc(k * sizeof(mpz_t));
        int f,j=k,z=0;


        mpz_init(z_i);
        mpz_init(z_res);
        for(int x=0;x<k;x++){
                mpz_init(p[x]);
        }

        for(int x=0;x<k;x++){
                mpz_init(w[x]);
        }
	
	mpz_set_ui(z_i,2);
	for(int i=0;i<2;i++){
		mpz_set(p[z],z_i);
        	z++;
		j--;
		mpz_add_ui(z_i,z_i,1);
	}


        while(1){
                f=test_miller_rabin(z_i,5);
                if(f!=0){
                        j--;
                        mpz_set(p[z],z_i);
                        z++;
                }
                if(j==0){
                         break;
                }
		mpz_add_ui(z_i,z_i,1);
        }

        for(int j=0;j<k;j++){
                mpz_mod(z_res,z_q,p[j]);
                mpz_set(w[j],z_res);
        }

        int repeat;
        while(1){
                while(1){
                        repeat=0;
                        for(int j=0;j<k;j++){
                                if(mpz_cmp_ui(w[j],0)==0){
                                        repeat=1;
                                        for(int j=0;j<k;j++){
                                                mpz_add_ui(w[j],w[j],2);
                                        }
                                        mpz_add_ui(z_q,z_q,2);
                                }
                        }
                        if(repeat==0){
                                break;
                        }

                }
                int result=test_miller_rabin(z_q,t);
                //gmp_printf("%d %Zu\n",result,z_q);
		if(result==0){
                        for(int j=0;j<k;j++){
                                mpz_add_ui(w[j],w[j],2);
                        }
                        mpz_add_ui(z_q,z_q,2);
                        continue;
                }
                else{
                        mpz_set(z_prime,z_q);
                        return;
                }

        }
}




