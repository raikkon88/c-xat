#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "interface_sockets.c"
#include "user_interface.c"

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
	//char iprem[16];
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
	readFromKeyboard(&inData);
	printFunctionResult(inData.number_bytes);
	while(strcmp(inData.buffer,end)){
		/* S'envia pel socket connectat scon el que es rep pel teclat */
		sendData(inData.buffer,inData.number_bytes,scon);
		readFromKeyboard(&inData);
		printFunctionResult(inData.number_bytes);
		printf("Nombre de bytes : %i", sizeof(inData.number_bytes));
	}

	/* Es tanca el socket scon (la connexió) */
	close(scon);

	return(0);
}
