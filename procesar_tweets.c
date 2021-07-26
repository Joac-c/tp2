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


#define ERROR "ERROR\n"

bool levantar_error(){
    printf(ERROR);
    return false;
}





typedef struct hashtag
{
    size_t apariciones;
    char* clave;
}ht_t;


void liberar_ht(void* elem){

    ht_t* ht = (ht_t*)elem;
    free(ht->clave);
    free(ht);
}

int cmp_alfabetico(char* hashtag1, char* hashtag2){
    int a = 0;
    int b = 0;
    while (true){
        if(hashtag1[a] < hashtag2[b]) return 1;
        if(hashtag1[a] > hashtag2[b]) return -1;
        if(hashtag1[a] == '\0' && hashtag2[b] == '\0') return 0;
        a++;
        b++;
    }
    
}

int cmp_hashtag(const void* uno, const void* dos){
    ht_t* ht1 = (ht_t*)uno;
    ht_t* ht2 = (ht_t*)dos;
    if(ht1->apariciones > ht2->apariciones) return 1;
    if(ht1->apariciones == ht2->apariciones) return cmp_alfabetico(ht1->clave, ht2->clave);
    return -1; 
}



bool imprimir_ht(heap_t* heap, int k){
    int i = 0;
    bool exito = true;
    while(!heap_esta_vacio(heap) && i < k){
        ht_t* ht = heap_desencolar(heap);
        if(!ht || !ht->apariciones || !ht->clave) exito = false;
        if(exito) printf("%ld %s\n", ht->apariciones, ht->clave);
        if(ht) liberar_ht(ht); //Libero el hashtag impreso
        if(!exito) break;
        i++;
    }
    return exito;

}


void imprimir_tokens(char** tokens){
    for( int i = 0; tokens[i] != NULL; i++) printf("Token %d es %s ", i, tokens[i]);
    printf("\n");
    
}

void cargar_hashtags(char** tokens, cms_t* cms, hash_t* hash ){
    int i = 1;
    while (tokens[i] != NULL){
        cargar_cms(cms, tokens[i]);
        //printf("las apariciones de %s son: %ld\n", tokens[i],  devolver_apariciones_cms(cms, tokens[i]));
        hash_guardar(hash, tokens[i], tokens[i]);

        i++;
    }
    free(tokens[0]);
    
}


bool descargar_tweets(cms_t* cms, hash_t* hash, size_t n){
    char* cadena = NULL;
    size_t tam = 0;
    ssize_t cargados =  getline(&cadena, &tam, stdin);
    if(cargados <= 1) return false;
    for(size_t i = 0; i < n && cargados > 1; i++){
        cadena[cargados - 1] = '\0';
        char** tokens = split(cadena, ',');
        //imprimir_tokens(tokens);
        cargar_hashtags(tokens, cms, hash);
        free(tokens);
        cargados = getline(&cadena, &tam, stdin);
        
    }
    
    free(cadena);
    return true;
}



bool cargar_heap(heap_t* heap, cms_t* cms, hash_t* hash){
    hash_iter_t* iter = hash_iter_crear(hash);
    bool exito = true;
    if(!iter) exito = false;
    while(!hash_iter_al_final(iter)){
        char* clave = (char*)hash_iter_ver_actual(iter);
        
        size_t  min = devolver_apariciones_cms(cms, clave);
        //printf("las apariciones de %s son: %ld\n", clave, min);
        ht_t* ht = calloc(1, sizeof(ht_t));  //Reservo memoria para cada hashtag
        ht->apariciones = min;
        ht->clave = (char*)hash_obtener(hash, clave);
        exito = heap_encolar(heap, ht);
        hash_iter_avanzar(iter);
    }
        
    //Destruyo el iterador, el hash y el cms
    hash_iter_destruir(iter);
    return exito;
}    



void destruir_hash_cms(hash_t* hash, cms_t* cms){
    destruir_cms(cms);
    hash_destruir(hash);
}


void procesar_tweets(size_t n, int k){
    
    cms_t* cms;
    hash_t* hash;
    heap_t* heap;
    bool exito = true;
    cms = crear_cms(10);
    int iters = 0;
    while (exito){
        iters++;
        hash = hash_crear(NULL);
        heap = heap_crear(cmp_hashtag);
        

        //Cargo el hash con los hashtags y el cms con las apariciones estimadas de cada uno 
        exito = descargar_tweets(cms, hash, n);
        
        if(!exito){
            //levantar_error();
            heap_destruir(heap, liberar_ht);
            hash_destruir(hash);
            break;
        }
        
        printf("--- %d\n", k);
        //Cargo en el heap los hashtags(con sus apariciones) ordenados por cantidad de apariciones
        exito = cargar_heap(heap, cms, hash);
        //Destruyo el hash y el cms
        hash_destruir(hash);
        
        if(!exito){ 
            //printf("cargar heap ");
            levantar_error();
            heap_destruir(heap, liberar_ht);
            break;
        }
        //else printf("Exito cargando el heap\n\n");
        
        //Desencolo n hashtags del heap y los imprimo en pantalla junto con sus apariciones. 
        if(!imprimir_ht(heap, k)){
            //printf("imprimir ");
            levantar_error();
        
        }
        
        //Destruyo el heap liberando los hashtags sobrantes
        heap_destruir(heap, liberar_ht);
    }
    destruir_cms(cms); 
}


int main(){
    /*
    cms_t* cms = crear_cms(5);
    cargar_cms(cms, "spymaster");
    printf("\n");
    cargar_cms(cms, "poncho");
    printf("\n");
    cargar_cms(cms, "spymaster");
    printf("\n");
    cargar_cms(cms, "dqwñeoinouygyfgv");
    printf("\n");
    cargar_cms(cms, "spymaster");
    printf("\n");
    cargar_cms(cms, "wqfhvb");
    printf("\n");
    printf("las apariciones son: %ld\n", devolver_apariciones_cms(cms, "spymaster"));
    destruir_cms(cms);
   */
   procesar_tweets(3, 2);
}