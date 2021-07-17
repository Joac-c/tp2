#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"



void capturar(){
    char* cadena = NULL;
    size_t tam = 0;
    ssize_t cargados;
    for(int i = 0; i < 4; i++){
        cargados = getline(&cadena, &tam, stdin);
        printf("linea");
        printf("%s", cadena);
    }
    free(cadena);
}


int main(){

    for(int i = 0; i < 4; i++){
        capturar();
        printf("CORTEE\n");
    }

}