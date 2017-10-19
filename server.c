#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
 int sesc, scon, i;
 int bytes_llegits, bytes_escrits;
 char buffer[200];
 int long_adrrem;
 struct sockaddr_in adrloc, adrrem;
 char iploc[16];
 int portloc;
 /* Es crea el socket (local) TCP sesc del servidor */
 if((sesc=socket(AF_INET,SOCK_STREAM,0))==-1)
 {
 perror("socket\n");
 exit(-1);
 }

 /* L'adreca del socket del servidor (socket local) */
 strcpy(iploc,"0.0.0.0"); /* 0.0.0.0 correspon a INADDR_ANY */
 portloc = 3000;

/* S’assigna @IP i #port TCP a sesc: l’@IP ha de ser una de les @IP de la màquina i el */
 /* #port TCP ha d’estar lliure. Es pren @IP 0.0.0.0 i #port portloc. L’@IP 0.0.0.0 */
 /* no és una @IP “concreta” sinó que vol dir “totes”: sesc queda lligat a totes les @IP */
 /* de la màquina i escolta peticions de connexió que arribin a qualsevol d’elles. */
 adrloc.sin_family=AF_INET;
 adrloc.sin_port=htons(portloc);
 adrloc.sin_addr.s_addr=inet_addr(iploc); /* o bé: ...s_addr = INADDR_ANY */
 for(i=0;i<8;i++){adrloc.sin_zero[i]='0';}
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
