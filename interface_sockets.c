#include "interface_sockets.h"
/**
 * Interfície de sockets compartida
 */


/* Es crea el socket (local) TCP sesc del servidor */
int create_socket(int fd){
	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		return -100;
	}
	return fd;
}

/* S’assigna @IP i #port TCP a sesc: l’@IP ha de ser una de les @IP de la màquina i el */
/* #port TCP ha d’estar lliure. Es pren @IP 0.0.0.0 i #port portloc. L’@IP 0.0.0.0 */
/* no és una @IP “concreta” sinó que vol dir “totes”: sesc queda lligat a totes les @IP */
/* de la màquina i escolta peticions de connexió que arribin a qualsevol d’elles. */
struct sockaddr_in create_socket_struct(int port, char inet[]){

	int i;
	struct sockaddr_in adrloc;
	adrloc.sin_family=AF_INET;
	adrloc.sin_port=htons(port);
	adrloc.sin_addr.s_addr=inet_addr(inet); /* o bé: ...s_addr = INADDR_ANY */
	for(i=0;i<8;i++){adrloc.sin_zero[i]='0';}

	return adrloc;
}

int make_connection_queue(int fileDescriptor){
	if((listen(fileDescriptor,3))==-1)
	{
		close(fileDescriptor);
		return (-103);
	}
	return (0);
}

int make_bind(int fileDescriptor, struct sockaddr_in address){
	if((bind(fileDescriptor,(struct sockaddr*)&address,sizeof(address)))==-1)
	{
		perror("bind\n");
		close(fileDescriptor);
		return (-104);
	}
	return (0);
}

/* Espera una petició de connexió d’un client (via connect()), i un cop rebuda, */
/* l’accepta. El descriptor de socket retornat, scon, serà el nou identificador */
/* del socket local per aquesta connexió. Els descriptors sesc i scon tenen la */
/* mateixa @IP i mateix #port TCP, però sesc és un socket “d’escolta” i scon un */
/* socket “connectat”. En aquest cas, però, sesc no té una @IP concreta, sinó */
/* més d’una, ja que està lligat a totes les @IP de la màquina (@IP 0.0.0.0), i */
/* scon tindrà la “mateixa” @IP, però “concretada” a una d’elles: a l’@IP que */
/* hagi fet servir el client en la petició de connexió */
int accept_connection(int fileDescriptor, struct sockaddr_in address){
	int socket;
	int long_adrrem=sizeof(address);
	if((socket=accept(fileDescriptor,(struct sockaddr*)&address, &long_adrrem))==-1)
	{
		close(fileDescriptor);
		return (-105);
	}
	return socket;
}

int make_connection(int fileDescriptor, struct sockaddr_in address){

	if((connect(fileDescriptor,(struct sockaddr*)&address,sizeof(address)))==-1)
	{
		close(fileDescriptor);
		return (-101);
	}
	return (0);
}

int send_data(char data[], int number_bytes, int fileDescriptor){
	int bytes_escrits;
	if((bytes_escrits=write(fileDescriptor,data,number_bytes))==-1)
	{
		close(fileDescriptor);
		return (-102);
	}
	return bytes_escrits;
}

struct string read_from_socket(int socket){

		struct string data;
		char buffer[200] = "";
		if((data.number_bytes=read(socket, buffer,sizeof(buffer)))==-1)
		{
				close(socket);
				data.number_bytes = -106;
		}
		else{
				data.number_bytes = strlen(buffer);
		    memset(data.buffer, '\0', sizeof(buffer));
		    strncpy(data.buffer, buffer, strlen(buffer));
		}
		return data;
}
