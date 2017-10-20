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
