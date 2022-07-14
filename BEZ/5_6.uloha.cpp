//schejond
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/pem.h>

#define BUFFER_SIZE_1024 1024

using namespace std;

int main(int argc, char * argv[]) {	
	char buff[BUFFER_SIZE_1024];

	int sockfd;
	struct sockaddr_in servaddr;

	//TCP spojeni
	sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//or bzero (bez 0 jako argumentu)
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("147.32.232.248"); //ip adresa fit.cvut.cz
	servaddr.sin_port=htons(443); // port

	if (0 != connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
		cout << "Chyba pri pripojovani na stranku" << endl;
		exit(1);
	}

	//inicializace knihovny openssl
	SSL_library_init();
	//vytvoreni noveho SSL kontextu
	SSL_CTX * ssl_ctx = SSL_CTX_new(SSLv23_client_method());
	if(!ssl_ctx) {
		cout << "Chyba pri vytvareni noveho SSL contextu" << endl;
		exit(2);
	}
	//zakazani starych protokolu
	SSL_CTX_set_options(ssl_ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1);

	//vytvoreni SSL struktury
	SSL * ssl = SSL_new(ssl_ctx);
	if(!ssl) {
		cout << "Chyba pri vytvareni SSL new" << endl;
		exit(3);
	}
	
	//prirazeni otevreneho spojeni
	if(!SSL_set_fd(ssl,sockfd)) {
		cout << "Chyba pri vytvareni SSL_Set_fd" << endl;
		exit(4);
	}
	
//-------------------cast ulohy 6
	//tady zakazu ty sifry co mi vysly
	// SSL_set_cipher_list(ssl,"ALL:!ECDHE-RSA-AES128-GCM-SHA256");
	SSL_set_cipher_list(ssl,"ALL:!ECDHE-RSA-AES256-GCM-SHA384");
	// int loaded = SSL_CTX_load_verify_locations(ssl_ctx, NULL, NULL);
	// if(!loaded) {
	// 	cout << "Chyba pri SSL_CTX_load_verify_locations" << endl;
	// 	exit(6);
	// }
//-------------------
	//zahajeni komunikace
	if(SSL_connect(ssl) <= 0) {
		cout << "Chyba pri SSL_connect" << endl;
		exit(5);
	}

//6ta uloha
	// SSL_get_verify_result(ssl_ctx);
	// if (SSL_get_verify_result(ssl_ctx)) {
	// 	cout << "tady!" << endl;
	// 	exit(66);
	// }

	//zapsani HTTP pozadavku do bufferu
	snprintf(buff,BUFFER_SIZE_1024,"GET /student/odkazy HTTP/1.1\r\nConnection: close\r\nHost: fit.cvut.cz\r\n\r\n");
	//poslani HTTP pozadavku
	if(SSL_write(ssl, buff, strlen(buff)+1) <= 0) {
		cout << "Chyba pri SSL_write" << endl;
		exit(7);
	}
	
	int res;
	int datalen = 0;
	FILE * fOutput = fopen("SERVER_INFO.PEM","w");
	//cteni odpovedi
	while((res = SSL_read(ssl, buff, BUFFER_SIZE_1024)) > 0) {
		fwrite(buff, sizeof(char), res, fOutput);
		datalen += res;
	}
	fprintf(fOutput,"\n");
	fclose(fOutput);
	
	// cout << "Zapsano: " << datalen << " bytu do SERVER_INFO.PEM" << endl;
//---uloha 6 vypsani pouzit sifry
	const SSL_CIPHER * sc = SSL_get_current_cipher(ssl);
	cout << "Pouzita sifra: " << SSL_CIPHER_get_name(sc) << endl;
	
	cout << "Sifry dostupne na klientovi:" << endl;
	int prior = 0;
	const char * list;
	while((list = SSL_get_cipher_list(ssl,prior))) {
		cout << "-> " << list << endl; 
		prior++;
	}
//----
	//ukonceni a uklizeni po sobe
	SSL_shutdown(ssl);
	close(sockfd);
	SSL_free(ssl);
	SSL_CTX_free(ssl_ctx);

	cout << "OK" << endl;
	return 0;
}