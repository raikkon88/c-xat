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
	int long_adrrem;
	struct sockaddr_in adrrem;
	struct connection_params params;
	//char iprem[16];
	int portrem;

	/* Es llegeixen els paràmetres de connexió del servidor */
	printFunctionResult(readConnectionParams((struct connection_params*)&params));
	/* L'adreca del socket del servidor (socket remot) */
	printFunctionResult(create_socket(scon));
	/* Es creen els sockets */
	adrrem = create_socket_struct(params.port, params.ip);
	/* S'obre la connexió */
	printFunctionResult(make_connection(scon, & adrrem));

	/*if((connect(scon,(struct sockaddr*)&adrrem,sizeof(adrrem)))==-1)
	{
		printf("Error dins make connection retorna -101\n");
		close(scon);
		exit(-1);
	}*/

	/* S'envia pel socket connectat scon el que es rep pel teclat */
	if((bytes_llegits=read(0,buffer,sizeof(buffer)))==-1)
	{
		perror("read\n");
		close(scon);
		exit(-1);
	}
	printf("Nombre de bytes : %i", sizeof(bytes_llegits));
	while(sizeof(bytes_llegits) != 0){
		if((bytes_escrits=write(scon,buffer,bytes_llegits))==-1)
		{
			perror("write\n");
			close(scon);
			exit(-1);
		}
		/* S'envia pel socket connectat scon el que es rep pel teclat */
		if((bytes_llegits=read(0,buffer,sizeof(buffer)))==-1)
		{
			perror("read\n");
			close(scon);
			exit(-1);
		}
		printf("Nombre de bytes : %i", sizeof(bytes_llegits));
	}

	/* Es tanca el socket scon (la connexió) */
	close(scon);

	return(0);
}
