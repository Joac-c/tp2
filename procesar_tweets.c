#include "procesar_tweets.h"
#include "tools/heap.h"
#include "tools/strutil.h"
#include "tools/count_min_sketch.h"
#include "tools/hash.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"



void cargar_hashtags(char** tokens, cms_t* cms, hash_t* hash ){
    int i = 1;
    
    
    while (tokens[i]){
        cargar_cms(cms, tokens[i]);
        hash_guardar(hash, tokens[i], NULL);
    }
    
}


typedef struct hashtag
{
    size_t cantidad;
    const char* clave;
}ht_t;


int cmp_hashtag(const void* uno, const void* dos){
    ht_t* ht1 = (ht_t*)uno;
    ht_t* ht2 = (ht_t*)dos;
    if(ht1->cantidad < ht2->cantidad) return 1;
    if(ht1->cantidad == ht2->cantidad) return 0;
    return -1; 
}

void procesar_tweets(int n, int k){
    char* cadena = NULL;
    size_t tam = 0;
    ssize_t cargados = 2;
    cms_t* cms;
    hash_t* hash;
    heap_t* heap;
    hash_iter_t* iter;

    while (cargados > 1){
        cms = crear_cms(n/4);
        hash = hash_crear(NULL);
        heap = heap_crear(cmp_hashtag);


        //Cargo el hash con los hashtags y el cms con las apariciones estimadas de cada uno 
        for(int i = 0; i < k && cargados > 1; i++){
            cargados = 0;
            cargados = getline(&cadena, &tam, stdin);    
            char** tokens = split(cadena, ',');
            cargar_hashtags(tokens, cms, hash);
            free_strv(tokens);

        }

        //Cargo en el heap los hashtags(con sus apariciones) ordenados por cantidad de apariciones
        iter = hash_iter_crear(hash);
        while(!hash_iter_al_final(iter)){
            char* clave = (char*)hash_iter_ver_actual(iter);
            size_t  min = devolver_apariciones_cms(cms, clave);
            ht_t* ht = calloc(1, sizeof(ht_t));  //Reservo memoria para cada hashtag
            ht->cantidad = min;
            ht->clave = clave;
            heap_encolar(heap, ht);
        }

        //Destruyo el iterador, el hash y el cms
        hash_iter_destruir(iter);
        hash_destruir(hash);
        destruir_cms(cms);
        
        //Desencolo n hashtags del heap y los imprimo en pantalla junto con sus apariciones. 
        int i = 0;
        while(!heap_esta_vacio(heap) && i < n){
            ht_t* ht = heap_desencolar(heap);
            printf("%ld %s", ht->cantidad, ht->clave);
            free(ht); //Libero el hashtag impreso
        }

        //Destruyo el heap liberando los hashtags sobrantes
        heap_destruir(heap, free);
    }    
    free(cadena);
    
    

}


int main(){

    return 0;
}