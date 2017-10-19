#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "interface_sockets.c"

int main(int argc,char *argv[])
{
	int scon, i;
	int bytes_llegits=0, bytes_escrits;	
	char buffer[200];
	int long_adrrem;
	struct sockaddr_in adrrem;
	char iprem[16];
	int portrem;
	
	printf("bytes %i\n", bytes_llegits);
	printf("Entra per aquest ordre la @IP del servidor i el #port al que es vol connectar : \n");
	printf("@IP :\n");

	if(bytes_llegits=read(0,buffer,sizeof(buffer))==-1){
		printf("Error de lectura de @IP\n");
	}
	printf("#port :");
	
 
	scon = create_socket(scon);
 
	if(scon >= 0){
		printf("Socket client creat a %i\n", scon);
	}
	
	/* L'adreca del socket del servidor (socket remot) */
	strcpy(iprem,"127.0.0.1"); // TODO : Cambiar per lectura de teclat
	portrem = 3000; // TODO : Cambiar per lectura de teclat

	adrrem = create_socket_struct(portrem, iprem);
	
	if((connect(scon,(struct sockaddr*)&adrrem,sizeof(adrrem)))==-1)
	{
		perror("connect\n");
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
	
	if((bytes_escrits=write(scon,buffer,bytes_llegits))==-1)
	{
		perror("write\n");
		close(scon);
		exit(-1);
	}

	/* Es tanca el socket scon (la connexió) */
	close(scon);
	
	return(0);
}
