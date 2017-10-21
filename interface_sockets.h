#ifndef INTERFACE_SOCKETS_H_   /* Include guard */
#define INTERFACE_SOCKETS_H_

/**
 * INTERFACE SOCKETS HEADER FILE
 */

int create_socket(int sesc);

struct sockaddr_in create_socket_struct(int port, char inet[]);

int make_connection(int fileDescriptor, struct sockaddr_in address);

int sendData(char data[], int number_bytes, int fileDescriptor);

#endif
