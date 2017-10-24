#ifndef INTERFACE_SOCKETS_H_   /* Include guard */
#define INTERFACE_SOCKETS_H_

/**
 * INTERFACE SOCKETS HEADER FILE
 */

int create_socket(int sesc);

struct sockaddr_in create_socket_struct(int port, char inet[]);

int make_connection(int fileDescriptor, struct sockaddr_in address);

int send_data(char data[], int number_bytes, int fileDescriptor);

int make_connection_queue(int fileDescriptor);

int make_bind(int fileDescriptor, struct sockaddr_in address);

int accept_connection(int fileDescriptor, struct sockaddr_in address);

struct string read_from_socket(int socket);

#endif
