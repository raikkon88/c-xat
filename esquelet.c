/**************************************************************************/
/*                                                                        */
/* P1 - MI amb sockets TCP/IP - Part I                                    */
/* Versio numero 1                                                        */
/*                                                                        */
/* Autors: Feng Lin i Marc Sánchez                                        */
/*                                                                        */
/**************************************************************************/



/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */
#include <string.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Definició de constants, p.e., #define MAX_LINIA 150                    */
#define TECLAT 				0

/* Definició de màxims de longitud dels diferents tipus d'string */
#define MAX_BUFFER 		200
#define MAX_TYPE 			2
#define MAX_IP 				16
#define MAX_LINE			204
#define MAX_PORT			5

/* Declaració de funcions (les seves definicions es troben més avall) per */
/* així fer-les conegudes des d'aqui fins al final de fitxer.             */

int TCP_CreaSockClient(const char *IPloc, int portTCPloc);
int TCP_CreaSockServidor(const char *IPloc, int portTCPloc);
int TCP_DemanaConnexio(int Sck, const char *IPrem, int portTCPrem);
int TCP_AcceptaConnexio(int Sck, char *IPrem, int *portTCPrem);
int TCP_Envia(int Sck, const char *SeqBytes, int LongSeqBytes);
int TCP_Rep(int Sck, char *SeqBytes, int LongSeqBytes);
int TCP_TancaSock(int Sck);
int TCP_TrobaAdrSockLoc(int Sck, char *IPloc, int *portTCPloc);
int TCP_TrobaAdrSockRem(int Sck, char *IPrem, int *portTCPrem);
int HaArribatAlgunaCosa(const int *LlistaSck, int LongLlistaSck);
void MostraError(const char *text);
void EvalResult(int res, const int *sockets, int nSockets);

/**************************************************************************/
/*                                                                        */
/* Struct String
/* Versio numero 1                                                        */
/*                                                                        */
/* Autors: Marc Sánchez i Feng Lin                                        */
/*                                                                        */
/**************************************************************************/

