#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<gmp.h>
#include"operations.h"
#include"hash.h"
#include"/usr/include/openssl/evp.h"


void digital_signature(FILE *plainData,mpz_t d,mpz_t N){

	unsigned int blockSize=20;
	char *block= (char*)malloc(blockSize*sizeof(char));
	char *signature_hexa=NULL;
	char *data=NULL;
	size_t dataCurLen=0;
	char chr;
	unsigned char Hash[32];
	char Hash_hexa[64];

	mpz_t z_hashDecimal,z_signature;

	mpz_inits(z_hashDecimal,z_signature,NULL);

	FILE *digital_signature;
	digital_signature=fopen("digital_signature.txt","w");
        if(digital_signature==NULL){
        	printf("digital_signature.txt file didnt open!\n");
        	exit(1);
        }
	while(fgets(block,blockSize+1,plainData)){
		data=(char*) realloc(data,dataCurLen+strlen(block)+1);
		for(int i=0;i<strlen(block);i++){
			data[i+dataCurLen]=block[i];
		}
		data[strlen(block)+dataCurLen]='\0';
		dataCurLen+=strlen(block);
	}

	hash("SHA256",data,strlen(data),Hash);
	for(int i=0;i<32;i++) {
    		sprintf(&Hash_hexa[i*2],"%02X",Hash[i]);
	}

	mpz_set_str(z_hashDecimal,Hash_hexa, 16);
	//mpz_powm(z_signature,z_hashDecimal,d,N);
	square_and_multiply(d,z_hashDecimal,z_signature,N);
	
	signature_hexa=mpz_get_str(NULL,16,z_signature);

	fprintf(digital_signature,"%s\n",signature_hexa);

	fclose(digital_signature);
	free(data);
	mpz_clears(z_hashDecimal,z_signature,NULL);

}

void verify_signature(mpz_t e,mpz_t N){

	unsigned int blockSize=20;
        char *block= (char*)malloc(blockSize*sizeof(char));
        char *hashToCheck=NULL;
        char *data=NULL;
        size_t dataCurLen=0;
        char chr;
        unsigned char Hash[32];
        char Hash_hexa[64];

	
	FILE *decrypt,*signature;

	mpz_t z_signature,z_signature_final;

	mpz_inits(z_signature,z_signature_final,NULL);

	decrypt=fopen("decrypt.txt","a+");
        if(decrypt==NULL){
                printf("decrypt.txt file didnt open!\n");
                exit(1);
	}
	
	signature=fopen("digital_signature.txt","r");
	if(signature==NULL){
		printf("digital_signature.txt file didnt open!\n");
		exit(1);
	}

	mpz_inp_str(z_signature,signature,16);
	//mpz_powm(z_signature_final,z_signature,e,N);
	square_and_multiply(e,z_signature,z_signature_final,N);

	hashToCheck=mpz_get_str(NULL,16,z_signature_final);
	
	while(fgets(block,blockSize+1,decrypt)){
                data=(char*) realloc(data,dataCurLen+strlen(block)+1);
                for(int i=0;i<strlen(block);i++){
                        data[i+dataCurLen]=block[i];
                }
                data[strlen(block)+dataCurLen]='\0';
                dataCurLen+=strlen(block);
        }

	hash("SHA256",data,strlen(data),Hash);
	for(int i=0;i<32;i++) {
                sprintf(&Hash_hexa[i*2],"%02x",Hash[i]);
        }

	if(strcmp(Hash_hexa,hashToCheck)==0){
		//printf("VALID SIGNATURE!\n");
		fputs("\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::||||VALID SIGNATURE!||||::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\r\n",decrypt);
	}
	else{
		//printf("INVALID SIGNATURE!\n");
		fputs("\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::||||INVALID SIGNATURE!||||::::::::::::::::::::::::::::::::::::::::::::::::::::::::\r\n",decrypt);

	}

	mpz_init(z_signature);
	fclose(decrypt);
	fclose(signature);

}


