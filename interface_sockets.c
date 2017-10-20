#include "interface_sockets.h"


/**
 * Interfície de sockets compartida
 */


/* Es crea el socket (local) TCP sesc del servidor */
int create_socket(int sesc){

	if((sesc=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		return -100;
	}
	return sesc;
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

int make_connection(int fileDescriptor, struct sockaddr_in * address){
	printf("anem per Connectar\n");
	if((connect(fileDescriptor,(struct sockaddr*)&address,sizeof(address)))==-1)
	{
		close(fileDescriptor);
		printf("Error dins make connection retorna -101\n");
		return (-101);
	}
	else{
		printf("Connectat a %i \n", fileDescriptor);
		return fileDescriptor;
	}

}
