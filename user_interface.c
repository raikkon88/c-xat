#include "user_interface.h"

/**
 * USER INTERFACE CODE FILE.
 */

int readConnectionParams(struct connection_params * params){
  char buffer[16] = "";
  int i;

  int bytes_llegits;

  printf("Entra per aquest ordre la @IP del servidor i el #port al que es vol connectar : \n");
  printf("@IP : \n");
  if(bytes_llegits=read(0,buffer,sizeof(buffer))==-1){ return (-1); }
  else{
    memset(params->ip, '\0', sizeof(buffer));
    strncpy(params->ip, buffer, strlen(buffer) - 1);
    printf("%s\n", params->ip);
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
  printf("resultat : %i\n", result);
  if(result < 0){
    printf("ERROR !! \n");
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