void encrypt(FILE *plainData,mpz_t e,mpz_t d,mpz_t N){
	
	clock_t start = clock();
	unsigned int blockSize=20;
	char *block=(char*)malloc(blockSize*sizeof(char));
	char chr;
	int done=0;

	FILE *encrypted,*check;
	encrypted=fopen("cipher.txt","w");
        if(encrypted==NULL){
        	printf("cipher.txt file didnt open!\n");
        	exit(1);
        }
	check=fopen("check.txt","w");
        if(check==NULL){
                printf("File cipher didnt open!\n");
                exit(1);
        }


        unsigned int c;
        char ciph;
	mpz_t z_block,z_power,z_multi,z_nb256,z_cipher,z_c;
	
	while(fgets(block,blockSize+1,plainData)){
		if(strlen(block)<20){
                        for(int i=strlen(block);i<20;i++){
                                block[i]='`';
                        }
                }
		mpz_inits(z_block,z_nb256,z_power,z_multi,z_cipher,z_c,NULL);
        	mpz_set_ui(z_block,256);
        	mpz_set_ui(z_nb256,0);

		for(int i=0;i<blockSize;i++){
                	int asc=(int)block[i];
                	mpz_set_ui(z_multi,1);
                	mpz_pow_ui(z_power,z_block,i);
                	mpz_mul_ui(z_multi,z_power,asc);
                	mpz_add(z_nb256,z_nb256,z_multi);
       		}

		//mpz_powm(z_cipher,z_nb256,e,N);
		square_and_multiply(e,z_nb256,z_cipher,N);
		

		while(1){
                	mpz_mod_ui(z_c,z_cipher,256);
                	c=mpz_get_ui(z_c);

                        fprintf(encrypted,"%u\n",c);
                		
			mpz_div_ui(z_cipher,z_cipher,256);

                	if(mpz_cmp_ui(z_cipher,0)==0){//lezem tkoun cipher
                        	break;
                	}
        	}
		fprintf(encrypted,"-\n");
	}
	fclose(encrypted);
	fclose(check);
        mpz_clears(z_block,z_nb256,z_power,z_c,z_multi,z_cipher,NULL);
	fseek(plainData,0,SEEK_SET);
	clock_t end = clock();
	float time=(float)(end-start)/CLOCKS_PER_SEC;
	printf("the time of the encryption RSA mode standard with built-in functions: %f\n",time);

	start=clock();
	//Sign the document
	digital_signature(plainData,d,N);
	end=clock();
	time=(float)(end-start)/CLOCKS_PER_SEC;
	printf("the time of the signature RSA mode standard with built-in functions: %f\n",time);
}



void decrypt(mpz_t e,mpz_t d,mpz_t N){
	
	clock_t start = clock();
	FILE *cipher,*decrypt;
	char *line=NULL;
	size_t len;

	int ascii,check,done=0,i;
	mpz_t z_block,z_power,z_multi,z_nb256,z_plain,z_result,z_ascii,z_tmp;

	cipher=fopen("cipher.txt","r");
	if(cipher==NULL){
		printf("cipher.txt file didnt open!\n");
		exit(1);
	}

	decrypt=fopen("decrypt.txt","w");
        if(decrypt==NULL){
                printf("decrypt.txt file didnt open!\n");
                exit(1);
        }

	while(1){
		mpz_inits(z_block,z_nb256,z_power,z_multi,z_ascii,z_tmp,NULL);

        	mpz_set_ui(z_block,256);
        	mpz_set_ui(z_nb256,0);
		
		i=0;
		while(1){
			check=getline(&line,&len,cipher);
			if(check==-1){
				done=1;
				break;
			}
			if(strcmp(line,"-\n")==0){
				break;
			}
			ascii=atoi(line);
			mpz_pow_ui(z_power,z_block,i);
			//power_ui(z_power,z_block,i);
                	mpz_mul_ui(z_multi,z_power,ascii);
                	mpz_add(z_nb256,z_nb256,z_multi);
			i++;
			free(line);
			line=NULL;
			len=0;
		}
		if(mpz_cmp_ui(z_nb256,0)!=0){
			mpz_powm(z_nb256,z_nb256,d,N);//8ayera			     
			//mpz_set(z_tmp,z_nb256);
			//square_and_multiply(d,z_tmp,z_nb256,N);

			while(1){
                		mpz_mod_ui(z_ascii,z_nb256,256);
                		ascii=mpz_get_ui(z_ascii);
                		if(ascii==96){
                        		break;
                		}	
                		else{
                        		fprintf(decrypt,"%c",(char)ascii);
                		}
                		mpz_div_ui(z_nb256,z_nb256,256);

                		if(mpz_cmp_ui(z_nb256,0)==0){
                        		break;
                		}
        		}
		}
		if(done==1){
			break;
		}
	}
	fclose(cipher);
	fclose(decrypt);
	mpz_clears(z_block,z_nb256,z_power,z_multi,z_ascii,NULL);
	clock_t end = clock();
	float time=(float)(end-start)/CLOCKS_PER_SEC;
        printf("the time of the decryption RSA mode standard with built-in functions: %f\n",time);

	start= clock();
	//Verift the signature
	verify_signature(e,N);
	end = clock();
        time=(float)(end-start)/CLOCKS_PER_SEC;
        printf("the time of the signature verification RSA mode standard with built-in functions: %f\n",time);
}

