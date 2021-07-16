#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdbool.h>  // bool
#include <stddef.h>   // size_t

//Funciones auxiliares

/* Recibe como parámetros un arreglo de punteros, la posición dentro del arreglo de la dirección del hijo al que le quiero
 * aplicar up heap y el comparador.
 */
void up_heap(void **elementos, size_t posicion_hijo, cmp_func_t cmp);

/* Recibe como parámetros un arreglo de punteros, la posición dentro del arreglo de la dirección del padre al que le quiero
 * aplicar down heap, el tamaño del heap y el comparador.
 */
void down_heap(void** elementos, size_t posicion_padre, size_t tam, cmp_func_t cmp);

/* Recibe como parámetros un arreglo de punteros al que le quiero aplicar heapify, el tamaño del mismo y el comparador.
 */
void heapify(void** elementos, size_t tam, cmp_func_t cmp);

/* Recibe como parámetros dos punteros dobles a dos elementos e intercambia donde apunta cada uno.
 */
void swap(void** a, void** b);

/* Recibe un puntero al heap que se desea redimesionar y un booleano que en caso de ser true lo expande y si es false lo achica.
 * En caso de tener éxito en la operación devuelve true, sino false.
 */
bool heap_redimensionar(heap_t* heap, bool expansion);

/* Recibe la posición del hijo dentro del heap y devuelve la posición de su padre
 */
size_t calcular_posicion_padre(size_t posicion_hijo);

/* Recibe la posición del hijo dentro del heap y un booleano.
 * En caso de que el booleano sea true devuelve la posición de su hijo izquierdo, sino del derecho.
 */
size_t calcular_posicion_hijos(size_t posicion_padre, bool izq);

/* Recibe el tamaño del arreglo y devuelve el pivote para heapify
 */
size_t obtener_pivote_heapify(size_t tam);

/* Recibe un puntero al heap y al elemento que se desea agregar. Luego lo agrega.
 */
void agregar_valor(heap_t* heap, void* valor);

void pruebas_heap_estudiante(void);

#endif  // AUXILIARES_H