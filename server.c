#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "user_interface.c"
#include "interface_sockets.c"

#define TECLAT 0

int main(int argc,char *argv[])
{

	int sesc, scon, i;
	int bytes_llegits, bytes_escrits;
	char buffer[200];
	int long_adrrem;
	struct sockaddr_in adrloc, adrrem;
	char iploc[16];
	int portloc;
	struct string data;

	fd_set conjunt;               /* conjunt de descriptors de fitxer de lectura */
	int descmax;

	sesc = create_socket(sesc);
	printFunctionResult(sesc);

	/* L'adreca del socket del servidor (socket local) */
	strcpy(iploc,"0.0.0.0"); /* 0.0.0.0 correspon a INADDR_ANY */
	portloc = 3001;

	adrloc = create_socket_struct(portloc, iploc);
	/* Enllaça ip i port al socket */
	make_bind(sesc, adrloc);
	/* Es crea una cua per emmagatzemar peticions de connexió pendents */
	make_connection_queue(sesc);
	/* S'accepta la connexió */
	scon = accept_connection(sesc, adrrem);
	/* Mostrem paràmetres del client */
	show_connection_params(adrrem, scon);


	int volta = 0;
	descmax = scon; /* el número de descriptor màxim */

	printf("Entra el nick : \n");
	readFromKeyboard(&data);
	char nick_local[200];
	strncpy(nick_local,data.buffer,strlen(data.buffer));
	data.number_bytes=write(scon,nick_local,strlen(nick_local));

	while(strcmp(data.buffer, "$")){

		 /* Poden arribar dades via teclat o via scon: fem una llista de lectura amb els dos */
		FD_ZERO(&conjunt);             /* esborrem el contingut de la llista */
		FD_SET(TECLAT,&conjunt);       /* afegim (“marquem”) el teclat a la llista */
		FD_SET(scon,&conjunt);         /* afegim (“marquem”) el socket connectat a la llista */
		/* Llegeix de socket */
		//data = read_from_socket(scon);
		/* Escriu per pantalla */
		//bytes_escrits = write_screen(data, scon);
	//}


	 /* examinem lectura del teclat i del socket scon amb la llista conjunt */
		printf("select \n");
		 if(select(descmax+1, &conjunt, NULL, NULL, NULL) == -1)
		 {
		  perror("Error en select");
		  exit(-1);
		 }
		 /* Ara a conjunt no hi ha “marcats” teclat i scon sinó només un: aquell al que han  */
		 /* arribat dades i que cal llegir (de fet, podrien ser dos si arriben dades als dos */
		 if(FD_ISSET (TECLAT,&conjunt)) /* el teclat està “marcat”? han arribat dades al teclat? */
		 {
			 readFromKeyboard(&data);
			//fgets(data.buffer,200, stdin);
			if (strcmp(data.buffer,"$\n") == 0) {
				printf("rebut $ tancant la connexio\n");
				break;
			}

			data.number_bytes=write(scon,data.buffer,strlen(data.buffer));

			if(data.number_bytes<0)  {
				error("Error al escriure al socket");
			}
			else{
				printf("Enviat\n");
			}
		 }
		 if(FD_ISSET (scon,&conjunt)) /* el socket scon està “marcat”? han arribat dades a scon? */
		 {

				printf("missatge rebut: \n");
				data = read_from_socket(scon);
				printf("%s",data.buffer);
		 }
		 if(volta == 0){
			 	printf("El servidor %s s'ha connectat amb el servidor : %s\n", nick_local, data.buffer);
				volta++;
		 }

	}


	/* Es tanquen els sockets scon (la connexió) i sesc */
	close(scon);
	close(sesc);

	return(0);
}
