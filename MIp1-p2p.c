/**************************************************************************/
/*                                                                        */
/* P1 - MI amb sockets TCP/IP - Part I                                    */
/* Fitxer p2p.c que implementa la interfície aplicació-usuari de          */
/* l'aplicació de MI, sobre la capa d'aplicació de MI (fent crides a la   */
/* interfície de la capa MI -fitxers mi.c i mi.h-).                       */
/* Autors: Marc Sánchez, Feng Lin                                                         */
/*                                                                        */
/**************************************************************************/


/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */
/* Incloem "MIp1v4-mi.h" per poder fer crides a la interfície de MI       */
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "MIp1-mi.c"

/* Definició de constants, p.e., #define MAX_LINIA 150                      */
#define TECLAT 				0

/* Definició de màxims de longitud dels diferents tipus d'string            */
#define MAX_BUFFER 			300
#define MAX_TYPE 			2
#define MAX_IP 				16
#define MAX_LINE			304
#define MAX_PORT			5

/* Definició de funcions per el programa principal                          */
void EvalResult(int res, const int *sockets, int nSockets);
int getPort();


int main(int argc,char *argv[])
{
    // CONSTANTS que es poden cambiar.
   char ipLocal[MAX_IP] = "0.0.0.0";

   // Creem les linies de dades del protocol.
   char ipRemota[MAX_IP];      // Ip on es connectarà tant per la connexió de servidor com la de client.
   char nickname[MAX_BUFFER];
   char nicknameRemot[MAX_BUFFER];
   char missatge[MAX_LINE];

   int port = 3000; // Valor per defecte.
   int portLocal;

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

   //***
   socketsEscoltant[1] = MI_IniciaEscPetiRemConv(port);
   //***

   printf("Benvingut %s , has configurat el port %i\n", nickname, port);
   EvalResult(socketsEscoltant[1], socketsEscoltant, nSockets);
   nSockets = 2;

   int socketActiu;
   //printf("Quedem escoltant que arrivi alguna cosa.\n");
   printf("Escriu la Ip de on et vols connectar o espera't que es connectin amb tu.\n");

   //***
   socketActiu=MI_HaArribatPetiConv(socketsEscoltant[1]);
   //***

   EvalResult(socketActiu, socketsEscoltant, nSockets);
   //printf("HA arrivat per el socket : %i\n", socketActiu);
   // Si el socket actiu és el teclat fem un socket i un connect.
   if(socketActiu == TECLAT){

       //readFromKeyboard(ipRemota);
       int ipLong = getIPAddress(ipRemota);
       //printf("%s\n", ipRemota);
       EvalResult(ipLong, socketsEscoltant, nSockets);
       //printf("%s\n", ipRemota.buffer);
       //printf("%s\n", ipRemota);

       //*** linia 107 esquelet.c

       socketActiu = MI_DemanaConv(ipRemota, port, ipLocal, &portLocal, nickname, nicknameRemot);
       socketsEscoltant[1] = (int)socketActiu;
       //printf("Ha creat un socket client i està demanant connexió : \n");
       //EvalResult(TCP_DemanaConnexio(socketActiu, ipRemota.buffer, port), socketsEscoltant, nSockets);
       //EvalResult(TCP_DemanaConnexio(socketActiu, ipRemota, port), socketsEscoltant, nSockets);
       //printf("Ha demanat la connexió \n");

   }
   // Si el socket actiu no és un teclat fem un accept.
   else {
       // ***
       MI_AcceptaConv(socketActiu, ipRemota, &port, ipLocal, &portLocal, nickname, nicknameRemot);
       printf("Ha acceptat connexió \n");
       socketsEscoltant[1] = socketActiu;
   }

 }

/***************** FUNCIONS D'EVALUACIÓ ************************************/

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

/***************** FUNCIONS DE LECTURA ************************************/

/**
 * Llegeix de teclat numberBytesToRead caràcters i els posa a inData
 * Retorna un -1 si quelcom ha anat malament.
 * Retorna el nombre de bytes llegits si tot ha anat bé, inData conté la cadena de caràcters llegida.
 */
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

/**
 * Llegeix un número de teclat que fa referència al port.
 * Retorna aquest número com un enter positiu > 0.
 * Si hi ha error retorna -1.
 */
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


int getNickname(char * nickname){
	printf("%s\n", "Entra el teu nickname : ");
	return readFromKeyboard(nickname, MAX_BUFFER);
}

int getIPAddress(char * ip){
	return readFromKeyboard(ip, MAX_IP);
}
