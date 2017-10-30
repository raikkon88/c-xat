# Xat, Pràctica de xarxes

> Autors : Feng Lin i Marc Sànchez

## Client 

### Crides 

#### getSockName() -> Per saber la l'adreça del servidor o del client després de fer la connexió. 

#### socket 

Crea un socket

- Primer paràmetre -> AF_INET és una constant que assigna l'arquitectura d'internet. 
- Segon paràmetre -> SOCK_STREAM és un protocol que defineix TCP, en cas que sigui UDO seria SOCK_DATA, es defineix una part de paquet.

#### Bind 

Al client actualment no cal fer un bind, peró es pot fer, el que passa és que si no es fa el sistema operatiu assigna un port lliure. 

#### connect

El primer argument és el descriptor del socket i els segon argument és l'estructura per referència de on s'ha de connectar el servidor. 

## Servidor

### Crides 

#### bind 

Assigna una ip i un port a un descriptor de un socket. 

#### Listen 

Crea una cua de peticions de connexió. La cua sol ser sense límit. 

#### accept 

Desencua una petició de connexió, reb el descriptor del socket, l'estructura de dades del client per que l'empleni i la longitud de l'estructura. 

Un cop s'ha fet l'accept es retorna un descriptor del socket on ha quedat connectat el client i el servidor, en aquest moment s'ha de parlar directament per aquest scon. 

#### select

Serveix per manterin una conversa. Un cop tots han fet les crides de connexio els dos fan un select(teclat, socket). Llavors si arriva una lína per teclat es fa un llegir enviar i si arriva per el socket es fa un llegir i mostrar per pantalla. 

PAS 2
======

CreaSocketServidor -> socket, bind i listen
CreaSocketClient -> Socket, bind (opcional)
Accepta -> accept arguments(ip i port del client)
DemanaConnexió -> connect
HaArrivatAlgunaCosa -> select

PAS 3
=======

S'ha de fer que amb el select escolti 3 possibles casos, una connexió tcp, una string d'un socket connectat o el teclat. 

El codi farà un socket, un bind i un listen i un select. 

Quan surt del select s'ha de saber què ha passat, per saber-ho hem de mirar què ha arrivat. Si entra un socket farem un accept, en canvi si ve de teclat farem un socket i un connect. 

Tot seguit es fa el while amb un select. 

### PROTOCOL : 

La idea és que els missatges tinguin un significat i que els camps possibles que s'envien siguin estandaritzats amb tots els programes. 

Donats 2 processos, P1 i P2. 

#### 1 - Com P1 li diu a P2 que vol iniciar la conversa?

P1 fa connect i P2 fa un accept per tant p1 fa Petició inici de connexió i P2 fa resposta de petició.

#### 2 - Com P1 li diu a P2 que vol acabar?

La idea és fer una desconnexió : A nivell de transport P1 li enviarà una petició de fi de connexió i P2 enviarà resposta de petició. 

Es fa un close de un socket TCP, llavors s'envien els missatges que s'han comentat a la línia anterior. llavors quan es fa el read retorna un 0. 

La interfície usuari aplicació li és igual com es tenca el programa per que el missatge final no té importància de com es tenca. 

#### 3 - Nicknames 

Es construeixen missatges amb 3 camps : 

- Tipus -> (L,N) de línia o nickname
- Longitud -> 3bytes
- Informació -> El contingut del nickname o les línies. 

#### 4 - Línies

No tenen ni \n ni \0. 

Quan arriven les línies es poden processar com un string o llegint byte a byte. 


#### Eines! 

- sprintf() -> Enlloc de imprimir per pantalla ho imprimeix en un string. 

	char miss[100];
	sprintf(miss, "%s%.3d%s", tipus, longitud, informacio); 
	
El resultat serà amb \0 peró s'ha de treure. El %.3d formatarà el tipus amb 3 bytes. 4 -> 004

- strlen("hola\0") -> retorna 4 i s'han ocupat 5 bytes. 
