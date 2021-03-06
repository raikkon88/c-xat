#ifndef USER_INTERFACE_H_   /* Include guard */
#define USER_INTERFACE_H_

#include "error_filter.c"

/************************************************
 *  USER INTERFACE HEADER FILE
 ************************************************/

struct connection_params{
  char ip[16];
  int port;
};

struct string{
  char buffer[200];
  int number_bytes;
};

int readFromKeyboard(struct string * inData);

int printOnScreen(char * buffer[]);

int printConnectionParams(struct connection_params params);

int readConnectionParams(struct connection_params * params);

int printFunctionResult(int result);

int printError(struct error * e);

#endif
