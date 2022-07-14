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

using namespace std;

enum Mode {ecb, cbc};

int main(int argc, char const *argv[]) {
//nacitani a kontrola vstupu
	//kontrola vstupu
	if (argc != 4) {
		cout << "Nespravny vstup!" << endl;
		return 1;
	}
	if (*argv[1] != '-'
		|| (*(argv[1] + 1) != 'd' && *(argv[1] + 1) != 'e')) {
		cout << "Nespravny vstup!" << endl;
		return 1;
	}
	Mode mode;
	const char* modeName = argv[2];
	if (modeName[3] == '\0' && modeName[2] == 'b'
		&& modeName[1] == 'c' && modeName[0] == 'e') {
			mode = Mode::ecb;
	} else if (modeName[3] == '\0' && modeName[2] == 'c'
		&& modeName[1] == 'b' && modeName[0] == 'c') {
		mode = Mode::cbc;
	} else {
		cout << "Neznama sifra!" << endl;
		return 1;
	}
	const char* srcFileName = argv[3];

	ifstream INFILE;
	INFILE.open(srcFileName, ios::in | ios::binary);

	INFILE.seekg (0, INFILE.end);
	const unsigned inputFileSize = INFILE.tellg();
	// cout << "Vstupni obr ma velikost: " << inputFileSize << endl;
	INFILE.seekg (0, INFILE.beg);

	if (!INFILE.is_open()) {
		cout << "Chyba - nepodarilo se otevrit vstupni soubor!" << endl;
		return 1;
	}

	// output file name
	string outputFileName(srcFileName);
	outputFileName.pop_back();
	outputFileName.pop_back();
	outputFileName.pop_back();
	outputFileName.pop_back();
	if (mode == Mode::cbc)
		outputFileName += "_cbc";
	else if (mode == Mode::ecb)
		outputFileName += "_ecb";
	if (*(argv[1] + 1) == 'd')
		outputFileName += "_dec";
	outputFileName += ".bmp";
//----//loads head of the image
	//magic head
	uint16_t magicHead;
	if (!(INFILE.read( (char*)( &magicHead) , sizeof(magicHead)))) {
		cout << "Nespravny format vstupniho obrazku" << endl;
		return 1;
	}
	//fileLength
	uint32_t fileLength;
	if (!(INFILE.read((char*)(&fileLength), sizeof(fileLength)))) {
		cout << "Nespravny format vstupniho obrazku" << endl;
		return 1;
	}
	// cout << "fileLength: " << fileLength << endl;
	//partAfterLength
	uint32_t partAfterLength;
	if (!(INFILE.read((char*)(&partAfterLength), sizeof(partAfterLength)))) {
		cout << "naspravny format vstupniho obrazku" << endl;
		return 1;
	}

	//positionsOfStartOfThePic
	uint32_t positionsOfStartOfThePic;
	if (!(INFILE.read((char*)(&positionsOfStartOfThePic), sizeof(positionsOfStartOfThePic)))) {
		cout << "naspravny format vstupniho obrazku" << endl;
		return 1;
	}
	// cout << "zacatek obrazku na: " << positionsOfStartOfThePic << endl;
	unsigned int currentPositionINFILE = INFILE.tellg();
//---------
	if (positionsOfStartOfThePic >= inputFileSize || positionsOfStartOfThePic < 14 || (*(argv[1] + 1) == 'e' && inputFileSize != fileLength)) {
		cout << "Nespravna data" << endl;
		return 1;
	}
	//vytvoreni outputfile a zkopirovani dat
	ofstream OUTFILE;
	OUTFILE.open(outputFileName, ios::out | ios:: binary );
	if(!OUTFILE.is_open()) {
		cout << "Chyba - nepodarilo se vytvorit vystupni soubor!" << endl;
		INFILE.close();
		return 1;
	}
	OUTFILE << (char)magicHead << (char)(magicHead/(pow(2,8)));
	OUTFILE << (char)fileLength << (char)(fileLength/(pow(2,8))) << (char)(fileLength/(pow(2,16))) << (char)(fileLength/(pow(2,24)));
	OUTFILE << (char)partAfterLength << (char)(partAfterLength/(pow(2,8))) << (char)(partAfterLength/(pow(2,16))) << (char)(partAfterLength/(pow(2,24)));
	OUTFILE << (char)positionsOfStartOfThePic << (char)(positionsOfStartOfThePic/(pow(2,8))) << (char)(positionsOfStartOfThePic/(pow(2,16))) << (char)(positionsOfStartOfThePic/(pow(2,24)));

	while (currentPositionINFILE < positionsOfStartOfThePic) {
		uint16_t dataPackage;
		if (!INFILE.read((char*)&dataPackage, sizeof(dataPackage))) {
			cout << "error while copying dataPackage" << endl;
			return 1;
		}
		OUTFILE << (char)dataPackage << (char)(dataPackage/(pow(2,8)));
		currentPositionINFILE = INFILE.tellg();
	}
	if (currentPositionINFILE != positionsOfStartOfThePic) {
		cout << "licha delka! neosetreno!! " << endl;
		return 1;
	}
	// cout << "skoncil jsem na pozici: " << currentPositionINFILE << endl;
//---------
	//inicializace sifry
	unsigned char key[EVP_MAX_KEY_LENGTH] = "Muj klic";  // klic pro sifrovani
  	unsigned char iv[EVP_MAX_IV_LENGTH] = "Initial vector";

  	OpenSSL_add_all_ciphers();//??
	EVP_CIPHER_CTX *ctx; // context structure
  	ctx = EVP_CIPHER_CTX_new();
  	if (ctx == NULL)
  		exit(2);

  	int res;
//---------
//encrypting image
	if (*(argv[1] + 1) == 'e') {
		//inicializace
		if (mode == Mode::cbc) {
			res = EVP_EncryptInit_ex(ctx, EVP_des_cbc(), NULL, key, iv);
		} else if (mode == Mode::ecb) {
			res = EVP_EncryptInit_ex(ctx, EVP_des_ecb(), NULL, key, iv);
		} else {
			cout << "Sifra nerozpoznama" << endl;
			return 1;
		}
		if (res != 1) 
			exit(3);
		//sifrovani
		int tmpLength = 0;
		int otLength = 1; //delka ot
		int stLength = 0;

		unsigned char imageData[1];
		unsigned char st[EVP_MAX_BLOCK_LENGTH];
		// cout << "input file size: "  << inputFileSize << endl;
		while (currentPositionINFILE < inputFileSize) {
			tmpLength = 0;
			otLength = 1;
			// stLength = 0;
			if (!INFILE.read((char*)&imageData, sizeof(unsigned char))) {
				cout << "Error while encrypting imageData" << endl;
				return 1;
			}
			res = EVP_EncryptUpdate(ctx, st, &tmpLength, imageData, otLength);
			if(res != 1)
	 			exit(4);
	 		stLength += tmpLength;
			currentPositionINFILE = INFILE.tellg();

			for (int i = 0 ; i < tmpLength ; i++) {
				OUTFILE << (char)st[i];
			}
		}
	 	res = EVP_EncryptFinal_ex(ctx, st, &tmpLength);  // get the remaining ct
  		if(res != 1)
  			exit(5);
  		for (int i = 0 ; i < tmpLength ; i++) {
			OUTFILE << (char)st[i];
		}
	}
//---------
//decrypting image , desifrovani
	if (*(argv[1] + 1) == 'd') {
		//inicializace
		if (mode == Mode::cbc) {
			res = EVP_DecryptInit_ex(ctx, EVP_des_cbc(), NULL, key, iv);
		} else if (mode == Mode::ecb) {
			res = EVP_DecryptInit_ex(ctx, EVP_des_ecb(), NULL, key, iv);
		} else {
			cout << "Sifra nerozpoznama" << endl;
			return 1;
		}
		if (res != 1) 
			exit(3);
		//desifrovani
		int stLength = 1;
		int otLength = 0; //delka ot
		int tmpLength = 0;
		unsigned char cipheredImage[1];
		unsigned char decryptedImage[EVP_MAX_BLOCK_LENGTH];
		while (currentPositionINFILE < inputFileSize) {
			// otLength = 0;
			stLength = 1;
			tmpLength = 0;
			if (!INFILE.read((char*)&cipheredImage, sizeof(unsigned char))) {
				cout << "Error while encrypting cipheredImage" << endl;
				return 1;
			}
			res = EVP_DecryptUpdate(ctx, decryptedImage, &tmpLength, cipheredImage, stLength);
			if (res != 1)
				exit(4);
			otLength += tmpLength;
			currentPositionINFILE = INFILE.tellg();

			for (int i = 0 ; i < tmpLength ; i++) {
				OUTFILE << (char)decryptedImage[i];
			}
		}
		res = EVP_DecryptFinal_ex(ctx, decryptedImage, &tmpLength);  // get the remaining ct
  		if(res != 1)
  			exit(5);
		for (int i = 0 ; i < tmpLength ; i++) {
			OUTFILE << (char)decryptedImage[i];
		}
	}

	EVP_CIPHER_CTX_free(ctx);
	OUTFILE.close();
	INFILE.close();

	cout << "OK!" << endl;
	return 0;
}