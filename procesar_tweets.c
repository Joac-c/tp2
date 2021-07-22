#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "procesar_tweets.h"
#include "tools/count_min_sketch.h"
#include "tools/heap.h"
#include "tools/strutil.h"
#include "tools/hash.h"


#define ERROR "ERROR"

bool levantar_error(){
    printf(ERROR);
    return false;
}


void cargar_hashtags(char** tokens, cms_t* cms, hash_t* hash ){
    int i = 1;
    while (tokens[i] != NULL){
        cargar_cms(cms, tokens[i]);
        hash_guardar(hash, tokens[i], NULL);
        i++;
    }
    
}


typedef struct hashtag
{
    size_t cantidad;
    char* clave;
}ht_t;


void liberar_ht(void* elem){

    ht_t* ht = (ht_t*)elem;
    free(ht->clave);
    free(ht);
}


int cmp_hashtag(const void* uno, const void* dos){
    ht_t* ht1 = (ht_t*)uno;
    ht_t* ht2 = (ht_t*)dos;
    if(ht1->cantidad < ht2->cantidad) return 1;
    if(ht1->cantidad == ht2->cantidad) return 0;
    return -1; 
}

void imprimir_tokens(char** tokens){
    for( int i = 0; tokens[i] != NULL; i++) printf("Token %d es %s ", i, tokens[i]);
}


ssize_t descargar_tweets(cms_t* cms, hash_t* hash, int k){
    char* cadena = NULL;
    size_t tam = 0;
    ssize_t cargados =  getline(&cadena, &tam, stdin);

    for(int i = 0; i < k && cargados > 1; i++){
        printf("la cadena es %s\n", cadena);
        char** tokens = split(cadena, ',');
        imprimir_tokens(tokens);
        cargar_hashtags(tokens, cms, hash);
        free_strv(tokens);
        cargados = getline(&cadena, &tam, stdin);
    }
    
    free(cadena);
    return cargados;
}



bool cargar_heap(heap_t* heap, cms_t* cms, hash_t* hash){
    hash_iter_t* iter = hash_iter_crear(hash);
    bool exito = true;
    if(!iter) exito = false;
    while(!hash_iter_al_final(iter)){
        char* clave = (char*)hash_iter_ver_actual(iter);
        size_t  min = devolver_apariciones_cms(cms, clave);
        ht_t* ht = calloc(1, sizeof(ht_t));  //Reservo memoria para cada hashtag
        ht->cantidad = min;
        ht->clave = clave;
        exito = heap_encolar(heap, ht);
    }
        
    //Destruyo el iterador, el hash y el cms
    hash_iter_destruir(iter);
    return exito;
}    



void destruir_hash_cms(hash_t* hash, cms_t* cms){
    destruir_cms(cms);
    hash_destruir(hash);
}


bool imprimir_ht(heap_t* heap, size_t n){
    int i = 0;
    bool exito = true;
    while(!heap_esta_vacio(heap) && i < n){
        ht_t* ht = heap_desencolar(heap);
        if(!ht || !ht->cantidad || !ht->clave) exito = false;
        if(exito) printf("%ld %s", ht->cantidad, ht->clave);
        if(ht) liberar_ht(ht); //Libero el hashtag impreso
        if(!exito) break;
    }
    return exito;

}


void procesar_tweets(size_t n, int k){
    

    cms_t* cms;
    hash_t* hash;
    heap_t* heap;
    ssize_t cargados = 2;
    bool exito = true;

    while (cargados > 1){
        cms = crear_cms(n/4);
        hash = hash_crear(NULL);
        heap = heap_crear(cmp_hashtag);
        printf("iteramos\n ");

        //Cargo el hash con los hashtags y el cms con las apariciones estimadas de cada uno 
        cargados = descargar_tweets(cms, hash, k);
        
        if(cargados <= 1){
            printf("descargar ");
            levantar_error();
            destruir_hash_cms(hash, cms);
            break;
        }

        //Cargo en el heap los hashtags(con sus apariciones) ordenados por cantidad de apariciones
        exito = cargar_heap(heap, cms, hash);

        //Destruyo el hash y el cms
        destruir_hash_cms(hash, cms);

        if(!exito){ 
            printf("cargar heap ");
            levantar_error();
            heap_destruir(heap, free);
            break;
        }

        //Desencolo n hashtags del heap y los imprimo en pantalla junto con sus apariciones. 
        if(!imprimir_ht(heap, n)){
            printf("imprimir ");
            levantar_error();
        
        }
        //Destruyo el heap liberando los hashtags sobrantes
        heap_destruir(heap, liberar_ht);
    } 
}


int main(){
    cms_t* cms = crear_cms(4);
    hash_t* hash = hash_crear(NULL);
    descargar_tweets(cms, hash, 4);
    destruir_hash_cms(hash, cms);
    return 0;
}