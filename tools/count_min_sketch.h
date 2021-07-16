
#ifndef COUNT_MIN_SKETCH
#define COUNT_MIN_SKETCH
#include "stdlib.h"
#include "stdio.h"

struct count_min_sketch;

typedef struct count_min_sec cms_t;


cms_t* crear_cms(size_t tam);

/*
 * Recibe un cms,  una lista de cadenas(los hashtags)
 * y un tamaño(depende de la cantidad de elementos distintos en la lista)
 * Carga el cms con las apariciones de cada cadena.
 */

void cargar_cms(cms_t* cms, char* hashtag);


/*
 * Recibe un cms cargado y un hashtag. 
 * Devuelve la cantidad de apariciones de dicho hashtag 
 */
size_t devolver_apariciones_cms(cms_t* cms, char* hashtag);

void destruir_cms(cms_t* cms);


#endif  // COUNT_MIN_SKETCH