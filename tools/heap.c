#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "auxiliares.h"

#define CAPACIDAD_INICIAL 20
#define FACTOR_AGRANDAMIENTO 2
#define FACTOR_REDUCCION 2
#define PROPORCION_MAXIMA_CAPACIDAD_CANTIDAD 4

struct heap{
    void** elementos; //arreglo de punteros a elementos
    size_t cantidad;   
    size_t capacidad;
    cmp_func_t comparador;  
};

//Funciones auxiliares
void agregar_valor(heap_t* heap, void* valor){
        heap->elementos[heap->cantidad]=valor;
        heap->cantidad++;
}

void up_heap(void **elementos, size_t posicion_hijo, cmp_func_t cmp){
    if(!posicion_hijo){
        return;
    }
    size_t posicion_padre=(posicion_hijo-1)/2;
    if(cmp(elementos[posicion_padre], elementos[posicion_hijo])<0){
        swap(&(elementos[posicion_padre]), &(elementos[posicion_hijo]));
        up_heap(elementos, posicion_padre, cmp);
    }
}

void down_heap(void** elementos, size_t posicion_padre, size_t tam, cmp_func_t cmp){
    if(posicion_padre>=tam){
        return;
    }
    size_t posicion_hijo_der=calcular_posicion_hijos(posicion_padre, false);
    size_t posicion_hijo_izq=calcular_posicion_hijos(posicion_padre, true);
    size_t posicion_hijo_mayor=posicion_hijo_izq;
    if(posicion_hijo_izq>tam-1){
        return;
    }
    if(posicion_hijo_der<tam){
        if(cmp(elementos[posicion_hijo_izq], elementos[posicion_hijo_der])<0){
            posicion_hijo_mayor=posicion_hijo_der;
        }
    }
    if(cmp(elementos[posicion_padre], elementos[posicion_hijo_mayor])<0){
        swap(&(elementos[posicion_padre]), &(elementos[posicion_hijo_mayor]));
        down_heap(elementos, posicion_hijo_mayor, tam, cmp);
    }
}
void heapify(void** elementos, size_t tam, cmp_func_t cmp){
    int pivote=(int)obtener_pivote_heapify(tam);
    for(int i=pivote; i>=0; i--){
        down_heap(elementos, i, tam, cmp);
    }
}

void swap(void** a, void** b){
    void** temp=*a;
    *a=*b;
    *b=temp;
}

size_t calcular_posicion_padre(size_t posicion_hijo){
    return (posicion_hijo-1)/2;
}

size_t calcular_posicion_hijos(size_t posicion_padre, bool izq){
    if(izq==true){
        return 2*posicion_padre+1;
    }
    return 2*posicion_padre+2;
}

size_t obtener_pivote_heapify(size_t tam){
    return (tam)/2-1;
}

bool heap_redimensionar(heap_t* heap, bool expansion){
    if(expansion==false){
        heap->elementos=realloc(heap->elementos, sizeof(void*)*heap->capacidad/FACTOR_REDUCCION);
        heap->capacidad/=FACTOR_REDUCCION;
        return true;
    }
    void* aux=realloc(heap->elementos, sizeof(void*)*heap->capacidad*FACTOR_AGRANDAMIENTO);
    if(!aux){
        return false;
    }
    heap->elementos=aux;
    heap->capacidad*=FACTOR_AGRANDAMIENTO;
    return true;
}

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos, cant, cmp);
    while(cant>1){
        swap(&(elementos[0]), &(elementos[cant-1]));
        cant--;
        down_heap(elementos, 0, cant, cmp);
    }   
}

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap=malloc(sizeof(heap_t));
    if(!heap){
        return NULL;
    }
    if(!(heap->elementos=malloc(sizeof(void *)*CAPACIDAD_INICIAL))){
        free(heap);
        return NULL;
    }
    heap->capacidad=CAPACIDAD_INICIAL;
    heap->comparador=cmp;
    heap->cantidad=0;
    return heap;    
}

/*
 * Constructor alternativo del heap. Además de la función de comparación,
 * recibe un arreglo de valores con que inicializar el heap. Complejidad
 * O(n).
 *
 * Excepto por la complejidad, es equivalente a crear un heap vacío y encolar
 * los valores de uno en uno
*/
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap=malloc(sizeof(heap_t));
    if(!heap){
        return NULL;
    }
    if(!(heap->elementos=malloc(sizeof(void *)*n))){
        free(heap);
        return NULL;
    }
    heap->capacidad=n;
    heap->comparador=cmp;
    heap->cantidad=n;
    for(size_t i=0; i<n; i++){
        heap->elementos[i]=arreglo[i];
    }
    heapify(heap->elementos, n, heap->comparador);
    return heap;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){    
    if(destruir_elemento){
        while(heap_esta_vacio(heap)==false){
            (*destruir_elemento)(heap_desencolar(heap));
        }
    }
    free(heap->elementos);
    free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap){
    return heap->cantidad;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t *heap){
    return heap->cantidad==0;
}

/* Agrega un elemento al heap.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
bool heap_encolar(heap_t *heap, void *elem){
    if(heap->cantidad==heap->capacidad){
        if(heap_redimensionar(heap, true)==false){
            return false;
        }
    }
    agregar_valor(heap, elem);
    up_heap(heap->elementos, heap->cantidad-1, heap->comparador);
    return true;
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL.
 * Pre: el heap fue creado.
 */
void* heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)==true){
        return NULL;
    }
    return heap->elementos[0];
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void* heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)==true){
        return NULL;
    }
    void* elemento=heap_ver_max(heap);
    heap->cantidad--;
    swap(&(heap->elementos[0]), &(heap->elementos[heap->cantidad]));
    if(heap->capacidad >= heap->cantidad*PROPORCION_MAXIMA_CAPACIDAD_CANTIDAD && heap->cantidad>=(CAPACIDAD_INICIAL/FACTOR_REDUCCION)){
        heap_redimensionar(heap, false);
    }
    down_heap(heap->elementos, 0, heap->cantidad, heap->comparador);
    return elemento;
}