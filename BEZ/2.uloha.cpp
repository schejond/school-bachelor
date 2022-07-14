#include <stdlib.h>
#include <openssl/evp.h>
#include <string>
#include <string.h>
#include <iostream>

using namespace std;

void encryptMessage(unsigned char* ot1, unsigned char* st, const EVP_CIPHER * cipher, const unsigned char* key, const unsigned char* iv, EVP_CIPHER_CTX *ctx, int* otLength1, int* stLength1, int*tmpLength1) {
  int res;

  /* Sifrovani */
  res = EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);  // context init - set cipher, key, init vector
  if(res != 1) exit(3);
  res = EVP_EncryptUpdate(ctx,  st, tmpLength1, ot1, *otLength1);  // encryption of pt
  if(res != 1) exit(4);
  *stLength1 += *tmpLength1;
  res = EVP_EncryptFinal_ex(ctx, st + *stLength1, tmpLength1);  // get the remaining ct
  if(res != 1) exit(5);
  *stLength1 += *tmpLength1;

  // printf ("Zasifrovano %d znaku.\n", *stLength1);
}

void decryptMessage(unsigned char* ot1, unsigned char* st, const EVP_CIPHER * cipher, const unsigned char* key, const unsigned char* iv, EVP_CIPHER_CTX *ctx, int* otLength1, int* stLength1, int*tmpLength1){
  int res;
  /* Desifrovani */
  res = EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);  // nastaveni kontextu pro desifrovani
  if(res != 1) exit(6);
  res = EVP_DecryptUpdate(ctx, ot1, tmpLength1,  st, *stLength1);  // desifrovani st
  if(res != 1) exit(7);
  *otLength1 += *tmpLength1;
  res = EVP_DecryptFinal_ex(ctx, ot1 + *otLength1, tmpLength1);  // dokonceni (ziskani zbytku z kontextu)
  if(res != 1) exit(8);
  *otLength1 += *tmpLength1;
}

unsigned char hexToDecASCII(const unsigned char val) { 
  if (val >= 48 && val <= 57) {
    return val - 48; 
  }
  if (val >= 65 && val <= 70) {
    return val - 65 + 10;
  }
  if (val >= 97 && val <= 103) {
    return val - 97 + 10;
  }
  cout << "error: " << (unsigned)val << endl;
  exit(1); 

  return 255;
}

//prevede a zarovna ST do normalni podoby
void putHexaToDec(unsigned char* st, unsigned char* res) {
  unsigned char tmp1;
  unsigned char tmp2;
  unsigned char decElem = 1;
  unsigned indexResult = 0;
  unsigned indexTmp1 = 0;

  //pokud je delka licha, dej nulu zleva
  if (strlen((const char*)st)%2) {
    tmp1 = 0;
    tmp2 = st[0];
    decElem = hexToDecASCII(tmp1) * 16 + hexToDecASCII(tmp2);
    res[indexResult] = decElem;
    indexTmp1 += 1;
    indexResult++;
  }
  if (strlen((const char*)st) >= 2) {
    while (indexTmp1 <= strlen((const char*)st) - 1) {
      tmp1 = st[indexTmp1];
      tmp2 = st[indexTmp1 + 1];
      decElem = hexToDecASCII(tmp1) * 16 + hexToDecASCII(tmp2);
      res[indexResult] = decElem;
      indexTmp1 += 2;    
      indexResult++;
    }
  }
}

int main(int argc, char *argv[]) {
  char command[2];
  unsigned char ot1[1024] = "Skryty text.";
  unsigned char ot2[1024] = "Verejny.";
  unsigned char st1[1024];
  unsigned char st2[1024];
  unsigned char st1Dec[1024];
  unsigned char st2Dec[1024];
  unsigned char result[1024];

  if (argc != 2) {
      cout << "spatny vstup" << endl;
      return 1;
  }

  if (argc == 2 &&
      (sscanf(argv[1], "%s", command) != 1 || command[0] != '-' || (command[1] != 'e' && command[1] != 'd'))) {
      cout << "spatny vstup" << endl;
      return 1;
  }

  if (command[1] == 'd') {
    //vycistim si pole
    for (int i = 0; i < 1024; i++) {
    st1[i] = ot1[i] = st2[i] = st1Dec[i] = st2Dec[i] = result[i] = '\0';
    }
    if (scanf(" %1024s %1024s %1024s", ot1, st1, st2) != 3) {
      cout << "spatny vstup" << endl;
      return 1;      
    }

    putHexaToDec(st1, st1Dec);
    putHexaToDec(st2, st2Dec);

    //najdu si ten nejkratsi text
    unsigned shortestArray = strlen((const char*)ot1);
    if (shortestArray > strlen((const char*)st1Dec)) {
      shortestArray = strlen((const char*)st1Dec);
    }
    if (shortestArray > strlen((const char*)st2Dec)) {
      shortestArray = strlen((const char*)st2Dec);
    }

    for (unsigned i = 0 ; i < shortestArray ; i++) {
      result[i] = ot1[i] ^ st1Dec[i] ^ st2Dec[i];
    }

    cout << "Umim desifrovat text delky: " << shortestArray << endl;
    cout << "Result OT: " << result << endl;
    return 0;
  }

  //prepinac je e
  cout << "OT1: " << ot1 << endl;
  cout << "OT2: " << ot2 << endl;
  unsigned char key[EVP_MAX_KEY_LENGTH] = "Muj klic";  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH] = "inicial. vektor";  // inicializacni vektor
  const char cipherName[] = "RC4";
  const EVP_CIPHER * cipher;

  OpenSSL_add_all_ciphers();
  /* sifry i hashe by se nahraly pomoci OpenSSL_add_all_algorithms() */
  cipher = EVP_get_cipherbyname(cipherName);
  if(!cipher) {
    printf("Sifra %s neexistuje.\n", cipherName);
    exit(1);
  }

  int otLength1 = strlen((const char*) ot1);
  int stLength1 = 0;
  int tmpLength1 = 0;

  EVP_CIPHER_CTX *ctx1; // context structure
  ctx1 = EVP_CIPHER_CTX_new();
  if (ctx1 == NULL) exit(2);

  encryptMessage(ot1, st1, cipher, key, iv, ctx1, &otLength1, &stLength1, &tmpLength1);

  decryptMessage(ot1, st1, cipher, key, iv, ctx1, &otLength1, &stLength1, &tmpLength1);

  //druha zprava
  int otLength2 = strlen((const char*) ot2);
  int stLength2 = 0;
  int tmpLength2 = 0;

  encryptMessage(ot2, st2, cipher, key, iv, ctx1, &otLength2, &stLength2, &tmpLength2);

  decryptMessage(ot2, st2, cipher, key, iv, ctx1, &otLength2, &stLength2, &tmpLength2);
  EVP_CIPHER_CTX_free(ctx1);

  // for (unsigned int i = 0 ; i < strlen((const char*)ot2) ; i++) {
  //   printf("%c", ot2[i]);
  // }
  // cout << endl;
  cout << "ST1: ";
  for (unsigned int i = 0 ; i < strlen((const char*)st1) ; i++) {
    printf("%02x", st1[i]);
  }
  cout << endl;
  cout << "ST2: ";
  for (unsigned int i = 0 ; i < strlen((const char*)st2) ; i++) {
    printf("%02x", st2[i]);
  }  
  cout << endl;

  exit(0);
}