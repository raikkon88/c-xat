#include "user_interface.h"

/**
 * USER INTERFACE CODE FILE.
 */

int readFromKeyboard(struct string * inData){
  int bytes_llegits;
  char buffer[200] = "";
  /* S'envia pel socket connectat scon el que es rep pel teclat */
	if((bytes_llegits=read(0, buffer,sizeof(buffer)))==-1)
	{
    inData->number_bytes = -3;
	}
  else{
    inData->number_bytes = strlen(buffer) - 1;
    memset(inData->buffer, '\0', sizeof(buffer));
    strncpy(inData->buffer, buffer, strlen(buffer) - 1);
  }
  return bytes_llegits;
}

int printOnScreen(char * bytes_llegits[]);

int readConnectionParams(struct connection_params * params){
  char buffer[16] = "";
  int i;

  int bytes_llegits;

  printf("Entra la @IP i el #port del servidor al que es vol connectar : \n");
  printf("@IP : \n");
  if(bytes_llegits=read(0,buffer,sizeof(buffer))==-1){ return (-1); }
  else{
    memset(params->ip, '\0', sizeof(buffer));
    strncpy(params->ip, buffer, strlen(buffer) - 1);
  }
  strcpy(buffer, "");
  printf("#port : \n");
  if(bytes_llegits=read(0,buffer,sizeof(buffer))==-1) { return (-2); }
  else{
    sscanf(buffer, "%d", &params->port);
  }
  return (0);
}

int printConnectionParams(struct connection_params params){
  printf("----------------------------------\n");
  printf("Print struct connection params\n");
  printf("%s\n", params.ip);
  printf("%i\n", params.port);
  printf("----------------------------------\n");
  return (0);
}

int printFunctionResult(int result){
  if(result < 0){
    struct error e = parseResult(result);
    printError(& e);
  }
  return(0);
}

int printError(struct error * e){
  printf("[ERROR]\n");
  printf(" MSG  -> %s\n", e->message);
  printf(" CODE -> %i\n", e->code);
  exit(-1);
}
