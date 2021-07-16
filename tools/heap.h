#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>  // bool
#include <stddef.h>   // size_t

/* Prototipo de función de comparación que se le pasa como parámetro a las
 * diversas funciones del heap.
 * Debe recibir dos punteros del tipo de dato utilizado en el heap, y
 * debe devolver:
 *   menor a 0  si  a < b
 *       0      si  a == b
 *   mayor a 0  si  a > b
 */
typedef int (*cmp_func_t)(const void *a, const void *b);


/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);

/*
 * Implementación de un TAD cola de prioridad, usando un max-heap.
 *
 * Notar que al ser un max-heap el elemento mas grande será el de mejor
 * prioridad. Si se desea un min-heap, alcanza con invertir la función de
 * comparación.
 */

/* Tipo utilizado para el heap. */
typedef struct heap heap_t;

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t *heap_crear(cmp_func_t cmp);

/*
 * Constructor alternativo del heap. Además de la función de comparación,
 * recibe un arreglo de valores con que inicializar el heap. Complejidad
 * O(n).
 *
 * Excepto por la complejidad, es equivalente a crear un heap vacío y encolar
 * los valores de uno en uno
*/
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp);

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e));

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap);

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t *heap);

/* Agrega un elemento al heap.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
bool heap_encolar(heap_t *heap, void *elem);

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL.
 * Pre: el heap fue creado.
 */
void *heap_ver_max(const heap_t *heap);

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void *heap_desencolar(heap_t *heap);

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

#endif  // HEAP_H