/**************************************************************************/
/*                                                                        */
/* P1 - MI amb sockets TCP/IP - Part I                                    */
/* Versio numero N                                                        */
/*                                                                        */
/* Autors: X, Y                                                           */
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

#define TECLAT 0

/* Definició de constants, p.e., #define MAX_LINIA 150                    */

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




int main(int argc,char *argv[])
{

	/* Declaració de variables, p.e., int n;                                 */
	// CONSTANTS que es poden cambiar.
	char ipLocal[16] = "0.0.0.0";
	char ipRemota[16] = "192.168.1.115";
	int port = 3000;
	int portRemot;
	int socketsEscoltant[2];
	int nSockets = 1;

	char missatge[200];

	/* Expressions, estructures de control, crides a funcions, etc.          */
	int res = TCP_CreaSockServidor(ipLocal, port);
	EvalResult(res, socketsEscoltant, nSockets);

	socketsEscoltant[0] = TECLAT;
	socketsEscoltant[1] = res;
	nSockets = 2;

	int socketActiu;
	socketActiu=HaArribatAlgunaCosa(socketsEscoltant, nSockets);
	EvalResult(socketActiu, socketsEscoltant, nSockets);

	// Si el socket actiu és el teclat fem un socket i un connect.
	if(socketActiu == TECLAT){
		socketActiu = TCP_CreaSockClient(ipLocal, port);

		socketsEscoltant[1] = (int)socket;

		EvalResult(socketActiu, socketsEscoltant, nSockets);
		EvalResult(TCP_DemanaConnexio(socketActiu, ipRemota, port), socketsEscoltant, nSockets);

	}
	// Si el socket actiu no és un teclat fem un accept.
	else {
		// Ja tinc les dades del que està en remot i el port per el que em puc comunicar amb ell.
		socketActiu = TCP_AcceptaConnexio(socketActiu, ipRemota, & portRemot);
		EvalResult(socketActiu, socketsEscoltant, nSockets);
		socketsEscoltant[1] = socketActiu;
	}

	int resultatAccio = 1;
	while(resultatAccio != 0 && !strcmp(missatge, "$")){
		socketActiu = HaArribatAlgunaCosa(socketsEscoltant, nSockets);
		if(socketActiu == TECLAT){
			resultatAccio = TCP_Envia(socketActiu, missatge, strlen(missatge));
		}
		else{
			resultatAccio = TCP_Rep(socketActiu, missatge, strlen(missatge));
		}
		// Si hi ha qualsevol error es tencaran els sockets.
		EvalResult(resultatAccio, socketsEscoltant, nSockets);
	}

	return (0);
}


void EvalResult(int res, const int *sockets, int nSockets){
	if(res < 0){
		int i = 0;
		while(i<nSockets){
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
	return read(Sck, SeqBytes,sizeof(SeqBytes));
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





/* Si ho creieu convenient, feu altres funcions...                        */
