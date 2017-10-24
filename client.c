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
	int scon, i;
	int bytes_llegits, bytes_escrits;
	char buffer[200];
	char end[200] = "$";
	int long_adrrem;
	struct sockaddr_in adrrem;
	struct connection_params params;
	struct string inData;
	int portrem;

	/* Es llegeixen els paràmetres de connexió del servidor */
	printFunctionResult(readConnectionParams((struct connection_params*)&params));
	/* L'adreca del socket del servidor (socket remot) */
	scon = create_socket(scon);
	printFunctionResult(scon);
	/* Es creen els sockets */
	adrrem = create_socket_struct(params.port, params.ip);
	/* S'obre la connexió */
	printFunctionResult(make_connection(scon, adrrem));
  /* Es llegeix de teclat el que es vol enviar */
	while(strcmp(inData.buffer,end)){
		readFromKeyboard(&inData);
		printFunctionResult(inData.number_bytes);
		/* S'envia pel socket connectat scon el que es rep pel teclat */
		send_data(inData.buffer,inData.number_bytes,scon);
	}

	/* Es tanca el socket scon (la connexió) */
	close(scon);

	return(0);
}
