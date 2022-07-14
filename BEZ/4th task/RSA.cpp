//schejond
#include <stdlib.h>
#include <openssl/evp.h>
#include <cmath>
#include <string>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <openssl/rand.h>
#include <openssl/pem.h>

using namespace std;

int main(int argc, char const *argv[]) {
//nacitani a kontrola vstupu
	if (argc != 5) {
		cout << "Nespravny vstup!" << endl;
		return 1;
	}
	if (*argv[1] != '-'
		|| (*(argv[1] + 1) != 'd' && *(argv[1] + 1) != 'e')) {
		cout << "Nespravny vstup!" << endl;
		return 1;
	}

  	if (RAND_load_file("/dev/random", 32) != 32) {
		puts("Cannot seed the random generator!");
	  	exit(1);
  	}

	const char* keyPathName = argv[2];
	const char* sourceFileName = argv[3];
	const char* outputFileName = argv[4];

	//nacteni vhodneho klice
	EVP_PKEY * key;
	FILE * fp = fopen(keyPathName, "rwb");
	if (*(argv[1] + 1) == 'e') {
		key = PEM_read_PUBKEY(fp, NULL, NULL, NULL);
	} else if (*(argv[1] + 1) == 'd') {
		key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
	}
	fclose(fp);

	//ziskani zasifrovaneho klice
	OpenSSL_add_all_ciphers();
	unsigned char * my_ek = (unsigned char *)malloc(EVP_PKEY_size(key));
	int key_length;
	if (*(argv[1] + 1) == 'd') {
		key_length = EVP_PKEY_size(key);
	}
	unsigned char iv[EVP_MAX_IV_LENGTH];

	EVP_PKEY_CTX * pctx = EVP_PKEY_CTX_new(key,NULL);
	if (!pctx) {
		cout << "Chyba pri vytvareni kontextu" << endl;
		return 1;
	}
	const EVP_CIPHER * cipher = EVP_des_cbc();
	if(!cipher) {
		cout << "Neznama sifra" << endl;
		return 1;
	}

	// Sifruj datovy soubor
	unsigned char buff[512];
	unsigned char buffOut[1024];
	int datalen = 0;
	int outlen = 0;

	FILE * INFILE = fopen(sourceFileName, "rw");
	if (!INFILE) {
		cout << "Nemohu otevrit vstupni soubor" << endl;
		return 1;
	}

	EVP_CIPHER_CTX ctx;
	//sifrovani
	if (*(argv[1] + 1) == 'e') {
		EVP_SealInit(&ctx, cipher, &my_ek, &key_length, iv, &key, 1);
		FILE * OUTFILE = fopen(outputFileName,"w");
		//zapsani hlavicky
		fwrite(iv, sizeof(unsigned char), EVP_MAX_IV_LENGTH, OUTFILE);
		fwrite(my_ek, sizeof(unsigned char), key_length, OUTFILE);
		
		int res;
		while((res = fread(buff,sizeof(unsigned char),strlen((const char*)buff),INFILE)) > 0 ) {
			if(!EVP_SealUpdate(&ctx,buffOut,&outlen,buff,res)) {
				cout << "Chyba v SealUpdate" << endl;
				return 1;
			}
			fwrite(buffOut,sizeof(unsigned char),outlen,OUTFILE);
			datalen += outlen;
		}

		if(!EVP_SealFinal(&ctx,buffOut,&outlen)) {
			cout << "Chyba pri SealFinal." << endl;
			return 1;
		}
		fwrite(buffOut,sizeof(unsigned char),outlen,OUTFILE);
		datalen += outlen;
		fclose(INFILE);
		fclose(OUTFILE);
	}
	//desifrace
	if (*(argv[1] + 1) == 'd') {
		//cteni hlavicky
		if(fread(iv,sizeof(unsigned char),EVP_MAX_IV_LENGTH,INFILE) != EVP_MAX_IV_LENGTH) {
			printf("Chyba pri nacitani IV.\n");
			return 1;
		}
		if(fread(my_ek,sizeof(unsigned char),key_length,INFILE) != (unsigned)key_length)
		{
			printf("Chyba pri nacitani klice.\n");
			return 1;
		}

		if(!EVP_OpenInit(&ctx, cipher, my_ek, key_length, iv, key)) {
			cout << "Chyba pri open init" << endl;
			return 1;
		}

		FILE * OUTFILE = fopen(outputFileName, "w");
		if(!OUTFILE) {
			cout << "Nelze otevrit vystupni soubor" << endl;
			return 1;
		}

		int res;
		while( (res = fread(buff,sizeof(unsigned char),strlen((char *)buff), INFILE)) > 0 ) {
			if(!EVP_OpenUpdate(&ctx, buffOut, &outlen, buff, res)) {
				cout << "Chyba pri OpenUpdate" << endl;
				return 1;
			}
			fwrite(buffOut,sizeof(unsigned char),outlen,OUTFILE);
			datalen += outlen;
		}
		if(!EVP_OpenFinal(&ctx, buffOut, &outlen)) {
			cout << "Chyba pri OpenFinal" << endl;
			return 1;
		}
	
		fwrite(buffOut,sizeof(unsigned char),outlen,OUTFILE);
		datalen += outlen;
	
		fclose(INFILE);
		fclose(OUTFILE);
	}
	free(my_ek);
	cout << "OK!" << endl;
	return 0;
}