#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<gmp.h>
#include "operations.h"


int main(int argc,char *argv[]){

if(argc!=3){
	printf("please insert k and e");
	exit(1);
}

mpz_t z_a;
mpz_t z_b;
mpz_t z_gcd;

mpz_init(z_a);
mpz_init(z_b);
mpz_init(z_gcd);

unsigned int k=atoi(argv[1]);

mpz_set_ui(z_a,2);
mpz_set_ui(z_b,2);

mpz_pow_ui(z_a,z_a,k-1);
mpz_pow_ui(z_b,z_b,k);
mpz_sub_ui(z_b,z_b,1);

char *private_keys="keys.txt";
FILE *params;
char *e_str,*n_str,*d_str,*p_str,*q_str,*Ip_str;
char *line=NULL;
ssize_t len;


mpz_t z_e;
mpz_t z_q,z_p,z_Ip,z_n,z_d,z_fN;
mpz_t z_fq,z_fp;

mpz_inits(z_e,z_q,z_p,z_Ip,z_n,z_fN,z_fq,z_fp,z_d,NULL);

mpz_set_ui(z_e,atoi(argv[2]));

mpz_t z_r;

mpz_init(z_r);
mpz_sub(z_r,z_b,z_a);
mpz_add_ui(z_r,z_r,1);

gmp_randstate_t state;
gmp_randinit_default(state);

gmp_randseed_ui(state,time(NULL));
unsigned int prime;

while(1){
	mpz_urandomm(z_q,state,z_r);
	mpz_add(z_q,z_q,z_a);
	prime=test_miller_rabin(z_q,5);

	if(prime==0){
		continue;		
	}
	else{
		mpz_sub_ui(z_fq,z_q,1);
		if(gmp_primeCheck(z_fq,z_e)!=1){
			continue;
		}
	}
	break;
}


while(1){
        mpz_urandomm(z_p,state,z_r);
        mpz_add(z_p,z_p,z_a);
        prime=test_miller_rabin(z_p,5);

        if(prime==0){
                continue;
        }
        else{
                mpz_sub_ui(z_fp,z_p,1);
                if(gmp_primeCheck(z_fp,z_e)!=1){
                        continue;
                }
		
        }
	if(mpz_cmp(z_q,z_p)==0){
		continue;
	}
        break;
}

//gmp_printf("p:%Zu\n",z_p);
//gmp_printf("q:%Zu\n\n",z_q);

clock_t start=clock();
mpz_mul(z_n,z_q,z_p);
mpz_mul(z_fN,z_fq,z_fp);

gmp_inverse(z_e,z_fN,z_d);//d
gmp_inverse(z_p,z_q,z_Ip);//Ip
clock_t end=clock();

float time=(float)(end-start)/CLOCKS_PER_SEC;
printf("The key generation duration time mode CRT is: %f\n\n",time);



//gmp_printf("d:%Zu\n",z_d);
//gmp_printf("e:%Zu\n",z_e);
//gmp_printf("Ip:%Zu\n",z_Ip);

params=fopen(private_keys,"w");
if(params==NULL){
	printf("params write error!");
	exit(1);
}

e_str=mpz_get_str(NULL,16,z_e);
n_str=mpz_get_str(NULL,16,z_n);
d_str=mpz_get_str(NULL,16,z_d);
p_str=mpz_get_str(NULL,16,z_p);
q_str=mpz_get_str(NULL,16,z_q);
Ip_str=mpz_get_str(NULL,16,z_Ip);

fprintf(params,"e = 0x%s\n",e_str);
fprintf(params,"n = 0x%s\n",n_str);
fprintf(params,"d = 0x%s\n",d_str);
fprintf(params,"p = 0x%s\n",p_str);
fprintf(params,"q = 0x%s\n",q_str);
fprintf(params,"Ip = 0x%s\n",Ip_str);

fclose(params);

params=fopen(private_keys,"r");
if(params==NULL){
      printf("params read error!");
      exit(1);
}

for(int i=0;i<6;i++){
	getline(&line,&len,params);
	printf("%s",line);
	line=NULL;
}

fclose(params);
gmp_randclear(state);
mpz_clears(z_e,z_n,z_d,z_gcd,z_a,z_b,z_q,z_p,z_fN,z_fq,z_fp,z_r,z_Ip,NULL);


}
