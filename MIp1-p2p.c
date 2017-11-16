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
#define PORT_DEFECTE        0

/* Definició de màxims de longitud dels diferents tipus d'string            */
#define MAX_BUFFER 			300
#define MAX_TYPE 			2
#define MAX_IP 				16
#define MAX_LINE			304
#define MAX_PORT			6

/* Definició de funcions per el programa principal                          */
void EvalResult(int res, const int *sockets, int nSockets);
int getPort();


int main(int argc,char *argv[])
{
    // CONSTANTS que es poden cambiar.
   char ipLocal[MAX_IP] = "0.0.0.0";

   // Creem les linies de dades del protocol.
   char ipRemota        [MAX_IP];      // Ip on es connectarà tant per la connexió de servidor com la de client.
   char nickname        [MAX_BUFFER];
   char nicknameRemot   [MAX_BUFFER];
   char missatge        [MAX_BUFFER];

   bzero(ipRemota,      MAX_IP);
   bzero(nickname,      MAX_BUFFER);
   bzero(nicknameRemot, MAX_BUFFER);
   bzero(missatge,      MAX_BUFFER);

   int port; // Valor per defecte.
   int portLocal=0;
   int fi = 1;

   int socketsEscoltant[2];
   int socketEscoltador;
   int nSockets = 2;
   int nBytes;
   int res;
   int socketActiu;

   printf("/*-------------------------------------------------------------------*/\n");
   printf("/* XAT DE XARXES -> Autors : Fent Lin i Marc Sánchez\n");
   printf("/*---------------------------------------------------\n");
   printf("/* Entra un nick per comunicar-te\n");
   EvalResult(getNickname(nickname), NULL, 0);
   socketsEscoltant[0] = TECLAT;
   while(fi != 0){
       socketEscoltador = MI_IniciaEscPetiRemConv(PORT_DEFECTE);
       socketsEscoltant[1] = socketEscoltador;
       EvalResult(socketEscoltador, socketsEscoltant, nSockets); // Evaluem el resultat de l'anterior instrucció
       // Obtenim el port i la ip locals assignades dinàmicament.
       portLocal = MI_DescobreixIpIPortDinamic(socketsEscoltant[1], ipLocal);
       EvalResult(portLocal, socketsEscoltant, 2);
       printf("/*-------------------------------------------------------------------*/\n");
       printf("/* Informació del programa : */\n");
       printf("/*---------------------------------------------------\n");
       printf("/* Ip configurada de manera local : %s\n",ipLocal);
       printf("/* Port configurat de manera local : %i\n", portLocal);

       // printf("Espera que es connectin o configura un port i una ip per aquest ordre per realitzar la connexió.\n");

       printf("/*-------------------------------------------------------------------*/\n");
       printf("/* Funcionament del programa : \n");
       printf("/* Opció 1 -> Espera que es connectin a la ip i port anteriors\n");
       printf("/* Opció 2 -> Entra port i ip per aquest ordre\n");
       printf("/*-------------------------------------------------------------------*/\n");

       socketActiu=MI_HaArribatPetiConv(socketsEscoltant[1]);
       EvalResult(socketActiu, socketsEscoltant, nSockets);
       if(socketActiu == TECLAT){
           port = getNumber();
           int ipLong = getIPAddress(ipRemota);
           EvalResult(ipLong, socketsEscoltant, nSockets);
           socketActiu = MI_DemanaConv(ipRemota, port, ipLocal, &portLocal, nickname, nicknameRemot);
           EvalResult(socketActiu, socketsEscoltant, nSockets);
           socketsEscoltant[1] = (int)socketActiu;
       }
       // Si el socket actiu no és un teclat fem un accept.
       else {
           socketActiu = MI_AcceptaConv(socketActiu, ipRemota, &port, ipLocal, &portLocal, nickname, nicknameRemot);
           EvalResult(socketActiu, socketsEscoltant, nSockets);
           socketsEscoltant[1] = socketActiu;
       }
       // -------------------------

       mostraDadesRemotes(nicknameRemot, port, ipRemota);

       // Intent de treure els prompts local i remot
       // S'ha intentat accedir al buffer de la pantalla per extreure el prompt si arriva algu i sinó es pinta l'altre.
       //---------------------------------------------------------------------------------------------------------------
       // char promptLocal[strlen(nickname)+1];
       // creaPrompt(promptLocal, nickname);
       //
       // char promptRemot[strlen(nicknameRemot)+1];
       // creaPrompt(promptRemot, nicknameRemot);

       int resultatAccio = 1;
       while(resultatAccio > 0){
           bzero(missatge, MAX_BUFFER);
           socketActiu = MI_HaArribatLinia(socketsEscoltant[1]);
           if(socketActiu == TECLAT){
               EvalResult(readFromKeyboard(missatge, MAX_BUFFER), socketsEscoltant, nSockets);
               if(strcmp(missatge,"$")!=1) break;
               resultatAccio = MI_EnviaLinia(socketsEscoltant[1], missatge);
               EvalResult(resultatAccio, socketsEscoltant, nSockets);
           }
           else{
               resultatAccio = MI_RepLinia(socketActiu, missatge);
               if(resultatAccio != 0){
                   printf("%s\n", missatge);
               }
           }
       }

       // En cas que el resultat sigui -2 o -1 es tenquen tots els sockets.
       EvalResult(resultatAccio, socketsEscoltant, nSockets);

       int i;
       close(socketsEscoltant[1]);
       socketsEscoltant[1] = socketEscoltador;
       printf("/*-------------------------------------------------------------------*/\n");
       printf("/* CONVERSA ACABADA, vols seguir amb el programa?? ");
       printf("/* Entre '1' per continuar '0' per acabar ");
       printf("/*-------------------------------------------------------------------*/\n");
       fi = getNumber();
   }


   return (0);

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
	if(res < 0 && res != -2){
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
	//printf("%i\n", bytes_llegits);
	inData[bytes_llegits-1] = '\0';
	//printf("%s\n",inData);

    return bytes_llegits;
}

/**
 * Crea el prompt donat un nickname i el desa al paràmetre prompt passat per referència.
 * prompt conté el nick + el caràcter '>'
 */
int creaPrompt(char * prompt, char * nick){
    bzero(prompt, strlen(nick)+1);
    strcpy(prompt, nick);
    prompt[strlen(nick)] = '>';
}

/**
 * Llegeix un número de teclat que fa referència al port.
 * Retorna aquest número com un enter positiu > 0.
 * Si hi ha error retorna -1.
 */
int getNumber(){
	char port[MAX_BUFFER];
	bzero(port, '\0');
	int res = readFromKeyboard(port, MAX_PORT);
	printf("Has entrat el port %s\n",port);
	EvalResult(res, NULL, 0); // Indiquem que no hi ha sockets amb un 0 i així no petarà.
	int portNumber;
	EvalResult(portNumber = atoi(port), NULL, 0); // indiquem que no hi ha sockets amb un 0 i així no petarà.
	return portNumber;
}

int mostraDadesRemotes(char * nicknameR, int portR, char * ipR){
    printf("/*-------------------------------------------------------------------*/\n");
    printf("/* Establerta connexió amb %s a la ip %s i el port %i\n", nicknameR, ipR, portR);
    printf("/*-------------------------------------------------------------------*/\n");
}

int getNickname(char * nickname){
	return readFromKeyboard(nickname, MAX_BUFFER);
}

int getIPAddress(char * ip){
	return readFromKeyboard(ip, MAX_IP);
}