int main(int argc,char *argv[])
{
	// CONSTANTS que es poden cambiar.
	char ipLocal[MAX_IP] = "0.0.0.0";

	// Creem les linies de dades del protocol.
	char ipRemota[MAX_IP];      // Ip on es connectarà tant per la connexió de servidor com la de client.
	char nickname[MAX_BUFFER];  // Nickname de l'usuari.
	char missatge[MAX_LINE];	  // Nickname de l'usuari.

	int port = 3000; // Valor per defecte.
	int portRemot;
	int socketsEscoltant[2];
	int nSockets = 1;
	int nBytes;
	int res;

	// LLegim el port
	port = getPort();
	// LLegim el nickname
	EvalResult(getNickname(nickname), NULL, 0);
	// Generem una escolta com a servidor
	//EvalResult(res, socketsEscoltant, nSockets);


	// Establim els sockets que escoltarem de moment.
	socketsEscoltant[0] = TECLAT;
	socketsEscoltant[1] = TCP_CreaSockServidor(ipLocal, port);
	printf("Benvingut %s , has configurat el port %i\n", nickname, port);
	EvalResult(socketsEscoltant[1], socketsEscoltant, nSockets);
	nSockets = 2;

	int socketActiu;
	//printf("Quedem escoltant que arrivi alguna cosa.\n");
	printf("Escriu la Ip de on et vols connectar o espera't que es connectin amb tu.\n");
	socketActiu=HaArribatAlgunaCosa(socketsEscoltant, nSockets);
	EvalResult(socketActiu, socketsEscoltant, nSockets);
	//printf("HA arrivat per el socket : %i\n", socketActiu);
	// Si el socket actiu és el teclat fem un socket i un connect.
	if(socketActiu == TECLAT){

		//readFromKeyboard(ipRemota);
		int ipLong = getIPAddress(ipRemota);
		printf("%s\n", ipRemota);
		EvalResult(ipLong, socketsEscoltant, nSockets);
		//printf("%s\n", ipRemota.buffer);
		printf("%s\n", ipRemota);
		socketActiu = TCP_CreaSockClient(ipLocal, port);
		socketsEscoltant[1] = (int)socketActiu;
		EvalResult(socketActiu, socketsEscoltant, nSockets);
		//printf("Ha creat un socket client i està demanant connexió : \n");
		//EvalResult(TCP_DemanaConnexio(socketActiu, ipRemota.buffer, port), socketsEscoltant, nSockets);
		EvalResult(TCP_DemanaConnexio(socketActiu, ipRemota, port), socketsEscoltant, nSockets);
		printf("Ha demanat la connexió \n");

	}
	// Si el socket actiu no és un teclat fem un accept.
	else {
		// Ja tinc les dades del que està en remot i el port per el que em puc comunicar amb ell.
		//socketActiu = TCP_AcceptaConnexio(socketActiu, ipRemota.buffer, & portRemot);
		socketActiu = TCP_AcceptaConnexio(socketActiu, ipRemota, & portRemot);
		printf("Ha acceptat connexió \n");
		EvalResult(socketActiu, socketsEscoltant, nSockets);
		socketsEscoltant[1] = socketActiu;
	}
	int resultatAccio = TCP_Envia(socketsEscoltant[1], nickname, strlen(nickname));

	//printf("Missatge abans del bucle : %s\n", missatge);
	while(resultatAccio != 0){
		bzero(missatge, 200);
		socketActiu = HaArribatAlgunaCosa(socketsEscoltant, nSockets);
		if(socketActiu == TECLAT){
			EvalResult(readFromKeyboard(missatge, MAX_BUFFER), socketsEscoltant, nSockets);
			if(strcmp(missatge,"$")!=1) break;int
			resultatAccio = envia_linia(socketsEscoltant[1], missatge);
			//resultatAccio = TCP_Envia(socketsEscoltant[1], missatge, strlen(missatge));
		}
		else{
			//resultatAccio = TCP_Rep(socketActiu, missatge.buffer, strlen(missatge.buffer));
			resultatAccio = TCP_Rep(socketActiu, missatge, MAX_LINE);
			if(resultatAccio != 0)
				//printf("%s\n", missatge.buffer);
				printf("%s\n", missatge);
		}
		// Si hi ha qualsevol error es tencaran els sockets.
		EvalResult(resultatAccio, socketsEscoltant, nSockets);
	}

	int i;
	// Tenca tots els sockets
	for(i = 0; i < nSockets; i++){
		close(socketsEscoltant[i]);
	}

	return (0);
}

/**
 * Evalua res, si és negatiu tenca els sockets i acaba l'apliacació, altrament no fa res.
 * - res : és el resultat que s'ha d'evaluar
 * - sockets : son tots els sockets que l'aplicació té oberts
 * - nSockets : és el nombre de sockets que l'aplicació té oberts
 * En cas que hagi tencat l'aplicació haurà mostrat per pantalla l'error que s'ha produït.
 */

void EvalResult(int res, const int *sockets, int nSockets){
	if(res < 0){
		int i = 0;
		// Faltaria buidar el buffer del TECLAT que sempre el poso a la posició 0 de sockets.
		while(sockets != NULL && i<nSockets){
				close(sockets[i]);
				i++;
		}
		MostraError("Error ");
		exit (-1);
	}
}


/* Definicio de funcions                                                  */



/* Crea un socket TCP “client” a l’@IP “IPloc” i #port TCP “portTCPloc”   */
/* (si “IPloc” és “0.0.0.0” i/o “portTCPloc” és 0 es fa/farà una          */
/* assignació implícita de l’@IP i/o del #port TCP, respectivament).      */
/* "IPloc" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud màxima de 16 chars (incloent '\0')                */
/* Retorna -1 si hi ha error; l’identificador del socket creat si tot     */
/* va bé.                                                                 */
int TCP_CreaSockClient(const char *IPloc, int portTCPloc)
{
	int fd;
	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		close(fd);

		return (-1);
	}

	return fd;

}



