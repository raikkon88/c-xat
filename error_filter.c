/**
 * ERROR FILTER HEADER FILE
 */
#include "error_filter.h"


struct error parseResult(int result){
    struct error e;
    if(result == -1){
        strcpy(e.message,"Error en la lectura de la ip");
    }
    else if(result == -100){
        strcpy(e.message, "Error al crear els sockets");
    }
    else if(result == -101){
        strcpy(e.message, "Error al connectar");
    }

    e.code = result;
    return e;
}
