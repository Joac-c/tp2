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

    size_t hash = 7017700;
    size_t c;

    while ((c = (size_t)(*clave++))) hash = ((hash << 13) + hash) + c; /* hash * 33 + c */
    return hash % tam;
}




size_t generar_indice_3(const char *clave, size_t tam){

    size_t hash = 8218;
    size_t c;

    while ((c = (size_t)(*clave++))) hash = ((hash << 21) + hash) + c; /* hash * 33 + c */
    return hash % tam;
}


size_t generar_indice_4(const char *clave, size_t tam){

    size_t hash = 1342;
    size_t c;

    while ((c = (size_t)(*clave++))) hash = ((hash << 3) + hash) + c; /* hash * 33 + c */
    return hash % tam;
}

size_t generar_indice_5(const char *clave, size_t tam){

    size_t hash = 3218;
    size_t c;

    while ((c = (size_t)(*clave++))) hash = ((hash << 11) + hash) + c; /* hash * 33 + c */
    return hash % tam;
}


typedef struct count_min_sec{
    size_t** arreglos;
    size_t tam;
}cms_t;


cms_t* crear_cms(size_t tam){
    
    size_t** arreglos = malloc(sizeof(size_t*)*5);
    arreglos[0] = calloc(tam, sizeof(size_t));
    arreglos[1] = calloc(tam, sizeof(size_t));
    arreglos[2] = calloc(tam, sizeof(size_t));
    arreglos[3] = calloc(tam, sizeof(size_t));
    arreglos[4] = calloc(tam, sizeof(size_t));
    cms_t* cms = calloc(1, sizeof(cms_t));
    cms->arreglos = arreglos;
    cms->tam = tam;
    
    
    return cms;
}


void cargar_cms(cms_t* cms, char* hashtag){
    
    size_t indice = 0;
    indice = generar_indice_1(hashtag, cms->tam);
    //printf("el indice 1 de %s es: %ld\n", hashtag,  indice);
    cms->arreglos[0][indice]++;

    indice = generar_indice_2(hashtag, cms->tam);
    //printf("el indice 2 de %s es: %ld\n", hashtag, indice);
    cms->arreglos[1][indice]++;

    indice = generar_indice_3(hashtag, cms->tam);
    //printf("el indice 3 de %s es: %ld\n", hashtag, indice);
    cms->arreglos[2][indice]++;

    indice = generar_indice_4(hashtag, cms->tam);
    //printf("el indice 4 de %s es: %ld\n", hashtag, indice);
    cms->arreglos[3][indice]++;

    indice = generar_indice_5(hashtag, cms->tam);
    //printf("el indice 5 de %s es: %ld\n", hashtag, indice);
    cms->arreglos[4][indice]++;
    
}


size_t devolver_apariciones_cms(cms_t* cms, char* hashtag){
        
        
        size_t indice1 = generar_indice_1(hashtag, cms->tam);
        
        size_t indice2 = generar_indice_2(hashtag, cms->tam);
        
        size_t indice3 = generar_indice_3(hashtag, cms->tam);
        
        size_t indice4 = generar_indice_4(hashtag, cms->tam);

        size_t indice5 = generar_indice_5(hashtag, cms->tam);

        size_t min = cms->arreglos[0][indice1];
        if(cms->arreglos[1][indice2] < min) min = cms->arreglos[1][indice2];
        if(cms->arreglos[2][indice3] < min) min = cms->arreglos[2][indice3];
        if(cms->arreglos[3][indice4] < min) min = cms->arreglos[3][indice4];
        if(cms->arreglos[4][indice5] < min) min = cms->arreglos[4][indice5];
        return min;

}
    


void destruir_cms(cms_t* cms){
    for(int i = 0; i<5; i++) free(cms->arreglos[i]);
    free(cms->arreglos);
    free(cms);
}