/* Crea un socket TCP “servidor” (o en estat d’escolta – listen –) a      */
/* l’@IP “IPloc” i #port TCP “portTCPloc” (si “IPloc” és “0.0.0.0” i/o    */
/* “portTCPloc” és 0 es fa una assignació implícita de l’@IP i/o del      */
/* #port TCP, respectivament).                                            */
/* "IPloc" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud màxima de 16 chars (incloent '\0').               */
/* Retorna -1 si hi ha error; l’identificador del socket creat si tot     */
/* va bé.                                                                 */

int TCP_CreaSockServidor(const char *IPloc, int portTCPloc)
{
	printf("Anem a configurar %s al port %i\n", IPloc, portTCPloc);

	int fd;
	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		close(fd);
		return -1;
	}

	int i;
	struct sockaddr_in adrloc;
	adrloc.sin_family=AF_INET;
	adrloc.sin_port=htons(portTCPloc);
	adrloc.sin_addr.s_addr=inet_addr(IPloc); /* o bé: ...s_addr = INADDR_ANY */
	for(i=0;i<8;i++){adrloc.sin_zero[i]='0';}

	if((bind(fd,(struct sockaddr*)&adrloc,sizeof(adrloc)))==-1)
	{
		close(fd);
		return (-1);
	}

	if((listen(fd,3))==-1)
	{
		close(fd);
		return (-1);
	}

	return fd;
}



/* El socket TCP “client” d’identificador “Sck” demana una connexió al    */
/* socket TCP “servidor” d’@IP “IPrem” i #port TCP “portTCPrem” (si tot   */
/* va bé es diu que el socket “Sck” passa a l’estat “connectat” o         */
/* establert – established –).                                            */
/* "IPrem" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud màxima de 16 chars (incloent '\0').               */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int TCP_DemanaConnexio(int Sck, const char *IPrem, int portTCPrem)
{
	struct sockaddr_in adrConnexio;
	adrConnexio.sin_family=AF_INET;
	adrConnexio.sin_port=htons(portTCPrem);
	adrConnexio.sin_addr.s_addr=inet_addr(IPrem); /* o bé: ...s_addr = INADDR_ANY */
	int i;
	for(i=0;i<8;i++){adrConnexio.sin_zero[i]='0';}

	if((connect(Sck,(struct sockaddr*)&adrConnexio,sizeof(adrConnexio)))==-1)
	{
		close(Sck);
		return (-1);
	}
	return Sck; // El socket serà un valor positiu.
}



/* El socket TCP “servidor” d’identificador “Sck” accepta fer una         */
/* connexió amb un socket TCP “client” remot, i crea un “nou” socket,     */
/* que és el que es farà servir per enviar i rebre dades a través de la   */
/* connexió (es diu que aquest nou socket es troba en l’estat “connectat” */
/* o establert – established –; el nou socket té la mateixa adreça que    */
/* “Sck”).                                                                */
/* Omple “IPrem*” i “portTCPrem*” amb respectivament, l’@IP i el #port    */
/* TCP del socket remot amb qui s’ha establert la connexió.               */
/* "IPrem*" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud màxima de 16 chars (incloent '\0').               */
/* Retorna -1 si hi ha error; l’identificador del socket connectat creat  */
/* si tot va bé.                                                          */

int TCP_AcceptaConnexio(int Sck, char *IPrem, int *portTCPrem)
{
	int socket;

	struct sockaddr_in adrConnexio;
	int long_adrrem=sizeof(adrConnexio);

	if((socket=accept(Sck,(struct sockaddr*)&adrConnexio, &long_adrrem))==-1)
	{
		close(Sck);
		return (-1);
	}

	// S'ha establert la connexió i s'emplenen les dades de qui s'ha connectat a les variables d'entrada.
	IPrem = inet_ntoa(adrConnexio.sin_addr);
	portTCPrem = (int *) (int) ntohs(adrConnexio.sin_port);
	return socket;
}



