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
 int scon, i;
 int bytes_llegits, bytes_escrits;
 char buffer[200];
 int long_adrrem;
 struct sockaddr_in adrrem;
 char iprem[16];
 int portrem;
 /* Es crea el socket (local) TCP scon del client */
 if((scon=socket(AF_INET,SOCK_STREAM,0))==-1)
 {
 perror("socket\n");
 exit(-1);
 }
 /* L'adreca del socket del servidor (socket remot) */
 strcpy(iprem,"10.0.0.23");
 portrem = 3000;

 /* Per assignar @IP i #port TCP a scon el programador ho podria fer de manera explícita */
 /* via bind(), però aquí es fa implícitament, és a dir, ho fa el S.O. (que escull una */
 /* @IP qualsevol de la màquina i un #port TCP lliure). Això ho farà connect() */
 /* Es connecta scon al socket remot del servidor. Primer s’omple l’adreça adrrem */
 /* que conté l’adreça del socket del servidor i després es fa connect(). A més, sense */
 /* bind(), connect() també farà que s’assigni implícitament @IP i #port TCP a scon */
 adrrem.sin_family=AF_INET;
 adrrem.sin_port=htons(portrem);
 adrrem.sin_addr.s_addr= inet_addr(iprem);
 for(i=0;i<8;i++){adrrem.sin_zero[i]='0';}
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
