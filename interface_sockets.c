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

int make_connection(int fileDescriptor, struct sockaddr_in address){

	if((connect(fileDescriptor,(struct sockaddr*)&address,sizeof(address)))==-1)
	{
		close(fileDescriptor);
		return (-101);
	}
	return (0);
}

int sendData(char data[], int number_bytes, int fileDescriptor){
	int bytes_escrits;
	if((bytes_escrits=write(fileDescriptor,data,number_bytes))==-1)
	{
		close(fileDescriptor);
		return (-102);
	}
	return bytes_escrits;
}
