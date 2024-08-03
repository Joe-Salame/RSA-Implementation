#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gmp.h>
#include"enc_sign_dec_verify.h"
#include"/usr/include/openssl/evp.h"

int main(int argc, char *argv[]){
	OpenSSL_add_all_algorithms();
	if(argc!=2){
		printf("Please input the documents name!\n");
		exit(1);
	}

	FILE *plainData,*keys,*decipher;
	char *e_str,*N_str,*d_str;
	ssize_t len;
	mpz_t e,d,N;

	mpz_inits(e,d,N,NULL);

	plainData=fopen(argv[1],"r");
	if(plainData==NULL){
		printf("%s file didnt open!\n",argv[1]);
		exit(1);
	}

	keys=fopen("keys.txt","r");
	if(keys==NULL){
		printf("keys.txt file didnt open!\n");
		exit(1);
	}

	//fetch the public key
        getline(&e_str,&len,keys);
        e_str=strtok(e_str," ");
        for(int i=0;i<2;i++){
                e_str=strtok(NULL," ");
        }
        mpz_set_str(e,e_str,0);

        //fetch the modular
        getline(&N_str,&len,keys);
        N_str=strtok(N_str," ");
        for(int i=0;i<2;i++){
                N_str=strtok(NULL," ");
        }
        mpz_set_str(N,N_str,0);

        //fetch the private key
        getline(&d_str,&len,keys);
        d_str=strtok(d_str," ");
        for(int i=0;i<2;i++){
                d_str=strtok(NULL," ");
        }
        mpz_set_str(d,d_str,0);
        fclose(keys);

	//encrypt the document
	encrypt(plainData,e,d,N);
	
	//cipher Modification
	/*FILE *modification=fopen("cipher.txt","a+");
        if(modification==NULL){
                printf("cipher.txt file didnt open!\n");
                exit(1);
        }
	fputs("65\n77\n-",modification);
	fclose(modification);*/
	
	//decrypt the document
	decrypt(e,d,N);



}
