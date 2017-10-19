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
	int sesc, scon, i;
	int bytes_llegits, bytes_escrits;
	char buffer[200];
	int long_adrrem;
	struct sockaddr_in adrloc, adrrem;
	char iploc[16];
	int portloc;

	sesc = create_socket(sesc);
	if(sesc >= 0){
		printf("Socket creat a : %i\n", sesc);
	}

	/* L'adreca del socket del servidor (socket local) */
	strcpy(iploc,"0.0.0.0"); /* 0.0.0.0 correspon a INADDR_ANY */
	portloc = 3000;

	adrloc = create_socket_struct(portloc, iploc);

	if((bind(sesc,(struct sockaddr*)&adrloc,sizeof(adrloc)))==-1)
	{
		perror("bind\n");
		close(sesc);
		exit(-1);
	}
	
	/* Es crea una cua per emmagatzemar peticions de connexió pendents */
	if((listen(sesc,3))==-1)
	{
		perror("listen\n");
		close(sesc);
		exit(-1);
	}
	
	
	/* Espera una petició de connexió d’un client (via connect()), i un cop rebuda, */
	/* l’accepta. El descriptor de socket retornat, scon, serà el nou identificador */
	/* del socket local per aquesta connexió. Els descriptors sesc i scon tenen la */
	/* mateixa @IP i mateix #port TCP, però sesc és un socket “d’escolta” i scon un */
	/* socket “connectat”. En aquest cas, però, sesc no té una @IP concreta, sinó */
	/* més d’una, ja que està lligat a totes les @IP de la màquina (@IP 0.0.0.0), i */
	/* scon tindrà la “mateixa” @IP, però “concretada” a una d’elles: a l’@IP que */
	/* hagi fet servir el client en la petició de connexió */
	long_adrrem=sizeof(adrrem);
	if((scon=accept(sesc,(struct sockaddr*)&adrrem, &long_adrrem))==-1)
	{
		perror("accept\n");
		close(sesc);
		exit(-1);
	}
	
	printf("Acceptada la connexió amb %i \n", scon); 
	
	/* S'escriu per pantalla el que arriba pel socket connectat scon */
	if((bytes_llegits=read(scon,buffer,sizeof(buffer)))==-1)
	{
		perror("read\n");
		close(scon);
		exit(-1);
	}
	
	if((bytes_escrits=write(1,buffer,bytes_llegits))==-1)
	{
		perror("write\n");
		close(scon);
		exit(-1);
	}
	/* Es tanquen els sockets scon (la connexió) i sesc */
	close(scon);
	close(sesc);
	
	return(0);
} 
