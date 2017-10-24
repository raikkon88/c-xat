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
	portloc = 3000;

	adrloc = create_socket_struct(portloc, iploc);
	/* Enllaça ip i port al socket */
	make_bind(sesc, adrloc);
	/* Es crea una cua per emmagatzemar peticions de connexió pendents */
	make_connection_queue(sesc);
	/* S'accepta la connexió */
	scon = accept_connection(sesc, adrrem);
	/* Mostrem paràmetres del client */
	show_connection_params(adrrem, scon);
	
	while(strcmp(data.buffer, "$")){
		/* Llegeix de socket */
		data = read_from_socket(scon);
		/* Escriu per pantalla */
		bytes_escrits = write_screen(data, scon);
	}

	/* Es tanquen els sockets scon (la connexió) i sesc */
	close(scon);
	close(sesc);

	return(0);
}
