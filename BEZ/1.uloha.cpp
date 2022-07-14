#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <stdbool.h>
#include <iostream>

using namespace std;

bool hashId (char text[], const EVP_MD *type, int *length) {
	unsigned char hash[EVP_MAX_MD_SIZE];
  EVP_MD_CTX *ctx;
	ctx = EVP_MD_CTX_create(); // create context for hashing
  if(ctx == NULL) exit(2);

  	int res;	
		res = EVP_DigestInit_ex(ctx, type, NULL); // context setup for our hash type
  		if(res != 1) exit(3);
  	res = EVP_DigestUpdate(ctx, text, strlen(text)); // feed the message in
  		if(res != 1) exit(4);
  	res = EVP_DigestFinal_ex(ctx, hash, (unsigned int *) length); // get the hash
  		if(res != 1) exit(5);

  	EVP_MD_CTX_destroy(ctx); // destroy the context
		if (hash[0] == 0xAA && hash[1] == 0xBB) {
  		printf("Pro text:\n");
  		for (int i = 0 ; i < 5 ; i++) {
  			printf("%02x", text[i]);
  		}
      printf("\n");
      printf("coz je: ");
      cout << text << endl;
      for (unsigned int i = 0 ; i < strlen((const char *)hash) ; i++) {
        printf("%02x", hash[i]);
      }
      cout << endl;
  		return true;
		}
	return false;
}

void tryHashing(char *text, const EVP_MD *type, int *length) {
  for (int i = 65 ; i < 122 ; i++) {//BLOGF
	 for (int j = 65 ; j < 122 ; j++) {
	 	 for (int k = 65 ; k < 122 ; k++) {
			 for (int l = 65 ; l < 122 ; l++) {
				  for (int m = 65 ; m < 122 ; m++) {
					 text[0] = i;
					 text[1] = j;
					 text[2] = k;
					 text[3] = l;
					 text[4] = m;
					 if (hashId(text, type, length)) {
						  return;
					 }
				  }
			 }
		  }
	 }
  }
}

int main(int argc, char *argv[]){
  //bude hledat zadany hash v 5ti mistnych retezcich
  char text[] = "AAAAA";
  char hashFunction[] = "sha256";  // zvolena hashovaci funkce ("sha1", "md5" ...)

  const EVP_MD *type; // typ pouzite hashovaci funkce
  int length;  // vysledna delka hashe

  /* Inicializace OpenSSL hash funkci */
  OpenSSL_add_all_digests();
  /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
  type = EVP_get_digestbyname(hashFunction);

  /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
  if(!type) {
    printf("Hash %s neexistuje.\n", hashFunction);
    exit(1);
  }

  tryHashing(text, type, &length);

  exit(0);
}