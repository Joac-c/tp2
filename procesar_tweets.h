#ifndef PROCESAR_TWEETS
#define PROCESAR_TWEETS
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "tools/count_min_sketch.h"
#include "tools/strutil.h"
#include "tools/hash.h"


//Esta estructura contiene el hashtag y sus apariciones
typedef struct hashtag ht_t;

//Carga la tabla del count_min_sketch y el hash con una lista de hashtags
void cargar_hashtags(char** tokens, cms_t* cms, hash_t* hash);


//Compara dos hashtags por su cantidad de apariciones
int cmp_hashtag(const void* uno, const void* dos);


//Devuelve cada k lineas ingresadas por entrada estandar los n hashtags mas frecuentes
void procesar_tweets(size_t n, int k);

#endif  // PROCESAR_TWEETS