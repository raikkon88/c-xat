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

	sesc = create_socket(sesc);
	printFunctionResult(sesc);

	/* L'adreca del socket del servidor (socket local) */
	strcpy(iploc,"0.0.0.0"); /* 0.0.0.0 correspon a INADDR_ANY */
	portloc = 3000;

	adrloc = create_socket_struct(portloc, iploc);
	/* Enllaça ip i port al socket */
	printFunctionResult(make_bind(sesc, adrloc));
	/* Es crea una cua per emmagatzemar peticions de connexió pendents */
	printFunctionResult(make_connection_queue(sesc));

	scon = accept_connection(sesc, adrrem);
	printFunctionResult(scon);
	long_adrrem=sizeof(adrrem);

	if (getpeername(scon, (struct sockaddr *)&adrrem, &long_adrrem) == -1)
 	{
 		perror("Error en getpeername");
 		close(scon);
 		exit(-1);
	}
	printf("Sock REM: @IP %s,TCP,#port %d\n",inet_ntoa(adrrem.sin_addr),ntohs(adrrem.sin_port));


	while(strcmp(data.buffer, "$")){
		// Llegeix
		data = read_from_socket(scon);
		printFunctionResult(data.number_bytes);
		if((bytes_escrits=write(1,data.buffer,data.number_bytes))==-1)
		{
			perror("write\n");
			close(scon);
			exit(-1);
		}
	}

	/* Es tanquen els sockets scon (la connexió) i sesc */
	close(scon);
	close(sesc);

	return(0);
}