/* Envia a través del socket TCP “connectat” d’identificador “Sck” la     */
/* seqüència de bytes escrita a “SeqBytes” (de longitud “LongSeqBytes”    */
/* bytes) cap al socket TCP remot amb qui està connectat.                 */
/* "SeqBytes" és un vector de chars qualsevol (recordeu que en C, un      */
/* char és un enter de 8 bits) d'una longitud >= LongSeqBytes bytes.      */
/* Retorna -1 si hi ha error; el nombre de bytes enviats si tot va bé.    */
int TCP_Envia(int Sck, const char *SeqBytes, int LongSeqBytes)
{
	//printf("%s -> dins de tcp_envia per el socket %i el nombre de bytes %i\n", SeqBytes, Sck, LongSeqBytes);

	return write(Sck,SeqBytes,strlen(SeqBytes));
}



/* Rep a través del socket TCP “connectat” d’identificador “Sck” una      */
/* seqüència de bytes que prové del socket remot amb qui està connectat,  */
/* i l’escriu a “SeqBytes*” (que té una longitud de “LongSeqBytes” bytes),*/
/* o bé detecta que la connexió amb el socket remot ha estat tancada.     */
/* "SeqBytes*" és un vector de chars qualsevol (recordeu que en C, un     */
/* char és un enter de 8 bits) d'una longitud <= LongSeqBytes bytes.      */
/* Retorna -1 si hi ha error; 0 si la connexió està tancada; el nombre de */
/* bytes rebuts si tot va bé.                                             */
int TCP_Rep(int Sck, char *SeqBytes, int LongSeqBytes)
{
	return read(Sck, SeqBytes, LongSeqBytes);
}



/* S’allibera (s’esborra) el socket TCP d’identificador “Sck”; si “Sck”   */
/* està connectat es tanca la connexió TCP que té establerta.             */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int TCP_TancaSock(int Sck)
{
	return close(Sck);
}



/* Donat el socket TCP d’identificador “Sck”, troba l’adreça d’aquest     */
/* socket, omplint “IPloc*” i “portTCPloc*” amb respectivament, la seva   */
/* @IP i #port TCP.                                                       */
/* "IPloc*" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud màxima de 16 chars (incloent '\0').               */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int TCP_TrobaAdrSockLoc(int Sck, char *IPloc, int *portTCPloc)
{
	struct sockaddr_in adrrem;
	int long_adrrem=sizeof(adrrem);
	if (getsockname(Sck, (struct sockaddr *)&adrrem, &long_adrrem) == -1)
	{
		close(Sck);
		return -1;
	}
	strcpy(IPloc, inet_ntoa(adrrem.sin_addr));
	//portTCPloc = (int*)ntohs(adrrem.sin_port);
	int res[1];
	res[0] = ntohs(adrrem.sin_port);
	portTCPloc = res;
}



/* Donat el socket TCP “connectat” d’identificador “Sck”, troba l’adreça  */
/* del socket remot amb qui està connectat, omplint “IPrem*” i            */
/* “portTCPrem*” amb respectivament, la seva @IP i #port TCP.             */
/* "IPrem*" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud màxima de 16 chars (incloent '\0').               */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int TCP_TrobaAdrSockRem(int Sck, char *IPrem, int *portTCPrem)
{
	struct sockaddr_in adrrem;
	int long_adrrem=sizeof(adrrem);
	if (getpeername(Sck, (struct sockaddr *)&adrrem, &long_adrrem) == -1)
	{
		close(Sck);
		return -1;
	}
	strcpy(IPrem, inet_ntoa(adrrem.sin_addr));
	int res[1];
	res[0] = ntohs(adrrem.sin_port);
	portTCPrem = res;
}



