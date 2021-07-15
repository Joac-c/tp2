
#ifndef COUNT_MIN_SKETCH
#define COUNT_MIN_SKETCH
#include "stdlib.h"
#include "stdio.h"

typedef struct count_min_sec cms_t;

/*
 * Recibe una lista de cadenas(los hashtags)
 * y un tamaño(depende de la cantidad de elementos distintos en la lista)
 * Devuelve un cms cargado con la cantidad de apariciones de cada cadena
 */

cms_t* cargar_cms(char** hashtags, size_t tam);


/*
 * Recibe un cms cargado y un hashtag. 
 * Devuelve la cantidad de apariciones de dicho hashtag 
 */
size_t** devolver_apariciones_cms(cms_t* cms, char** hashtag);

void destruir_cms(cms_t* cms);


#endif  // COUNT_MIN_SKETCH