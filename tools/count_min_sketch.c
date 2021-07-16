#include "stdlib.h"
#include "stdio.h"
#include "count_min_sketch.h"



size_t generar_indice_1(const char *clave, size_t tam){
    size_t hash = 5381;
    size_t c;

    while ((c = (size_t)(*clave++))) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % tam;
}

size_t generar_indice_2(const char *clave, size_t tam){

    size_t hash = 14216874;
    size_t c;

    while ((c = (size_t)(*clave++))) hash = ((hash << 46) + hash) + c; /* hash * 33 + c */
    return hash % tam;
}


size_t generar_indice_3(const char *clave, size_t tam){

    size_t hash = 8218971987;
    size_t c;

    while ((c = (size_t)(*clave++))) hash = ((hash << 21) + hash) + c; /* hash * 33 + c */
    return hash % tam;
}


typedef struct count_min_sec{
    size_t** arreglos;
    size_t tam;
}cms_t;


cms_t* crear_cms(size_t tam){
    size_t arreglo1[tam];
    size_t arreglo2[tam];
    size_t arreglo3[tam];

    for(size_t i = 0; i < tam; i++){
        arreglo1[i] = 0;
        arreglo2[i] = 0;
        arreglo3[i] = 0;
    } 
    size_t* arreglos[3];

    arreglos[0] = arreglo1;
    arreglos[1] = arreglo2;
    arreglos[2] = arreglo3;
    
    cms_t* cms = calloc(1, sizeof(cms_t));
    cms->arreglos = arreglos;
    cms->tam = tam;
    return cms;
}


void cargar_cms(cms_t* cms, char* hashtag){
    
    
    size_t indice = 0;
    indice = generar_indice_1(hashtag, cms->tam);
    cms->arreglos[0][indice]++;
    indice = generar_indice_2(hashtag, cms->tam);
    cms->arreglos[1][indice]++;
    indice = generar_indice_3(hashtag, cms->tam);
    cms->arreglos[2][indice]++;
    
}


size_t devolver_apariciones_cms(cms_t* cms, char* hashtag){
        
        
        size_t indice1 = generar_indice_1(hashtag, cms->tam);
        
        size_t indice2 = generar_indice_2(hashtag, cms->tam);
        
        size_t indice3 = generar_indice_3(hashtag, cms->tam);

        size_t min = cms->arreglos[0][indice1];
        if(cms->arreglos[1][indice2] < min) min = cms->arreglos[1][indice2];
        if(cms->arreglos[2][indice3] < min) min = cms->arreglos[2][indice3];
        return min;

}
    


void destruir_cms(cms_t* cms){
    free(cms);
}