/* Examina simultàniament i sense límit de temps (una espera indefinida)  */
/* els sockets (poden ser TCP, UDP i stdin) amb identificadors en la      */
/* llista “LlistaSck” (de longitud “LongLlistaSck” sockets) per saber si  */
/* hi ha arribat alguna cosa per ser llegida.                             */
/* "LlistaSck" és un vector d'enters d'una longitud >= LongLlistaSck      */
/* Retorna -1 si hi ha error; si arriba alguna cosa per algun dels        */
/* sockets, retorna l’identificador d’aquest socket.                      */
int HaArribatAlgunaCosa(const int *LlistaSck, int LongLlistaSck)
{
	fd_set conjunt;
	FD_ZERO(&conjunt);
	int i;
	int descmax = 0;
	for(i = 0; i < LongLlistaSck; i++){
		int fd = LlistaSck[i];
		FD_SET(fd,&conjunt);
		if(fd > descmax){
			descmax = fd;
		}
	}

	/* examinem lectura del teclat i del socket scon amb la llista conjunt */
	if(select(descmax+1, &conjunt, NULL, NULL, NULL) == -1)
	{
		perror("Error en select");
		return (-1);
	}
	// Ha d'haver arrivat algu per algun dels sockets
	int j;
	for(j = 0; j < LongLlistaSck; j++){
		if(FD_ISSET(LlistaSck[j], &conjunt)){
			printf("FD seleccionat : %i \n", LlistaSck[j]);
			return LlistaSck[j];
		}
	}
	// Si surt del bucle vol dir que hi ha hagut un error ja que cap dels sockets ha saltat i no pot ser.
	return -1;
}



/* Escriu un missatge de text al flux d’error estàndard stderr, format    */
/* pel text “Text”, un “:”, un espai en blanc, un text que descriu el     */
/* darrer error produït en una crida de sockets, i un canvi de línia.     */
void MostraError(const char *text)
{
 	fprintf(stderr, "%s: %s\n", text, strerror(errno));
}

int readFromKeyboard(char * inData, int numberBytesToRead){
	int bytes_llegits;
	if((bytes_llegits=read(0, inData, numberBytesToRead))==-1)
	{
    return (-1);
	}
	printf("%i\n", bytes_llegits);
	inData[bytes_llegits-1] = '\0';
	printf("%s\n",inData);

  return bytes_llegits;
}

int getNickname(char * nickname){
	printf("%s\n", "Entra el teu nickname : ");
	return readFromKeyboard(nickname, MAX_BUFFER);
}

int envia_linia(int sck, const char * missatge){
	char linia[MAX_LINE];
	sprintf(linia, "%s%.3d%s", "L", strlen(missatge), missatge);
	return TCP_Envia(sck, linia, strlen(linia));
}

int envia_nickname(int sck, const char * nickname){
	char linia[MAX_LINE];
	sprintf(linia, "%s%.3d%s", "N", strlen(nickname), nickname);
	return TCP_Envia(sck, linia, strlen(linia));
}

int getIPAddress(char * ip){
	return readFromKeyboard(ip, MAX_IP);
}

int getPort(){
	char port[MAX_BUFFER];
	bzero(port, '\0');
	printf("%s\n", "Entra el número de port que ha de fer servir el programa \nHa de ser un enter positiu entre 0 i 65000 : ");
	int res = readFromKeyboard(port, MAX_PORT);
	printf("Has entrat el port %s\n",port);
	EvalResult(res, NULL, 0); // Indiquem que no hi ha sockets amb un 0 i així no petarà.
	int portNumber;
	EvalResult(portNumber = atoi(port), NULL, 0); // indiquem que no hi ha sockets amb un 0 i així no petarà.
	return portNumber;
}

/* Si ho creieu convenient, feu altres funcions...                        */
